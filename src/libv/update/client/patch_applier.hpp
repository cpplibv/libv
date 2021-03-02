// Project: libv.update, File: src/libv/update/client/patch_applier.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <filesystem>
#include <memory>
#include <span>
#include <string>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

struct PatchApplyFailure {
	std::string filepath;
	std::error_code reason;
};

// -------------------------------------------------------------------------------------------------

struct PatchApplier {
	std::unique_ptr<class ImplPatchApplier> self;

public:
	PatchApplier(std::filesystem::path root_dir, std::shared_ptr<const class Patch> patch, bool continue_);
	virtual ~PatchApplier();

public:
	/// Repeat until its returns false, errors are reported with the failures() function
	bool progress();

public:
	[[nodiscard]] int64_t progress_current() const noexcept;
	[[nodiscard]] int64_t progress_total() const noexcept;

public:
	[[nodiscard]] std::span<const PatchApplyFailure> failures() const noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
