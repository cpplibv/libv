// Project: libv.fsw, File: src/libv/fsw/watcher.cpp

// hpp
#include <libv/fsw/watcher.hpp>
#include <libv/fsw/watcher.hxx>
// ext
#include <efsw/efsw.hpp>
// libv
#include <libv/algo/erase_if_unstable.hpp>
#include <libv/algo/linear_find.hpp>
#include <libv/utility/generic_path.hpp>
#include <libv/utility/is_parent_folder_of.hpp>


namespace libv {
namespace fsw {

// -------------------------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const Action& action) {
	switch (action) {
	case Action::create: return os << "create";
	case Action::remove: return os << "remove";
	case Action::modify: return os << "modify";
	case Action::rename: return os << "rename";
	}

	return os << "unknown";
}

std::ostream& operator<<(std::ostream& os, const Event& event) {
	if (event.action == Action::create)
		return os << "Created " << libv::generic_path(event.path);

	if (event.action == Action::remove)
		return os << "Removed " << libv::generic_path(event.path);

	if (event.action == Action::modify)
		return os << "Modified " << libv::generic_path(event.path);

	if (event.action == Action::rename)
		return os << "Moved " << libv::generic_path(event.path) << " from " << libv::generic_path(event.old_path);

	return os << "Unknown event";
}

// -------------------------------------------------------------------------------------------------

//void _debug_state(ImplWatcher& self) {
//	std::cout << "  -- --" << std::endl;
//	for (const auto& item : self.efsw_watcher->directories()) {
//		std::cout << "  -- EFSW Watch: " << item << std::endl;
//	}
//
//	for (const auto& item : self.watcher_entries) {
//		std::cout << "  -- Watch entry: " << item.path.string() << " ID:" << item.watchID << " RC:" << item.ref_count << std::endl;
//	}
//
//	for (const auto& item : self.directories) {
//		std::cout << "  -- DIR: " << item.first << " ID:" << item.second.watcher_used << std::endl;
//		for (const auto& item2 : item.second.file_callbacks) {
//			std::cout << "  --    FILE: " << item2.first << std::endl;
//		}
//	}
//}

void _append_slash(std::filesystem::path& dir) {
	static constexpr decltype(std::filesystem::path::preferred_separator) sep[] = {std::filesystem::path::preferred_separator, 0};

	if (dir.native().back() != std::filesystem::path::preferred_separator)
		dir.concat(sep);
}

std::filesystem::path _parent_directory(const std::filesystem::path& dir) {
	auto result = dir;

	if (result.native().back() == std::filesystem::path::preferred_separator)
		result = result.parent_path();

	result = result.parent_path();
	_append_slash(result);
	return result;
}

/// Unsafe means that self's mutex has to be held by the caller
DirectoryCluster& _unsafe_watch_dir(ImplWatcher& self, std::filesystem::path dir) {
	const auto entry = libv::linear_find_if_optional(self.watcher_entries, [&dir](const auto& entry) {
		return libv::is_parent_folder_of(entry.path, dir);
	});

	efsw::WatchID id = 0;
	auto str_dir = dir.string();
	const auto key = str_dir;

	const auto emplace_pair = self.directories.try_emplace(key);
	const auto is_new_cluster = emplace_pair.second;
	auto& dir_entry = emplace_pair.first->second;

	if (entry) {
		if (is_new_cluster)
			entry->ref_count++;
		id = entry->watchID;

	} else {
		// Add watcher to an existing ancestor
		while (!dir.empty()) {
			id = self.efsw_watcher->addWatch(str_dir, &self, true);
			if (id != efsw::Error::FileNotFound)
				break;

			if (!dir.has_parent_path())
				break;

			dir = _parent_directory(dir);
			str_dir = dir.string();
		}

		if (id <= 0) {
			// TODO P5: handle and report EFSW error

			//	FileNotFound	= -1
			//	FileRepeated	= -2
			//	FileOutOfScope	= -3
			//	FileNotReadable	= -4
			//	FileRemote		= -5
			//	Unspecified		= -6

		} else {
			uint32_t ref_count = 1;

			// Remove any watcher that is under the new watcher
			libv::erase_if_unstable(self.watcher_entries, [&](auto& watcher_entry) {
				if (!libv::is_parent_folder_of(dir, watcher_entry.path))
					return false;

				for (auto& dir_entry : self.directories) {
					if (dir_entry.second.watcher_used == watcher_entry.watchID) {
						dir_entry.second.watcher_used = id;
						ref_count++;
					}
				}

				self.efsw_watcher->removeWatch(watcher_entry.watchID);

				return true;
			});

			self.watcher_entries.emplace_back();
			self.watcher_entries.back().watchID = id;
			self.watcher_entries.back().path = dir;
			self.watcher_entries.back().ref_count = ref_count;
		}
	}

	if (dir_entry.watcher_used == 0)
		dir_entry.watcher_used = id;

	return dir_entry;
}

/// Unsafe means that self's mutex has to be held by the caller
void _unsafe_unwatch(ImplWatcher& self, efsw::WatchID id) {
	const auto it = libv::linear_find_if_iterator(self.watcher_entries, [&](auto& watcher_entry) {
		return watcher_entry.watchID == id;
	});

	if (it != self.watcher_entries.end())
		return; // should never happen

	if (--it->ref_count != 0)
		return;

	self.efsw_watcher->removeWatch(it->watchID);
	self.watcher_entries.erase(it);
}

void _handle_file_action(
		ImplWatcher& self,
		Action action,
		std::filesystem::path path,
		std::filesystem::path old_path) {

	std::error_code ignore_ec;
	auto rel_path = std::filesystem::relative(path, ignore_ec);
	auto rel_old_path = std::filesystem::relative(old_path, ignore_ec);

	const auto is_dir = std::filesystem::is_directory(path, ignore_ec);
	if (is_dir) {
		_append_slash(path);
		_append_slash(old_path);
		_append_slash(rel_path);
		_append_slash(rel_old_path);
	}

	Event event_abs{action, path, old_path};
	Event event_rel{action, std::move(rel_path), std::move(rel_old_path)};

	std::unique_lock lock(self.mutex);

	// Broadcast to file callbacks
	if (!is_dir) {
		const auto dir = _parent_directory(path);
		const auto key = dir.string();
		const auto file = path.filename().string();

		auto entry_it = self.directories.find(key);

		if (entry_it != self.directories.end()) {
			const auto it = entry_it->second.file_callbacks.find(file);
			if (it != entry_it->second.file_callbacks.end()) {
				for (auto& cb : it->second) {
					if (*cb.callback) {
						(*cb.callback)(cb.uses_relative_path ? event_rel : event_abs);
					}
				}
			}
		}
	}

	// Broadcast to directory callbacks
	for (auto up_dir = path; true; up_dir = _parent_directory(up_dir)) {
		const auto up_key = up_dir.string();

		const auto up_it = self.directories.find(up_key);
		if (up_it != self.directories.end()) {
			for (auto& cb : up_it->second.callbacks) {
				if (*cb.callback) {
					(*cb.callback)(cb.uses_relative_path ? event_rel : event_abs);
				}
			}
		}

		if (!up_dir.has_relative_path())
			break;
	}
}

Watcher::token_type _subscribe_file(
		ImplWatcher& self,
		std::filesystem::path&& path,
		bool is_relative,
		Watcher::callback_type&& callback) {

	std::error_code ignore_ec;

	const auto str_file = path.filename().string();
	const auto dir = _parent_directory(path);
	const auto str_dir = dir.string();

	auto cb_up = std::make_unique<Watcher::callback_type>(std::move(callback));
	const auto ptr = cb_up.get();
	const auto token = Watcher::token_type(ptr);

	std::unique_lock lock(self.mutex);

	_unsafe_watch_dir(self, dir).file_callbacks[str_file].emplace_back(is_relative, std::move(cb_up));

//	_debug_state(self);

	return token;
}

Watcher::token_type _subscribe_directory(
		ImplWatcher& self,
		std::filesystem::path&& path,
		bool is_relative,
		Watcher::callback_type&& callback) {

	auto cb_up = std::make_unique<Watcher::callback_type>(std::move(callback));
	const auto ptr = cb_up.get();
	const auto token = Watcher::token_type(ptr);

	std::unique_lock lock(self.mutex);

	auto& entry = _unsafe_watch_dir(self, path);
	entry.callbacks.emplace_back(is_relative, std::move(cb_up));

//	_debug_state(self);

	return token;
}

void _unsubscribe(ImplWatcher& self, Watcher::token_type token) {
	std::unique_lock lock(self.mutex);

	auto dit = self.directories.begin();
	for (; dit != self.directories.end(); ++dit) {
		auto& dir_pair = *dit;
		auto& dir = dir_pair.second;

		const auto cit = libv::linear_find_if_iterator(dir.callbacks, [&](const auto& cb) {
			return token.id == cb.callback.get();
		});

		if (cit != dir.callbacks.end()) {
			dir.callbacks.erase(cit);
			break;
		}

		bool found = false;
		for (auto fit = dir.file_callbacks.begin(); fit != dir.file_callbacks.end(); ++fit) {
			auto& file = *fit;

			const auto fcit = libv::linear_find_if_iterator(file.second, [&](const auto& cb) {
				return token.id == cb.callback.get();
			});

			if (fcit != file.second.end()) {
				file.second.erase(fcit);

				if (file.second.empty())
					dir.file_callbacks.erase(fit);
				found = true;
				break;
			}
		}

		if (found)
			break;
	}

	if (dit->second.callbacks.empty() && dit->second.file_callbacks.empty()) {
		_unsafe_unwatch(self, dit->second.watcher_used);
		self.directories.erase(dit);
	}
}

// -------------------------------------------------------------------------------------------------

void ImplWatcher::handleFileAction(efsw::WatchID id, const std::string& dir, const std::string& filename, efsw::Action action, std::string old_path) {
	(void) id;

	Action normal_action = [](efsw::Action a){
		switch (a) {
		case efsw::Action::Add: return Action::create;
		case efsw::Action::Delete: return Action::remove;
		case efsw::Action::Modified: return Action::modify;
		case efsw::Action::Moved: return Action::rename;
		}
		return Action::modify; // Assert false
	}(action);

	std::error_code ignore_ec;

	auto normal_path = std::filesystem::path(dir) / filename;
	normal_path = std::filesystem::weakly_canonical(normal_path, ignore_ec);
	normal_path = std::filesystem::absolute(normal_path, ignore_ec);
	normal_path.make_preferred();

	auto normal_old_path = std::filesystem::path(old_path);
	normal_old_path = std::filesystem::weakly_canonical(normal_old_path, ignore_ec);
	normal_old_path = std::filesystem::absolute(normal_old_path, ignore_ec);
	normal_old_path.make_preferred();

	_handle_file_action(*this, normal_action, normal_path, normal_old_path);
}

Watcher::Watcher() :
	self(std::make_unique<ImplWatcher>()) {
	self->efsw_watcher.emplace();
	self->efsw_watcher->watch();
}

Watcher::~Watcher() {
	// Unsure about EFSW cleanup so callbacks are cleaned up with the mutex held
	// If a late call would happen client callbacks would not suffer it.
	{
		std::unique_lock lock(self->mutex);

		for (const auto& entries : self->watcher_entries)
			self->efsw_watcher->removeWatch(entries.watchID);

		self->watcher_entries.clear();
		self->directories.clear();
	}

	// Force EFSW to clean itself up and clear any ongoing callback (handleFileAction) to finish
	self->efsw_watcher.reset();
}

Watcher::token_type Watcher::subscribe_file(std::filesystem::path path, callback_type callback) {

	std::error_code ignore_ec;
	const auto is_relative = path.is_relative();

	path = std::filesystem::weakly_canonical(path, ignore_ec);
	path = std::filesystem::absolute(path, ignore_ec);
	path.make_preferred();

	return _subscribe_file(*self, std::move(path), is_relative, std::move(callback));
}

Watcher::token_type Watcher::subscribe_directory(std::filesystem::path path, callback_type callback) {

	std::error_code ignore_ec;
	const auto is_relative = path.is_relative();

	path = std::filesystem::weakly_canonical(path, ignore_ec);
	path = std::filesystem::absolute(path, ignore_ec);
	path.make_preferred();
	_append_slash(path);

	return _subscribe_directory(*self, std::move(path), is_relative, std::move(callback));
}

void Watcher::unsubscribe(token_type token) {
	_unsubscribe(*self, token);
}

// -------------------------------------------------------------------------------------------------

} // namespace fsw
} // namespace libv
