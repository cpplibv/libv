// Project: libv, File: app/update/patch/patch_applier.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <filesystem>
#include <memory>
#include <span>
#include <string>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

// */**
// */**.new
// */**.old
// patch/v0001_v0000.vup
// patch/*.vup
// .lock
// .patch
// program
//
//	update outcomes:
//		failure - operation failure
//			attempt to retry and continue
//			rollback -> do nothing
//			rollback -> fix corruption -> do nothing
//			rollback -> fix corruption -> update again
//			force apply -> fix corruption
//		failure - corruption
//			rollback -> do nothing
//			rollback -> fix corruption -> do nothing
//			rollback -> fix corruption -> update again
//			force apply -> fix corruption
//		success

// -------------------------------------------------------------------------------------------------

struct PatchApplyFailure {
//		enum class operation : int {
//			create,
//			modify,
//			modify_to,
//			rename,
//			remove,
//		};

	std::string filepath;
//		operation operation;
	std::error_code reason;
};

// -------------------------------------------------------------------------------------------------

struct PatchApplier {
	std::unique_ptr<class ImplPatchApplier> self;

public:
	PatchApplier(std::filesystem::path root_dir, std::shared_ptr<const class Patch> patch, bool continue_);
	virtual ~PatchApplier();

private:
	void init();

public:
	// start
	// stop

//	void progress(size_t min_amount);
	bool progress();

public:
	[[nodiscard]] std::span<const PatchApplyFailure> failures() const noexcept;

public:
	[[nodiscard]] size_t progress_current() const noexcept;
	[[nodiscard]] size_t progress_total() const noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
