// Project: libv, File: app/update/patch/patch_applier.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <update/patch/patch_applier.hpp>
// libv
#include <libv/diff/diff.hpp>
#include <libv/utility/function_ref.hpp>
#include <libv/utility/write_file.hpp>
// std
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <string>
#include <variant>
#include <vector>
// pro
#include <update/patch/log.hpp>
#include <update/patch/patch.hpp>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

namespace { // -------------------------------------------------------------------------------------

[[nodiscard]] std::filesystem::path path_concat(const std::filesystem::path& path, const std::string_view str) {
	std::filesystem::path result = path;
	result += str;
	return result;
}

[[nodiscard]] bool file_exists(const std::filesystem::path& filepath) {
	std::error_code ec;
	const auto result = std::filesystem::exists(filepath, ec);
	log_update.error_if(ec, "Failed to check file existence for: {} - {} {}", filepath, ec, ec.message());
	return result;
}

[[nodiscard]] bool file_hash(const std::filesystem::path& filepath, const libv::hash::md5& hash) {
	const auto file_md5 = libv::hash::hash_file_md5_ec(filepath);
	if (file_md5.ec) {
		log_update.error("Failed to calculate file hash: {} - {} {}", filepath, file_md5.ec, file_md5.ec.message());
		return false;
	}

	return file_md5.hash == hash;
}

[[nodiscard]] auto file_exists_hash(const std::filesystem::path& filepath, const libv::hash::md5& hash) {
	struct Result {
		bool exists = false;
		bool matches = false;
	} result;

	result.exists = file_exists(filepath);
	if (!result.exists)
		return result;

	result.matches = file_hash(filepath, hash);
	return result;
}

// -------------------------------------------------------------------------------------------------

using failure_cb_ref_t = libv::function_ref<void(std::string, std::error_code)>;

// ---

struct OperationChain {
	bool aborted = false;
};

// ---

struct OperationCreateDir {
	std::filesystem::path path;
};

struct OperationRemoveDir {
	std::filesystem::path path;
};

struct OperationCreate {
	std::shared_ptr<OperationChain> chain;
	std::filesystem::path path;
	const file_full_data* data;
	libv::hash::md5 hash;
};

struct OperationModify {
	std::shared_ptr<OperationChain> chain;
	std::filesystem::path path_old;
	std::filesystem::path path_new;
	const file_diff_data* diff;
	libv::hash::md5 hash_old;
	libv::hash::md5 hash_new;
};

struct OperationRename {
	std::shared_ptr<OperationChain> chain;
	std::filesystem::path path_old;
	std::filesystem::path path_new;
	libv::hash::md5 hash;
};

struct OperationRemove {
	std::shared_ptr<OperationChain> chain;
	std::filesystem::path path;
	std::optional<libv::hash::md5> hash;
};

// -------------------------------------------------------------------------------------------------

template <typename Op>
void execute(const Op& op, const failure_cb_ref_t& failure_cb) {
	const auto success = do_execute(op, op.chain->aborted, failure_cb);
	op.chain->aborted &= !success;
}

void execute(const OperationCreateDir& op, const failure_cb_ref_t& failure_cb) {
	log_update.trace("Creating directory {}", op.path.generic_string());

	if (op.path.empty())
		return;

	std::error_code ec;
	std::filesystem::create_directories(op.path, ec);
	if (ec) {
		auto path_str = op.path.generic_string();
		log_update.error("Failed to create directory {} - {} {}", path_str, ec, ec.message());
		failure_cb(path_str, ec);
		return;
	}
}

void execute(const OperationRemoveDir& op, const failure_cb_ref_t& failure_cb) {
	log_update.trace("Removing directory {}", op.path.generic_string());

	if (op.path.empty())
		return;

	std::error_code ec;
	const auto exists = std::filesystem::exists(op.path, ec);
	if (ec) {
		auto path_str = op.path.generic_string();
		log_update.error("Failed to check directory existence for: {} - {} {}", op.path, ec, ec.message());
		failure_cb(path_str, ec);
		return;
	}

	if (!exists) {
		auto path_str = op.path.generic_string();
		log_update.warn("Directory that is marked for removal does not exist {}", path_str);
		// No failure only warning
		return;
	}

	const auto is_empty = std::filesystem::is_empty(op.path, ec);
	if (ec) {
		auto path_str = op.path.generic_string();
		log_update.error("Failed to test if directory is empty {} - {} {}", path_str, ec, ec.message());
		failure_cb(path_str, ec);
		return;
	}

	if (!is_empty) {
		auto path_str = op.path.generic_string();
		log_update.warn("Directory is not empty {}, skipping remove.", path_str);
		// No failure only warning
		return;
	}

	std::filesystem::remove(op.path, ec);
	if (ec) {
		auto path_str = op.path.generic_string();
		log_update.error("Failed to remove directory {} - {} {}", path_str, ec, ec.message());
		failure_cb(path_str, ec);
		return;
	}
}

[[nodiscard]] bool do_execute(const OperationCreate& op, bool aborted, const failure_cb_ref_t& failure_cb) {
	if (aborted)
		return false;
	log_update.trace("Creating {}", op.path.generic_string());

	std::error_code ec;

	const auto dir = op.path.parent_path();
	if (!dir.empty()) {
		std::filesystem::create_directories(dir, ec);

		if (ec) {
			auto path_str = op.path.generic_string();
			log_update.error("Failed to create directories for {} - {} {}", path_str, ec, ec.message());
			failure_cb(path_str, ec);
			return false;
		}
	}

	libv::write_file(op.path, *op.data, ec);
	if (ec) {
		auto path_str = op.path.generic_string();
		log_update.error("Failed to create {} - {} {}", path_str, ec, ec.message());
		failure_cb(path_str, ec);
		return false;
	}

	return true;
}

[[nodiscard]] bool do_execute(const OperationModify& op, bool aborted, const failure_cb_ref_t& failure_cb) {
	if (aborted)
		return false;
	log_update.trace("Modifying {} to {}", op.path_new.generic_string(), op.path_old.generic_string());

	std::error_code ec;

	auto file_old = std::ifstream(op.path_old, std::ios::binary | std::ios::in);
	if (!file_old) {
		auto path_str = op.path_old.generic_string();
		ec = std::make_error_code(static_cast<std::errc>(errno));
		log_update.error("Failed to open {} - {} {}", path_str, ec, ec.message());
		failure_cb(path_str, ec);
	}

	const auto dir_new = op.path_new.parent_path();
	if (!dir_new.empty()) {
		std::filesystem::create_directories(dir_new, ec);

		if (ec) {
			auto path_str = op.path_new.generic_string();
			log_update.error("Failed to create directories for {} - {} {}", path_str, ec, ec.message());
			failure_cb(path_str, ec);
			return false;
		}
	}

	auto file_new = std::ofstream(op.path_new, std::ios::binary | std::ios::out | std::ios::trunc);
	if (!file_new) {
		auto path_str = op.path_new.generic_string();
		ec = std::make_error_code(static_cast<std::errc>(errno));
		log_update.error("Failed to open {} - {} {}", path_str, ec, ec.message());
		failure_cb(path_str, ec);
	}

	if (!file_old || !file_new)
		return false;

	const auto success = libv::diff::apply_patch(file_old, *op.diff, file_new);
	if (!success) {
		auto path_str = op.path_old.generic_string();
		auto matches_expected_hash = file_hash(op.path_old, op.hash_old);

		if (matches_expected_hash) {
			log_update.error("Failed to apply patch to {}: File does not matches expected hash", path_str);
			failure_cb(path_str, std::make_error_code(std::errc::illegal_byte_sequence));
		} else {
			log_update.error("Failed to apply patch to {}: Internal error or data corruption", path_str);
			failure_cb(path_str, std::make_error_code(std::errc::state_not_recoverable));
		}
	}

	return success;
}

[[nodiscard]] bool do_execute(const OperationRename& op, bool aborted, const failure_cb_ref_t& failure_cb) {
	if (aborted)
		return false;
	log_update.trace("Renaming {} to {}", op.path_old.generic_string(), op.path_new.generic_string());

	std::error_code ec;

	const auto dir_new = op.path_new.parent_path();
	if (!dir_new.empty()) {
		std::filesystem::create_directories(dir_new, ec);

		if (ec) {
			auto path_str = op.path_new.generic_string();
			log_update.error("Failed to create directories for {} - {} {}", path_str, ec, ec.message());
			failure_cb(path_str, ec);
			return false;
		}
	}

	std::filesystem::rename(op.path_old, op.path_new, ec);
	if (ec) {
		auto path_str = op.path_old.generic_string();
		log_update.error("Failed to rename {} to - {} {}", path_str, op.path_new.generic_string(), ec, ec.message());
		failure_cb(path_str, ec);
	}

	return !ec;
}

[[nodiscard]] bool do_execute(const OperationRemove& op, bool aborted, const failure_cb_ref_t& failure_cb) {
	if (aborted)
		return false;
	log_update.trace("Removing {}", op.path.generic_string());

	std::error_code ec;

	const auto exists = std::filesystem::exists(op.path, ec);
	if (ec) {
		auto path_str = op.path.generic_string();
		log_update.error("Failed to check file existence for: {} - {} {}", op.path, ec, ec.message());
		failure_cb(path_str, ec);
		return false;
	}

	if (!exists) {
		auto path_str = op.path.generic_string();
		log_update.warn("File that is marked for removal does not exist {}", path_str);
		// No failure only warning
		return true;
	}

	std::filesystem::remove(op.path, ec);
	if (ec) {
		auto path_str = op.path.generic_string();
		log_update.error("Failed to remove {} - {} {}", path_str, ec, ec.message());
		failure_cb(op.path.generic_string(), ec);
	}

	return !ec;
}

// -------------------------------------------------------------------------------------------------

struct ExecutionPlan {
	std::vector<OperationCreateDir> create_dir_;
	std::vector<OperationCreate> creates;
	std::vector<OperationModify> modifies;
	std::vector<OperationRename> rename_pass1;
	std::vector<OperationRename> rename_pass2;
	std::vector<OperationRemove> removes;
	std::vector<OperationRemoveDir> remove_dir_;

public:
	inline void create_dir(std::filesystem::path path) {
		create_dir_.emplace_back(std::move(path));
	}

	inline void remove_dir(std::filesystem::path path) {
		remove_dir_.emplace_back(std::move(path));
	}

	inline void create(std::shared_ptr<OperationChain> chain, std::filesystem::path path, const file_full_data* data, libv::hash::md5 hash) {
		creates.emplace_back(std::move(chain), std::move(path), data, hash);
	}

	inline void modify(std::shared_ptr<OperationChain> chain, std::filesystem::path path_old, std::filesystem::path path_new, const file_diff_data* data, libv::hash::md5 hash_old, libv::hash::md5 hash_new) {
		modifies.emplace_back(std::move(chain), std::move(path_old), std::move(path_new), data, hash_old, hash_new);
	}

	inline void rename_1(std::shared_ptr<OperationChain> chain, std::filesystem::path path_old, std::filesystem::path path_new, libv::hash::md5 hash) {
		rename_pass1.emplace_back(std::move(chain), std::move(path_old), std::move(path_new), hash);
	}

	inline void rename_2(std::shared_ptr<OperationChain> chain, std::filesystem::path path_old, std::filesystem::path path_new, libv::hash::md5 hash) {
		rename_pass2.emplace_back(std::move(chain), std::move(path_old), std::move(path_new), hash);
	}

	inline void remove(std::shared_ptr<OperationChain> chain, std::filesystem::path path, libv::hash::md5 hash) {
		removes.emplace_back(std::move(chain), std::move(path), hash);
	}

	inline void remove(std::shared_ptr<OperationChain> chain, std::filesystem::path path) {
		removes.emplace_back(std::move(chain), std::move(path), std::nullopt);
	}
};

// -------------------------------------------------------------------------------------------------

void plan(const StepCreateDir& step, const std::filesystem::path& root_dir, ExecutionPlan& plan, bool fast_plan) {
	const auto path = root_dir / step.path;

	if (!fast_plan && file_exists(path))
		return;

	plan.create_dir(path);
}

void plan(const StepRemoveDir& step, const std::filesystem::path& root_dir, ExecutionPlan& plan, bool fast_plan) {
	const auto path = root_dir / step.path;

	if (!fast_plan && !file_exists(path))
		return;

	plan.remove_dir(path);
}

void plan(const StepCreate& step, const std::filesystem::path& root_dir, ExecutionPlan& plan, bool fast_plan) {
	const auto chain = std::make_shared<OperationChain>();

	const auto path_live = root_dir / step.filepath;
	const auto path_new_ = path_concat(path_live, ".new");

	if (!fast_plan) {
		const auto[new_exists, new_is_new] = file_exists_hash(path_new_, step.hash_new);
		const auto[live_exists, live_is_new] = file_exists_hash(path_live, step.hash_new);

		if (live_is_new) {
			log_update.trace("Live file matches expected hash, fast forwarding creation of {}", path_live.generic_string());
			if (new_exists)
				plan.remove(chain, path_new_);
			return;
		}

		if (new_is_new) {
			log_update.trace("New file matches expected hash, fast forwarding creation of {}", path_new_.generic_string());
			plan.rename_2(chain, path_new_, path_live, step.hash_new);
			return;
		}
	}

	plan.create(chain, path_new_, &step.data, step.hash_new);
	plan.rename_2(chain, path_new_, path_live, step.hash_new);
}

void plan(const StepModify& step, const std::filesystem::path& root_dir, ExecutionPlan& plan, bool fast_plan) {
	const auto chain = std::make_shared<OperationChain>();

	const auto path_live = root_dir / step.filepath;
	const auto path_old_ = path_concat(path_live, ".old");
	const auto path_new_ = path_concat(path_live, ".new");

	if (!fast_plan) {
		const auto[live_exists, live_is_new] = file_exists_hash(path_live, step.hash_new);
		const auto[new_exists, new_is_new] = file_exists_hash(path_new_, step.hash_new);
		const auto old_exists = file_exists(path_old_);

		if (live_is_new) {
			log_update.trace("Live file matches expected hash, fast forwarding modification of {}", path_live.generic_string());
			if (new_exists)
				plan.remove(chain, path_new_);
			if (old_exists)
				plan.remove(chain, path_old_);
			return;
		}

		if (new_is_new) {
			log_update.trace("New file matches expected hash, fast forwarding modification of {}", path_new_.generic_string());
			if (live_exists) {
				plan.rename_1(chain, path_live, path_old_, step.hash_old);
			}
			plan.rename_2(chain, path_new_, path_live, step.hash_new);
			plan.remove(chain, path_old_, step.hash_old);
			return;
		}
	}

	plan.modify(chain, path_live, path_new_, &step.diff, step.hash_old, step.hash_new);
	plan.rename_1(chain, path_live, path_old_, step.hash_old);
	plan.rename_2(chain, path_new_, path_live, step.hash_new);
	plan.remove(chain, path_old_, step.hash_old);
}

void plan(const StepModifyTo& step, const std::filesystem::path& root_dir, ExecutionPlan& plan, bool fast_plan) {
	const auto chain = std::make_shared<OperationChain>();

	const auto path_origin = root_dir / step.filepath_old;
	const auto path_live = root_dir / step.filepath_new;
	const auto path_new_ = path_concat(path_live, ".new");

	if (!fast_plan) {
		const auto[live_exists, live_is_new] = file_exists_hash(path_live, step.hash_new);
		const auto[new_exists, new_is_new] = file_exists_hash(path_new_, step.hash_new);

		if (live_is_new) {
			log_update.trace("Live file matches expected hash, fast forwarding modification of {}", path_live.generic_string());
			if (new_exists)
				plan.remove(chain, path_new_);
			return;
		}

		if (new_is_new) {
			log_update.trace("New file matches expected hash, fast forwarding modification of {}", path_new_.generic_string());
			plan.rename_2(chain, path_new_, path_live, step.hash_new);
			return;
		}
	}

	plan.modify(chain, path_origin, path_new_, &step.diff, step.hash_old, step.hash_new);
	plan.rename_2(chain, path_new_, path_live, step.hash_new);
}

void plan(const StepRename& step, const std::filesystem::path& root_dir, ExecutionPlan& plan, bool fast_plan) {
	const auto chain = std::make_shared<OperationChain>();

	const auto path_origin = root_dir / step.filepath_old;
	const auto path_live = root_dir / step.filepath_new;
	const auto path_new_ = path_concat(path_live, ".new");

	if (!fast_plan) {
		const auto[live_exists, live_is_new] = file_exists_hash(path_live, step.hash_new);
		const auto[new_exists, new_is_new] = file_exists_hash(path_new_, step.hash_new);

		if (live_is_new) {
			log_update.trace("Live file matches expected hash, fast forwarding rename of {}", path_live.generic_string());
			if (new_exists)
				plan.remove(chain, path_new_);
			return;
		}

		if (new_is_new) {
			plan.rename_2(chain, path_new_, path_live, step.hash_new);
			return;
		}
	}

	plan.rename_1(chain, path_origin, path_new_, step.hash_new);
	plan.rename_2(chain, path_new_, path_live, step.hash_new);
}

void plan(const StepRemove& step, const std::filesystem::path& root_dir, ExecutionPlan& plan, bool fast_plan) {
	const auto chain = std::make_shared<OperationChain>();

	const auto path_live = root_dir / step.filepath;
	const auto path_old_ = path_concat(path_live, ".old");

	if (!fast_plan) {
		const auto old_exists = file_exists(path_old_);

		if (old_exists) {
			log_update.trace("Old file exists, fast forwarding rename of {}", path_live.generic_string());
			plan.remove(chain, path_old_, step.hash_old);
			plan.remove(chain, path_live);
			return;
		}
	}

	plan.rename_1(chain, path_live, path_old_, step.hash_old);
	plan.remove(chain, path_old_, step.hash_old);
}

// -------------------------------------------------------------------------------------------------

static constexpr size_t progress_weight_io_write_byte = 1;
static constexpr size_t progress_weight_io_read_byte  = 1;

static constexpr size_t progress_weight_io_input      =  8 * 1024;
static constexpr size_t progress_weight_io_output     =  8 * 1024;
static constexpr size_t progress_weight_io_remove     =  8 * 1024;
static constexpr size_t progress_weight_io_rename     =  8 * 1024;

static constexpr size_t progress_weight_create_dir    = 32 * 1024;
static constexpr size_t progress_weight_remove_dir    = 32 * 1024;

static constexpr size_t progress_weight_step          = 16 * 1024;

size_t progress_weight(const StepCreateDir&) {
	return progress_weight_create_dir;
}
size_t progress_weight(const StepRemoveDir&) {
	return progress_weight_remove_dir;
}
size_t progress_weight(const StepCreate& step) {
	return
			progress_weight_io_output +
			progress_weight_io_write_byte * step.data.size() +
			progress_weight_io_rename;
}
size_t progress_weight(const StepModify& step) {
	const auto diff_info = libv::diff::get_diff_info(step.diff);
	return
			progress_weight_io_output +
			progress_weight_io_input +
			progress_weight_io_read_byte * diff_info.old_size +
			progress_weight_io_write_byte * diff_info.new_size +
			progress_weight_io_rename * 2 +
			progress_weight_io_remove;
}
size_t progress_weight(const StepModifyTo& step) {
	const auto diff_info = libv::diff::get_diff_info(step.diff);
	return
			progress_weight_io_output +
			progress_weight_io_input +
			progress_weight_io_read_byte * diff_info.old_size +
			progress_weight_io_write_byte * diff_info.new_size +
			progress_weight_io_rename;
}
size_t progress_weight(const StepRename&) {
	return progress_weight_io_rename * 2;
}
size_t progress_weight(const StepRemove&) {
	return
			progress_weight_io_rename +
			progress_weight_io_remove;
}

//// -------------------------------------------------------------------------------------------------
//
//size_t num_step(const Patch& patch) {
//	return
//			patch.creates_dir.size() +
//			patch.removes_dir.size() +
//
//			patch.creates.size() +
//			patch.modifies.size() +
//			patch.modifies_to.size() +
//			patch.renames.size() +
//			patch.removes.size();
//}
//
//template <typename F>
//void call_step(const Patch& patch, size_t i, F&& func) {
//	if (i < patch.creates_dir.size())
//		return func(patch.creates_dir[i]);
//	i -= patch.creates_dir.size();
//
//	if (i < patch.removes_dir.size())
//		return func(patch.removes_dir[i]);
//	i -= patch.removes_dir.size();
//
//	if (i < patch.creates.size())
//		return func(patch.creates[i]);
//	i -= patch.creates.size();
//
//	if (i < patch.modifies.size())
//		return func(patch.modifies[i]);
//	i -= patch.modifies.size();
//
//	if (i < patch.modifies_to.size())
//		return func(patch.modifies_to[i]);
//	i -= patch.modifies_to.size();
//
//	if (i < patch.renames.size())
//		return func(patch.renames[i]);
//	i -= patch.renames.size();
//
//	if (i < patch.removes.size())
//		return func(patch.removes[i]);
//}

} // namespace -------------------------------------------------------------------------------------

struct ImplPatchApplier {
	std::filesystem::path root_dir;
	std::shared_ptr<const Patch> patch;

public:
	bool fast_plan;
	ExecutionPlan plan;

public:
//	size_t num_steps = 0;
//
//	size_t work_current = 0;
//	size_t work_total = 0;

	size_t progress_current = 0;
	size_t progress_total = 0;

public:
	enum class progress_stage {
		plan_creates_dir,
		plan_removes_dir,
		plan_creates,
		plan_modifies,
		plan_modifies_to,
		plan_renames,
		plan_removes,
		execute_creates_dir,
		execute_creates,
		execute_modifies,
		execute_rename_pass1,
		execute_rename_pass2,
		execute_removes,
		execute_removes_dirs,
		done,
	};

	progress_stage current_stage = progress_stage::plan_creates_dir;
	size_t current_stage_it = 0;

public:
	std::vector<PatchApplyFailure> failures;
};

// -------------------------------------------------------------------------------------------------

PatchApplier::PatchApplier(std::filesystem::path root_dir, std::shared_ptr<const Patch> patch, bool continue_) :
	self(std::make_unique<ImplPatchApplier>()) {
	self->root_dir = std::move(root_dir);
	self->patch = std::move(patch);
	self->fast_plan = !continue_;
	init();
}

PatchApplier::~PatchApplier() = default; // For the sake of forward declared unique_ptr

void PatchApplier::init() {
//	self->num_steps = num_step(*self->patch);
//
//	self->progress_total = 0;
//	self->progress_total += self->num_steps * progress_weight_step;
//	for (size_t i = 0; i < self->num_steps; i++)
//		call_step(*self->patch, i, [this](const auto& step) {
//			self->progress_total += progress_weight(step);
//		});
}

bool PatchApplier::progress() {
	const auto plan_stage = [this](const auto& step_container, const auto next_stage) {
		if (self->current_stage_it < step_container.size()) {
			const auto& step = step_container[self->current_stage_it];
			plan(step, self->root_dir, self->plan, self->fast_plan);
//			self->progress_current += progress_weight(step);
			self->current_stage_it++;
			return true;
		} else {
			self->current_stage = next_stage;
			self->current_stage_it = 0;
			return false;
		}
	};

	const auto execute_stage = [this](const auto& operation_container, const auto next_stage) {
		const auto failure_cb = [this](std::string filepath, std::error_code ec) {
			self->failures.emplace_back(std::move(filepath), ec);
		};

		if (self->current_stage_it < operation_container.size()) {
			const auto& op = operation_container[self->current_stage_it];
			execute(op, failure_cb);
//			self->progress_current += progress_weight(op);
			self->current_stage_it++;
			return true;
		} else {
			self->current_stage = next_stage;
			self->current_stage_it = 0;
			return false;
		}
	};

	const auto& patch = *self->patch;
	const auto& plan = self->plan;
	using progress_stage = ImplPatchApplier::progress_stage;

	switch (self->current_stage) {
		case progress_stage::plan_creates_dir:
			if (plan_stage(patch.creates_dir, progress_stage::plan_removes_dir))
				return true;
			[[fallthrough]];
		case progress_stage::plan_removes_dir:
			if (plan_stage(patch.removes_dir, progress_stage::plan_creates))
				return true;
			[[fallthrough]];
		case progress_stage::plan_creates:
			if (plan_stage(patch.creates, progress_stage::plan_modifies))
				return true;
			[[fallthrough]];
		case progress_stage::plan_modifies:
			if (plan_stage(patch.modifies, progress_stage::plan_modifies_to))
				return true;
			[[fallthrough]];
		case progress_stage::plan_modifies_to:
			if (plan_stage(patch.modifies_to, progress_stage::plan_renames))
				return true;
			[[fallthrough]];
		case progress_stage::plan_renames:
			if (plan_stage(patch.renames, progress_stage::plan_removes))
				return true;
			[[fallthrough]];
		case progress_stage::plan_removes:
			if (plan_stage(patch.removes, progress_stage::execute_creates_dir))
				return true;
			[[fallthrough]];

		case progress_stage::execute_creates_dir:
			if (execute_stage(plan.create_dir_, progress_stage::execute_creates))
				return true;
			[[fallthrough]];
		case progress_stage::execute_creates:
			if (execute_stage(plan.creates, progress_stage::execute_modifies))
				return true;
			[[fallthrough]];
		case progress_stage::execute_modifies:
			if (execute_stage(plan.modifies, progress_stage::execute_rename_pass1))
				return true;
			[[fallthrough]];
		case progress_stage::execute_rename_pass1:
			if (execute_stage(plan.rename_pass1, progress_stage::execute_rename_pass2))
				return true;
			[[fallthrough]];
		case progress_stage::execute_rename_pass2:
			if (execute_stage(plan.rename_pass2, progress_stage::execute_removes))
				return true;
			[[fallthrough]];
		case progress_stage::execute_removes:
			if (execute_stage(plan.removes, progress_stage::execute_removes_dirs))
				return true;
			[[fallthrough]];
		case progress_stage::execute_removes_dirs:
			if (execute_stage(plan.remove_dir_, progress_stage::done))
				return true;
			[[fallthrough]];

		case progress_stage::done:
			// Done
			[[fallthrough]];
	}

	return false;
}

std::span<const PatchApplyFailure> PatchApplier::failures() const noexcept {
	return self->failures;
}

size_t PatchApplier::progress_current() const noexcept {
	return self->progress_current;
}

size_t PatchApplier::progress_total() const noexcept {
	return self->progress_total;
}

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
