// Project: libv.control, Created on 2020.03.16. 16:41, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <libv/control/fwd.hpp>
// pro
#include <libv/control/binding_level.hpp>
#include <libv/control/enum.hpp>
#include <libv/control/scale.hpp>
#include <libv/control/sequence.hpp>
#include <libv/control/stated_sequence.hpp>


namespace libv {
namespace control {

// -------------------------------------------------------------------------------------------------

// NOTE: rename back to binding_outcome
//struct binding_result {
//	bool success;
//
//	//	failure_invalid_sequence,
//	//	failure_incompatible_sequence_and_feature,
//	int error;
//
//	//	warning_sequence_collusion,
//	//	warning_sequence_collusion_prefix,
//	//	warning_sequence_collusion_multi_stroke,
//	int warning;
//};

// -------------------------------------------------------------------------------------------------

struct Binding {
//	vector<class Feature*> features;
	std::string feature_name_;
	StatedSequence sequence_;
	scale_type scale_;
	binding_level level;
	OperationBinary operation_binary = OperationBinary::state;

public:
	explicit inline Binding(std::string&& feature_name, Sequence sequence, scale_type scale, binding_level level) :
		feature_name_(std::move(feature_name)),
		sequence_(std::move(sequence)),
		scale_(scale),
		level(level) { }

public:
	[[nodiscard]] constexpr inline const std::string& feature_name() const noexcept {
		return feature_name_;
	}
	[[nodiscard]] inline Sequence sequence() const noexcept {
		return sequence_.rebuild_sequence();
	}
	[[nodiscard]] inline int complexity() const noexcept {
		return static_cast<int>(sequence_.current_combination().inputs.size());
	}
	[[nodiscard]] constexpr inline scale_type scale() const noexcept {
		return scale_;
	}
};

// -------------------------------------------------------------------------------------------------

} //namespace control
} //namespace libv
