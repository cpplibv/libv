// Project: libv.ctrl, File: src/libv/ctrl/combination.cpp, Author: Császár Mátyás [Vader]

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

std::ostream& Combination::to_stream_symbol(std::ostream& os) const {
	return libv::ctrl::to_stream_symbol(os, *this);
}
std::ostream& Combination::to_stream_name(std::ostream& os) const{
	return libv::ctrl::to_stream_name(os, *this);
}

std::string Combination::to_string_symbol() const {
	return libv::ctrl::to_string_symbol(*this);
}
std::string Combination::to_string_name() const {
	return libv::ctrl::to_string_name(*this);
}

std::string Combination::to_string() const {
	return to_string_name();
}

// -------------------------------------------------------------------------------------------------

void Combination::normalize() {
	// Merge digital input actions
	//		auto > press
	//		auto > repeat
	//		auto > release
	//		auto > any
	//		auto > hold
	//		any > press
	//		any > release
	//		any > repeat
	//		repeat > press
	for (size_t i = 0; i < inputs.size(); ++i) {
		for (size_t j = 0; j < inputs.size(); ++j) {
			if (i == j)
				continue;

			if (inputs[i].id != inputs[j].id)
				continue;

			auto& lhs = inputs[i].dia;
			const auto rhs = inputs[j].dia;

			if (lhs == DigitalInputAction::auto_) {
				if (rhs == DigitalInputAction::press || rhs == DigitalInputAction::repeat ||
						rhs == DigitalInputAction::release || rhs == DigitalInputAction::any ||
						rhs == DigitalInputAction::hold)
					lhs = rhs;
			} else if (lhs == DigitalInputAction::any) {
				if (rhs == DigitalInputAction::press || rhs == DigitalInputAction::repeat ||
						rhs == DigitalInputAction::release)
					lhs = rhs;
			} else if (lhs == DigitalInputAction::repeat) {
				if (rhs == DigitalInputAction::press)
					lhs = rhs;
			}
		}
	}

	// Merge analog input actions
	//		X > X+
	//		X > X-
	//		Y > Y+
	//		Y > Y-
	for (size_t i = 0; i < inputs.size(); ++i) {
		for (size_t j = 0; j < inputs.size(); ++j) {
			if (i == j)
				continue;

			auto& lhs = inputs[i].id;
			const auto rhs = inputs[j].id;

			if (lhs.type() != rhs.type() || !lhs.is_analog())
				continue;

			const auto lhsdim = lhs.analogDimension();
			const auto rhsdim = rhs.analogDimension();

			if (lhsdim == AnalogDimension::x) {
				if (rhsdim == AnalogDimension::x_plus || rhsdim == AnalogDimension::x_minus)
					lhs.analogDimension(rhsdim);
			} else if (lhsdim == AnalogDimension::y) {
				if (rhsdim == AnalogDimension::y_plus || rhsdim == AnalogDimension::y_minus)
					lhs.analogDimension(rhsdim);
			}
		}
	}

	// Choose more specialized alias
	for (size_t i = 0; i < inputs.size(); ++i) {
		for (size_t j = 0; j < inputs.size(); ++j) {
			if (i == j)
				continue;

			auto& lhs = inputs[i].id;
			auto& rhs = inputs[j].id;

			const auto compare_result = more_specialized(lhs, rhs);
			if (compare_result == 0)
				continue;

			if (compare_result < 0)
				lhs = rhs;
			else
				rhs = lhs;
		}
	}

	// Fixing not valid inputs
	// TODO P2: libv.control: During normalization throw out invalid inputs
	// TODO P5: libv.control: Attempt to fix invalid input combination, but only if there is client consent

	// Sort inputs
	std::sort(inputs.begin(), inputs.end());

	// Remove duplicates
	inputs.erase(std::unique(inputs.begin(), inputs.end()), inputs.end());
}

// -------------------------------------------------------------------------------------------------

bool operator<(const Combination& lhs, const Combination& rhs) noexcept {
	return std::lexicographical_compare(lhs.inputs.begin(), lhs.inputs.end(), rhs.inputs.begin(), rhs.inputs.end());
}

bool operator==(const Combination& lhs, const Combination& rhs) noexcept {
	return std::equal(lhs.inputs.begin(), lhs.inputs.end(), rhs.inputs.begin(), rhs.inputs.end());
}

// -------------------------------------------------------------------------------------------------

} // namespace ctrl
} // namespace libv
