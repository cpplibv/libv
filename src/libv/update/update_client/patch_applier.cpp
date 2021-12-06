// Project: libv.update, File: src/libv/update/update_client/patch_applier.cpp

// hpp
#include <libv/update/update_client/patch_applier.hpp>
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
#include <libv/update/log.hpp>
#include <libv/update/patch.hpp>


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

static constexpr int64_t progress_weight_io_operation  = 8 * 1024;
static constexpr int64_t progress_weight_io_read_byte  = 1;
static constexpr int64_t progress_weight_io_write_byte = 1;
static constexpr int64_t progress_weight_step_plan     = 4 * 1024;

[[nodiscard]] static constexpr int64_t pw_iorw(std::size_t num_io, std::size_t num_read_byte, std::size_t num_write_byte) noexcept {
	return progress_weight_io_operation * static_cast<int64_t>(num_io) +
			progress_weight_io_read_byte * static_cast<int64_t>(num_read_byte) +
			progress_weight_io_write_byte * static_cast<int64_t>(num_write_byte);
}

int64_t progress_cost(const StepCreateDir&) { return pw_iorw(1, 0, 0) + progress_weight_step_plan; }
int64_t progress_cost(const StepRemoveDir&) { return pw_iorw(1, 0, 0) + progress_weight_step_plan; }
int64_t progress_cost(const StepCreate& step) { return pw_iorw(2, 0, step.data.size()) + progress_weight_step_plan; }
int64_t progress_cost(const StepModify& step) {
	const auto diff_info = libv::diff::get_diff_info(step.diff);
	return pw_iorw(5, diff_info.old_size, diff_info.new_size) + progress_weight_step_plan;
}
int64_t progress_cost(const StepModifyAs& step) {
	const auto diff_info = libv::diff::get_diff_info(step.diff);
	return pw_iorw(3, diff_info.old_size, diff_info.new_size) + progress_weight_step_plan;
}
int64_t progress_cost(const StepRename&) { return pw_iorw(2, 0, 0) + progress_weight_step_plan; }
int64_t progress_cost(const StepRemove&) { return pw_iorw(2, 0, 0) + progress_weight_step_plan; }

// Costs of steps and operations has to kept in sync!

int64_t progress_cost(const OperationCreateDir&) { return pw_iorw(1, 0, 0); }
int64_t progress_cost(const OperationRemoveDir&) { return pw_iorw(1, 0, 0); }
int64_t progress_cost(const OperationCreate& op) { return pw_iorw(1, 0, op.data->size()); }
int64_t progress_cost(const OperationModify& op) {
	const auto diff_info = libv::diff::get_diff_info(*op.diff);
	return pw_iorw(2, diff_info.old_size, diff_info.new_size);
}
int64_t progress_cost(const OperationRename&) { return pw_iorw(1, 0, 0); }
int64_t progress_cost(const OperationRemove&) { return pw_iorw(1, 0, 0); }

// -------------------------------------------------------------------------------------------------

struct ExecutionPlan {
	std::vector<OperationCreateDir> create_dir_;
	std::vector<OperationCreate> creates;
	std::vector<OperationModify> modifies;
	std::vector<OperationRename> rename_pass1;
	std::vector<OperationRename> rename_pass2;
	std::vector<OperationRemove> removes;
	std::vector<OperationRemoveDir> remove_dir_;

	int64_t last_steps_operation_cost = 0;

public:
	inline void create_dir(std::filesystem::path path) {
		create_dir_.emplace_back(std::move(path));
		last_steps_operation_cost += progress_cost(create_dir_.back());
	}

	inline void remove_dir(std::filesystem::path path) {
		remove_dir_.emplace_back(std::move(path));
		last_steps_operation_cost += progress_cost(remove_dir_.back());
	}

	inline void create(std::shared_ptr<OperationChain> chain, std::filesystem::path path, const file_full_data* data, libv::hash::md5 hash) {
		creates.emplace_back(std::move(chain), std::move(path), data, hash);
		last_steps_operation_cost += progress_cost(creates.back());
	}

	inline void modify(std::shared_ptr<OperationChain> chain, std::filesystem::path path_old, std::filesystem::path path_new, const file_diff_data* data, libv::hash::md5 hash_old, libv::hash::md5 hash_new) {
		modifies.emplace_back(std::move(chain), std::move(path_old), std::move(path_new), data, hash_old, hash_new);
		last_steps_operation_cost += progress_cost(modifies.back());
	}

	inline void rename_1(std::shared_ptr<OperationChain> chain, std::filesystem::path path_old, std::filesystem::path path_new, libv::hash::md5 hash) {
		rename_pass1.emplace_back(std::move(chain), std::move(path_old), std::move(path_new), hash);
		last_steps_operation_cost += progress_cost(rename_pass1.back());
	}

	inline void rename_2(std::shared_ptr<OperationChain> chain, std::filesystem::path path_old, std::filesystem::path path_new, libv::hash::md5 hash) {
		rename_pass2.emplace_back(std::move(chain), std::move(path_old), std::move(path_new), hash);
		last_steps_operation_cost += progress_cost(rename_pass2.back());
	}

	inline void remove(std::shared_ptr<OperationChain> chain, std::filesystem::path path, libv::hash::md5 hash) {
		removes.emplace_back(std::move(chain), std::move(path), hash);
		last_steps_operation_cost += progress_cost(removes.back());
	}

	inline void remove(std::shared_ptr<OperationChain> chain, std::filesystem::path path) {
		removes.emplace_back(std::move(chain), std::move(path), std::nullopt);
		last_steps_operation_cost += progress_cost(removes.back());
	}
};

// -------------------------------------------------------------------------------------------------

template <typename Step>
[[nodiscard]] inline int64_t plan_step(const Step& step, const std::filesystem::path& root_dir, ExecutionPlan& plan, bool fast_plan) {
	plan.last_steps_operation_cost = 0;
	do_plan_step(step, root_dir, plan, fast_plan);
	return progress_cost(step) - plan.last_steps_operation_cost;
}

void do_plan_step(const StepCreateDir& step, const std::filesystem::path& root_dir, ExecutionPlan& plan, bool fast_plan) {
	const auto path = root_dir / step.path;

	if (!fast_plan && file_exists(path))
		return;

	plan.create_dir(path);
}

void do_plan_step(const StepRemoveDir& step, const std::filesystem::path& root_dir, ExecutionPlan& plan, bool fast_plan) {
	const auto path = root_dir / step.path;

	if (!fast_plan && !file_exists(path))
		return;

	plan.remove_dir(path);
}

void do_plan_step(const StepCreate& step, const std::filesystem::path& root_dir, ExecutionPlan& plan, bool fast_plan) {
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

void do_plan_step(const StepModify& step, const std::filesystem::path& root_dir, ExecutionPlan& plan, bool fast_plan) {
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

void do_plan_step(const StepModifyAs& step, const std::filesystem::path& root_dir, ExecutionPlan& plan, bool fast_plan) {
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

void do_plan_step(const StepRename& step, const std::filesystem::path& root_dir, ExecutionPlan& plan, bool fast_plan) {
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

void do_plan_step(const StepRemove& step, const std::filesystem::path& root_dir, ExecutionPlan& plan, bool fast_plan) {
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

template <typename F>
void foreach_step(const Patch& patch, F&& func) {
	for (const auto& step : patch.creates_dir) func(step);
	for (const auto& step : patch.removes_dir) func(step);
	for (const auto& step : patch.creates) func(step);
	for (const auto& step : patch.modifies) func(step);
	for (const auto& step : patch.modifies_as) func(step);
	for (const auto& step : patch.renames) func(step);
	for (const auto& step : patch.removes) func(step);
}

} // namespace -------------------------------------------------------------------------------------

struct ImplPatchApplier {
	std::filesystem::path root_dir;
	std::shared_ptr<const Patch> patch;

public:
	bool fast_plan;
	ExecutionPlan plan;

public:
//	std::size_t num_steps = 0;
//
//	std::size_t work_current = 0;
//	std::size_t work_total = 0;

	int64_t progress_current = 0;
	int64_t progress_total = 0;

public:
	enum class progress_stage {
		init,
		plan_creates_dir,
		plan_removes_dir,
		plan_creates,
		plan_modifies,
		plan_modifies_as,
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

	progress_stage current_stage = progress_stage::init;
	std::size_t current_stage_it = 0;

public:
	std::vector<PatchApplyFailure> failures;
};

// -------------------------------------------------------------------------------------------------

PatchApplier::PatchApplier(std::filesystem::path root_dir, std::shared_ptr<const Patch> patch, bool continue_) :
	self(std::make_unique<ImplPatchApplier>()) {
	self->root_dir = std::move(root_dir);
	self->patch = std::move(patch);
	self->fast_plan = !continue_;

	foreach_step(*self->patch, [this](const auto& step) {
		self->progress_total += progress_cost(step);
	});
}

PatchApplier::~PatchApplier() = default; // For the sake of forward declared unique_ptr

bool PatchApplier::progress() {
	const auto& patch = *self->patch;
	const auto& plan = self->plan;
	using progress_stage = ImplPatchApplier::progress_stage;

	const auto plan_stage = [this](const auto& step_container, const auto next_stage) {
		if (self->current_stage_it < step_container.size()) {
			const auto& step = step_container[self->current_stage_it];
			const auto fast_forwarded_progress = plan_step(step, self->root_dir, self->plan, self->fast_plan);
			self->progress_current += fast_forwarded_progress;
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
			self->progress_current += progress_cost(op);
			self->current_stage_it++;
			return true;
		} else {
			self->current_stage = next_stage;
			self->current_stage_it = 0;
			return false;
		}
	};

	switch (self->current_stage) {
		case progress_stage::init:
			self->current_stage = progress_stage::plan_creates_dir;
			[[fallthrough]];

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
			if (plan_stage(patch.modifies, progress_stage::plan_modifies_as))
				return true;
			[[fallthrough]];
		case progress_stage::plan_modifies_as:
			if (plan_stage(patch.modifies_as, progress_stage::plan_renames))
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
			{ } // Done
	}

	return false;
}

std::span<const PatchApplyFailure> PatchApplier::failures() const noexcept {
	return self->failures;
}

int64_t PatchApplier::progress_current() const noexcept {
	return self->progress_current;
}

int64_t PatchApplier::progress_total() const noexcept {
	return self->progress_total;
}

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
