// Project: libv.control, Created on 2020.03.16. 16:41, Author: Császár Mátyás [Vader]

#pragma once

// ext
#include <boost/container/small_vector.hpp>
// pro
#include <libv/ctrl/duration.hpp>
#include <libv/ctrl/sequence.hpp>
#include <libv/ctrl/time_point.hpp>


// <<< P9: cleanup state_sequence file
namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

struct StatedInput {
	InputID id;
	DigitalInputAction dia;
	bool active = false;
};

struct StatedCombination {
	boost::container::small_vector<StatedInput, 3> inputs;
	bool active = false;

public:
	Combination build_combination() const {
		Combination result;
		for (const StatedInput& input : inputs)
			result.add_input(Input{input.id, input.dia});
		return result;
	}
};

struct StatedSequence {
	boost::container::small_vector<StatedCombination, 2> combinations;
	size_t checkpoint = 0; // The index of the currently active combination
	bool active = false;

	StatedSequence(const Sequence& src_sequence) {
		for (const Combination& src_combination : src_sequence.combinations) {
			StatedCombination& combination = combinations.emplace_back();

			for (const Input& src_input : src_combination.inputs) {
				StatedInput& input = combination.inputs.emplace_back();
				input.id = src_input.id;
				input.dia = src_input.dia;
			}
		}
	}

	Sequence rebuild_sequence() const {
		Sequence result;
		for (const StatedCombination& combination : combinations)
			result.add_combination(combination.build_combination());
		return result;
	}

	StatedCombination& previous_combination() noexcept {
		assert(checkpoint != 0 && "No previous combination");
		return combinations[checkpoint == 0 ? 0 : checkpoint - 1];
	}

	StatedCombination& current_combination() noexcept {
		assert(checkpoint < combinations.size() && "No more current combination");
		return combinations[checkpoint];
	}

	const StatedCombination& current_combination() const noexcept {
		assert(checkpoint < combinations.size() && "No more current combination");
		return combinations[checkpoint];
	}

	[[nodiscard]] inline bool is_active() const noexcept {
		return active;
	}

	bool is_last_combination() const noexcept {
		return checkpoint + 1 == combinations.size();
	}

	bool is_sequence() const noexcept {
		return combinations.size() > 1;
	}

	bool in_progress() const noexcept {
		return checkpoint != 0;
	}
};

// -------------------------------------------------------------------------------------------------

} //namespace ctrl
} //namespace libv
