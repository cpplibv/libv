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
#include <update/patch/patch.hpp>
#include <update/common/log.hpp>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

namespace { // -------------------------------------------------------------------------------------

[[nodiscard]] std::filesystem::path path_concat(const std::filesystem::path& path, const std::string_view str) {
	std::filesystem::path result = path;
	result += str;
	return result;
}

[[nodiscard]] bool file_exists(const std::filesystem::path& path) {
	std::error_code ec;
	const auto result = std::filesystem::exists(path, ec);
	app::log_app.error_if(ec, "Failed to check file existence for: {} - {}:{} {}", path, ec.category().name(), ec.value(), ec.message());
	return result;
}

[[nodiscard]] bool file_hash(const std::filesystem::path& path, const libv::hash::md5& hash) {
	std::ifstream file(path, std::ios::binary | std::ios::in);
	if (!file) {
		const auto ec = std::make_error_code(static_cast<std::errc>(errno));
		app::log_app.error_if(ec, "Failed to calculate file hash: {} - {}:{} {}", path, ec.category().name(), ec.value(), ec.message());
		return false;
	}

	const auto actual_hash = libv::hash::hash_md5(file);

	if (file.fail()) {
		const auto ec = std::make_error_code(static_cast<std::errc>(errno));
		app::log_app.error_if(ec, "Failed to calculate file hash: {} - {}:{} {}", path, ec.category().name(), ec.value(), ec.message());
		return false;
	}

	return actual_hash == hash;
}

[[nodiscard]] auto file_exists_hash(const std::filesystem::path& path, const libv::hash::md5& hash) {
	struct Result {
		bool exists = false;
		bool matches = false;
	} result;

	std::error_code ec;
	result.exists = std::filesystem::exists(path, ec);
	app::log_app.error_if(ec, "Failed to check file existence for: {} - {}:{} {}", path, ec.category().name(), ec.value(), ec.message());

	if (ec || !result.exists)
		return result;

	result.matches = file_hash(path, hash);
	return result;
}

// -------------------------------------------------------------------------------------------------

//[[nodiscard]] static std::ifstream file_input(const std::filesystem::path& path) {
//	return std::ifstream(path, std::ios::binary | std::ios::in);
//}
//
//[[nodiscard]] static std::ofstream file_output(const std::filesystem::path& path) {
//	return std::ofstream(path, std::ios::binary | std::ios::out | std::ios::trunc);
//}
//
//[[nodiscard]] static std::error_code file_remove(const std::filesystem::path& path) {
//	std::error_code ec;
//	std::filesystem::remove(path, ec);
//	return ec;
//}

//[[nodiscard]] static std::error_code file_remove_if_exists(const std::filesystem::path& path) {
//	std::error_code ec;
//	const auto exists = std::filesystem::exists(path, ec);
//	if (ec)
//		return ec;
//
//	if (!exists)
//		return ec;
//
//	std::filesystem::remove(path, ec);
//	return ec;
//}
//
//[[nodiscard]] static std::error_code file_rename(const std::filesystem::path& path_old, const std::filesystem::path& path_new) {
//	std::error_code ec;
//	std::filesystem::remove(path_new, ec);
//	if (ec)
//		return ec;
//
//	std::filesystem::rename(path_old, path_new, ec);
//	return ec;
//}

// -------------------------------------------------------------------------------------------------

using failure_cb_ref_t = libv::function_ref<void(std::string, std::error_code)>;

// ---

struct OperationChain {
	bool aborted = false;
};

// ---

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
void execute(Op& op, const failure_cb_ref_t& failure_cb) {
	const auto continue_ = do_execute(op, op.chain->aborted, failure_cb);
	op.chain->aborted &= !continue_;
}

[[nodiscard]] bool do_execute(OperationCreate& op, bool aborted, const failure_cb_ref_t& failure_cb) {
	if (aborted)
		return false;

	auto ec = libv::write_file_ec(op.path, *op.data);
	if (ec)
		failure_cb(op.path.generic_string(), ec);

	return !ec;
}

[[nodiscard]] bool do_execute(OperationModify& op, bool aborted, const failure_cb_ref_t& failure_cb) {
	if (aborted)
		return false;

	auto file_old = std::ifstream(op.path_old, std::ios::binary | std::ios::in);
	if (!file_old)
		failure_cb(op.path_old.generic_string(), std::make_error_code(static_cast<std::errc>(errno)));

	auto file_new = std::ofstream(op.path_new, std::ios::binary | std::ios::out | std::ios::trunc);
	if (!file_new)
		failure_cb(op.path_new.generic_string(), std::make_error_code(static_cast<std::errc>(errno)));

	if (!file_old || !file_new)
		return false;

	// <<< TMP DIFF STREAM MEMORY MIXED API
//		return libv::diff::apply_patch(file_old, *diff, file_new);
	std::stringstream diff_ss(std::string(reinterpret_cast<const char*>(op.diff->data()), op.diff->size()));
	const auto success = libv::diff::apply_patch(file_old, diff_ss, file_new);

	if (!success) {
		if (file_hash(op.path_old, op.hash_old))
			failure_cb(op.path_old.generic_string(), std::make_error_code(std::errc::state_not_recoverable));
		else
			failure_cb(op.path_old.generic_string(), std::make_error_code(std::errc::illegal_byte_sequence));
	}

	return success;
}

[[nodiscard]] bool do_execute(OperationRename& op, bool aborted, const failure_cb_ref_t& failure_cb) {
	if (aborted)
		return false;

	std::error_code ec;
	std::filesystem::rename(op.path_old, op.path_new, ec);
	if (ec)
		failure_cb(op.path_old.generic_string(), ec);

	return !ec;
}

[[nodiscard]] bool do_execute(OperationRemove& op, bool aborted, const failure_cb_ref_t& failure_cb) {
	if (aborted)
		return false;

	std::error_code ec;
	std::filesystem::remove(op.path, ec);
	if (ec)
		failure_cb(op.path.generic_string(), ec);

	return !ec;
}

// -------------------------------------------------------------------------------------------------

struct ExecutionPlan {
	using OperationCreateOrModify = std::variant<OperationCreate, OperationModify>;

	std::vector<OperationCreateOrModify> prepare;
	std::vector<OperationRename> apply_1;
	std::vector<OperationRename> apply_2;
	std::vector<OperationRemove> cleanup;

public:
	void create(std::shared_ptr<OperationChain> chain, std::filesystem::path path, const file_full_data* data, libv::hash::md5 hash) {
		prepare.emplace_back(std::in_place_type<OperationCreate>, std::move(chain), std::move(path), data, hash);
	}

	void modify(std::shared_ptr<OperationChain> chain, std::filesystem::path path_old, std::filesystem::path path_new, const file_diff_data* data, libv::hash::md5 hash_old, libv::hash::md5 hash_new) {
		prepare.emplace_back(std::in_place_type<OperationModify>, std::move(chain), std::move(path_old), std::move(path_new), data, hash_old, hash_new);
	}

	void rename_1(std::shared_ptr<OperationChain> chain, std::filesystem::path path_old, std::filesystem::path path_new, libv::hash::md5 hash) {
		apply_1.emplace_back(std::move(chain), std::move(path_old), std::move(path_new), hash);
	}

	void rename_2(std::shared_ptr<OperationChain> chain, std::filesystem::path path_old, std::filesystem::path path_new, libv::hash::md5 hash) {
		apply_2.emplace_back(std::move(chain), std::move(path_old), std::move(path_new), hash);
	}

	void remove(std::shared_ptr<OperationChain> chain, std::filesystem::path path, libv::hash::md5 hash) {
		cleanup.emplace_back(std::move(chain), std::move(path), hash);
	}

	void remove(std::shared_ptr<OperationChain> chain, std::filesystem::path path) {
		cleanup.emplace_back(std::move(chain), std::move(path), std::nullopt);
	}
};

// -------------------------------------------------------------------------------------------------

void plan(const StepCreate& step, const std::filesystem::path& root_dir, ExecutionPlan& plan, bool fast_plan) {
	const auto chain = std::make_shared<OperationChain>();

	const auto path_live = root_dir / step.filepath;
	const auto path_new_ = path_concat(path_live, ".new");

	if (!fast_plan) {
		const auto[new_exists, new_is_new] = file_exists_hash(path_new_, step.hash_new);
		const auto[live_exists, live_is_new] = file_exists_hash(path_live, step.hash_new);

		if (live_is_new) {
			if (new_exists)
				plan.remove(chain, path_new_);
			return;
		}

		if (new_is_new) {
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
			if (new_exists)
				plan.remove(chain, path_new_);
			if (old_exists)
				plan.remove(chain, path_old_);
			return;
		}

		if (new_is_new) {
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
			if (new_exists)
				plan.remove(chain, path_new_);
			return;
		}

		if (new_is_new) {
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
			plan.remove(chain, path_old_, step.hash_old);
			return;
		}
	}

	plan.rename_1(chain, path_live, path_old_, step.hash_old);
	plan.remove(chain, path_old_, step.hash_old);
}

} // namespace -------------------------------------------------------------------------------------

struct ImplPatchApplier {
	std::filesystem::path root_dir;
	std::shared_ptr<Patch> patch;

public:
	ExecutionPlan plan;

public:
	size_t progress_current;
	size_t progress_total;

public:
	std::vector<PatchApplyFailure> failures; // <<< P4: expose failures
};

// -------------------------------------------------------------------------------------------------

PatchApplier::PatchApplier(std::filesystem::path root_dir, std::shared_ptr<Patch> patch) :
	self(std::make_unique<ImplPatchApplier>()) {
	self->root_dir = std::move(root_dir);
	self->patch = std::move(patch);
}

PatchApplier::~PatchApplier() = default; // For the sake of forward declared unique_ptr

void PatchApplier::progress(bool fast_plan) {
	const auto failure_cb = [this](std::string filepath, std::error_code ec) {
		self->failures.emplace_back(std::move(filepath), ec);
	};

	// Prepare

	for (const auto& step : self->patch->creates)
		plan(step, self->root_dir, self->plan, fast_plan);
	for (const auto& step : self->patch->modifies)
		plan(step, self->root_dir, self->plan, fast_plan);
	for (const auto& step : self->patch->modifiesTo)
		plan(step, self->root_dir, self->plan, fast_plan);
	for (const auto& step : self->patch->renames)
		plan(step, self->root_dir, self->plan, fast_plan);
	for (const auto& step : self->patch->removes)
		plan(step, self->root_dir, self->plan, fast_plan);

	// Apply

	for (auto& opv : self->plan.prepare)
		std::visit([&](auto& op) { execute(op, failure_cb); }, opv);
	for (auto& op : self->plan.apply_1)
		execute(op, failure_cb);
	for (auto& op : self->plan.apply_2)
		execute(op, failure_cb);
	for (auto& op : self->plan.cleanup)
		execute(op, failure_cb);
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
