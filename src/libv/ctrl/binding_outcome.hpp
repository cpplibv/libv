// Project: libv.ctrl, File: src/libv/ctrl/controls.hpp

#pragma once


namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

enum class binding_outcome {
	success = 0,

	failure_invalid_sequence,
	failure_invalid_sequence_str,

	//	(?) failure_incompatible_sequence_and_feature,
	//	warning_sequence_collusion,
	//	warning_sequence_collusion_prefix,
	//	warning_sequence_collusion_multi_stroke,
};

enum class unbinding_outcome {
	success = 0,

	failure_invalid_sequence_str,
	//	ambigous_sequence,
	//
	//	not_found_binding,
	//	not_found_feature,
	//	not_found_sequence,
};

// -------------------------------------------------------------------------------------------------

} //namespace ctrl
} //namespace libv
