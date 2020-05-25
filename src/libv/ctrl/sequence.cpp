// Project: libv.control, Created on 2020.03.16. 16:41, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ctrl/sequence.hpp>
// std
#include <algorithm>
// pro
#include <libv/ctrl/enum.hpp>
#include <libv/ctrl/info.hpp>


namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

std::ostream& Sequence::to_stream_symbol(std::ostream& os) const {
	return libv::ctrl::to_stream_symbol(os, *this);
}
std::ostream& Sequence::to_stream_name(std::ostream& os) const{
	return libv::ctrl::to_stream_name(os, *this);
}

std::string Sequence::to_string_symbol() const {
	return libv::ctrl::to_string_symbol(*this);
}
std::string Sequence::to_string_name() const {
	return libv::ctrl::to_string_name(*this);
}

std::string Sequence::to_string() const {
	return to_string_name();
}

// -------------------------------------------------------------------------------------------------

void Sequence::normalize() {
	for (auto& combination : combinations)
		combination.normalize();
	// TODO P2: libv.control: During normalization throw out empty combinations
}

validation_result Sequence::validate() const noexcept {
	validation_result result = validation_result::success;

	// redundant_input_in_combination

	for (const auto& combination : combinations) {
		for (size_t i = 0; i < combination.inputs.size(); ++i) {
			for (size_t j = 0; j < combination.inputs.size(); ++j) {
				if (i == j)
					continue;

				if (alias_match(combination.inputs[i].id, combination.inputs[j].id))
					result |= validation_result::redundant_input_in_combination;
			}
		}
	}

	// multiple_action_in_combination

	for (const auto& combination : combinations) {
		size_t count = 0;
		for (const auto& input : combination.inputs)
			if (input.dia == DigitalInputAction::press
					|| input.dia == DigitalInputAction::repeat
					|| input.dia == DigitalInputAction::release
					|| input.dia == DigitalInputAction::any)
				count++;

		if (count > 1)
			result |= validation_result::multiple_action_in_combination;
	}

	// analog_with_dia

	for (const auto& combination : combinations)
		for (const auto& input : combination.inputs)
			if (input.id.is_analog() && input.dia != DigitalInputAction::auto_)
				result |= validation_result::analog_with_dia;

	// opposite_analog_dimension

	for (const auto& combination : combinations) {
		for (size_t i = 0; i < combination.inputs.size(); ++i) {
			for (size_t j = 0; j < combination.inputs.size(); ++j) {
				if (i == j)
					continue;

				const auto& lhs = combination.inputs[i].id;
				const auto& rhs = combination.inputs[j].id;

				if (lhs.type() != rhs.type() || !lhs.is_analog())
					continue;

				const auto lhs_ad = lhs.analogDimension();
				const auto rhs_ad = rhs.analogDimension();

				if ((lhs_ad == AnalogDimension::x_plus && rhs_ad == AnalogDimension::x_minus) ||
						(lhs_ad == AnalogDimension::x_minus && rhs_ad == AnalogDimension::x_plus) ||
						(lhs_ad == AnalogDimension::y_plus && rhs_ad == AnalogDimension::y_minus) ||
						(lhs_ad == AnalogDimension::y_minus && rhs_ad == AnalogDimension::y_plus))
					result |= validation_result::conflicting_analog_dimension;
			}
		}
	}

	// empty

	if (combinations.empty())
		result |= validation_result::empty;

	// contains_invalid_input

	for (const auto& combination : combinations)
		for (const auto& input : combination.inputs)
			if (input.id.type() == SourceType::invalid)
				result |= validation_result::contains_invalid_input;

	// contains_empty_combination

	for (const auto& combination : combinations)
		if (combination.inputs.empty())
			result |= validation_result::contains_empty_combination;

	// ---

	return result;
}

// -------------------------------------------------------------------------------------------------

bool operator<(const Sequence& lhs, const Sequence& rhs) noexcept {
	return std::lexicographical_compare(lhs.combinations.begin(), lhs.combinations.end(), rhs.combinations.begin(), rhs.combinations.end());
}

bool operator==(const Sequence& lhs, const Sequence& rhs) noexcept {
	return std::equal(lhs.combinations.begin(), lhs.combinations.end(), rhs.combinations.begin(), rhs.combinations.end());
}

// -------------------------------------------------------------------------------------------------

} // namespace ctrl
} // namespace libv
