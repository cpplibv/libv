// Project: libv.range, File: src/libv/range/view_glob_generic_path.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/algo/wildcard.hpp>
// ext
#include <range/v3/functional/bind_back.hpp>
#include <range/v3/range/concepts.hpp>
#include <range/v3/view/all.hpp>
#include <range/v3/view/facade.hpp>
// std
#include <filesystem>
#include <string_view>
#include <ranges>


namespace libv {
namespace view {

// -------------------------------------------------------------------------------------------------

namespace detail {

struct glob_generic_path_t {
	struct iterator {
		using iterator_category = std::input_iterator_tag;
		using value_type = std::filesystem::path;
		using difference_type = std::filesystem::recursive_directory_iterator::difference_type;
		using pointer = value_type*;
		using reference = value_type&;

		glob_generic_path_t* container;

		inline iterator& operator++() {
			++container->it;
			container->next_match();
			return *this;
		}
		inline void operator++(int) {
			++*this;
		}

		inline auto operator*() const {
			return std::filesystem::relative(container->it->path(), container->base);
		}

		[[nodiscard]] inline bool operator==(ranges::default_sentinel_t) const noexcept {
			return container->it == std::filesystem::recursive_directory_iterator{};
		}

		[[nodiscard]] inline bool operator!=(ranges::default_sentinel_t s) const noexcept {
			return !(*this == s);
		}
	};

private:
	libv::WildcardGlobMatcher matcher;
	std::filesystem::recursive_directory_iterator it;
	std::filesystem::path base;

public:
	[[nodiscard]] inline iterator begin() noexcept {
		return iterator{this};
	}

	[[nodiscard]] constexpr inline ranges::default_sentinel_t end() const noexcept {
		return ranges::default_sentinel;
	}

private:
	void next_match() {
		while (it != std::filesystem::recursive_directory_iterator{} && !matcher.match(std::filesystem::relative(it->path(), base).generic_string()))
			++it;
	}

public:
	inline glob_generic_path_t(const std::filesystem::path& path, const std::string_view pattern) noexcept :
		matcher(pattern),
		it(path),
		base(std::move(path)) {

		next_match();
	}
};

static_assert(std::input_iterator<glob_generic_path_t::iterator>);
static_assert(std::ranges::input_range<glob_generic_path_t>);

} // namespace detail ------------------------------------------------------------------------------

struct glob_generic_path_base_fn {
	auto operator()(const std::filesystem::path& path, const std::string_view pattern) const {
		return detail::glob_generic_path_t(path, pattern);
	}
};

struct glob_generic_path_fn : glob_generic_path_base_fn {
	using glob_generic_path_base_fn::operator();

	constexpr auto operator()(const std::string_view pattern) const {
		return ranges::make_view_closure(ranges::bind_back(glob_generic_path_base_fn{}, pattern));
	}
};

/// Iterates over the path with std::filesystem::directory_entry if it matches the glob pattern
static constexpr ranges::view::view_closure<glob_generic_path_fn> glob_generic_path{};

// -------------------------------------------------------------------------------------------------

} // namespace view
} // namespace libv
