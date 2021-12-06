// Project: libv.img, File: src/libv/img/save_result.hpp

#pragma once

// std
#include <memory>
#include <span>


namespace libv {
namespace img {

// -------------------------------------------------------------------------------------------------

struct formated_image_data_view {
	const std::byte* data;
	std::size_t size;
};

class [[nodiscard]] save_result {
private:
	std::shared_ptr<const formated_image_data_view> view;

public:
	explicit inline save_result(std::shared_ptr<const formated_image_data_view> view) noexcept : view(std::move(view)) {}

public:
	[[nodiscard]] inline const std::byte* data() const noexcept {
		return view->data;
	}

	[[nodiscard]] inline std::size_t size() const noexcept {
		return view->size;
	}

	[[nodiscard]] inline bool operator!() const noexcept {
		return view->data == nullptr;
	}

	[[nodiscard]] inline bool success() const noexcept {
		return view->data != nullptr;
	}

public:
	[[nodiscard]] inline std::span<const std::byte> span() const noexcept {
		return std::span<const std::byte>(data(), size());
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace img
} // namespace libv
