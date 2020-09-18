// Project: libv.ctrl, File: src/libv/ctrl/controls_introspection.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ctrl/controls.hpp>
// std
#include <cassert>
// pro
#include <libv/ctrl/binding.hpp>
#include <libv/ctrl/controls_impl.lpp>
#include <libv/ctrl/feature.hpp>
#include <libv/ctrl/search_mode.hpp>


namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

namespace {

[[nodiscard]] inline bool check_feature(std::string_view feature, std::string_view x, search_mode mode) noexcept {
	switch (mode) {
	case search_mode::starts_with:
		return feature.starts_with(x);
	case search_mode::ends_with:
		return feature.ends_with(x);
	case search_mode::matches:
		return feature == x;
	case search_mode::contains:
		return feature.find(x) != std::string::npos;
	case search_mode::not_starts_with:
		return !feature.starts_with(x);
	case search_mode::not_ends_with:
		return !feature.ends_with(x);
	case search_mode::not_matches:
		return feature != x;
	case search_mode::not_contains:
		return feature.find(x) == std::string::npos;
	}

	assert(false && "Invalid search_mode");
	return false;
}

[[nodiscard]] inline bool check_binding(const StatedSequence& sequence, const StatedSequence& x, search_mode mode) noexcept {
	/// Note: will allow any alias matching and does not forbids unused element in the sequence
	const auto compare = [](const StatedCombination& lhs, const StatedCombination& rhs) {
		for (const auto rhs_input : rhs.inputs) {
			bool found_similar = false;
			for (const auto lhs_input : lhs.inputs) {
				if (alias_match(lhs_input.id, rhs_input.id)) {
					found_similar = true;
					break;
				}
			}
			if (not found_similar)
				return false;
		}
		return true;
	};

	switch (mode) {
	case search_mode::starts_with:
		return std::mismatch(
				sequence.combinations.begin(), sequence.combinations.end(),
				x.combinations.begin(), x.combinations.end(),
				compare).second == x.combinations.end();
	case search_mode::ends_with:
		return std::mismatch(
				sequence.combinations.rbegin(), sequence.combinations.rend(),
				x.combinations.rbegin(), x.combinations.rend(),
				compare).second == x.combinations.rend();
	case search_mode::matches:
		return std::equal(sequence.combinations.begin(), sequence.combinations.end(),
				x.combinations.begin(), x.combinations.end(),
				compare);
	case search_mode::contains:
		return std::search(sequence.combinations.begin(), sequence.combinations.end(),
				x.combinations.begin(), x.combinations.end(),
				compare) != sequence.combinations.end();
	case search_mode::not_starts_with:
		return std::mismatch(
				sequence.combinations.begin(), sequence.combinations.end(),
				x.combinations.begin(), x.combinations.end(),
				compare).second != x.combinations.end();
	case search_mode::not_ends_with:
		return std::mismatch(
				sequence.combinations.rbegin(), sequence.combinations.rend(),
				x.combinations.rbegin(), x.combinations.rend(),
				compare).second != x.combinations.rend();
	case search_mode::not_matches:
		return !std::equal(sequence.combinations.begin(), sequence.combinations.end(),
				x.combinations.begin(), x.combinations.end(),
				compare);
	case search_mode::not_contains:
		return std::search(sequence.combinations.begin(), sequence.combinations.end(),
				x.combinations.begin(), x.combinations.end(),
				compare) == sequence.combinations.end();
	}

	assert(false && "Invalid search_mode");
	return false;
}

} // namespace

void Controls::foreach_codepoints(libv::function_ref<void(Keycode, Codepoint)> function) {
	for (const auto& [keycode, codepoint] : self->codepoint_mappings)
		function(keycode, codepoint);
}

bool Controls::is_feature(std::string_view name) const noexcept {
	// TODO P2: C++20 contains with temporary string instead of string_view
	return self->features.contains(std::string(name));
}

void Controls::foreach_features(libv::function_ref<void(const Feature&)> function) {
	for (const auto& [key, feature] : self->features)
		function(feature);
}

void Controls::foreach_features_in_context(libv::function_ref<void(const Feature&)> function) {
	for (const auto& [key, feature] : self->features)
		if (self->active_contexts.contains(feature.context))
			function(feature);
}

void Controls::foreach_features_search(libv::function_ref<void(const Feature&)> function, std::string_view name, search_mode mode) {
	for (const auto& [key, feature] : self->features)
		if (check_feature(feature.name(), name, mode))
			function(feature);
}

void Controls::foreach_bindings(libv::function_ref<void(const Binding&)> function) {
	for (const auto& binding : self->bindings)
		function(*binding);
}

void Controls::foreach_bindings_search(libv::function_ref<void(const Binding&)> function, const Sequence& seq, search_mode mode) {
	for (const auto& binding : self->bindings)
		if (check_binding(binding->sequence(), seq, mode))
			function(*binding);
}

void Controls::foreach_bindings_search(libv::function_ref<void(const Binding&)> function, std::string_view name, search_mode mode) {
	for (const auto& binding : self->bindings)
		if (check_feature(binding->feature_name(), name, mode))
			function(*binding);
}

// -------------------------------------------------------------------------------------------------

} // namespace ctrl
} // namespace libv
