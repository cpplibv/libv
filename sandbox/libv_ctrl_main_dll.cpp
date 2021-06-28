// Project: libv.ctrl, File: sandbox/libv_ctrl_main.cpp, Author: Császár Mátyás [Vader]

// std
#include <iostream>
// pro
//#include <libv/ctrl/controls.hpp>
//#include <libv/ctrl/feature_register.hpp>
#include <libv/ctrl/parse.hpp>
#include <libv/ctrl/combination.hpp>



// -------------------------------------------------------------------------------------------------

//inline libv::LoggerModule log_sandbox{libv::logger_stream, "sandbox"};

// -------------------------------------------------------------------------------------------------

static constexpr auto seek_delim = [](const auto begin, const auto end, const auto delim) {
	auto it = begin;
	bool skipped = false;

	while (true) {
		while (it != end && *it != delim)
			++it;

		if (it == end)
			break;

		auto it_before = it == begin ? it : it - 1;
		auto it_after = it + 1 == end ? it : it + 1;

		if (skipped || *it_before != '\'' || *it != delim || *it_after != '\'')
			break;

		// Special case hit, skip token and loop back around
		++it;
		skipped = true;
	}

	return it;
};

static constexpr auto sv = [](const auto begin, const auto end) {
	return std::string_view(&*begin, std::distance(begin, end));
};


//TODO CICA
std::optional<libv::ctrl::Combination> parse_combination_optional(const std::string_view str) {
	boost::container::small_vector<libv::ctrl::Input, 3> inputs;

	// Reverse iteration
	auto itr = str.rbegin();
	while (itr != str.rend()) {
		const auto segment_rbegin = itr;

		// First attempt parsing
		itr = seek_delim(segment_rbegin, str.rend(), '+');
		const auto window = sv(itr.base(), segment_rbegin.base());
		std::cout<<"window: "<<window<<std::endl;

		auto parsed_input = libv::ctrl::parse_input_optional(window);

		// Second attempt parsing with including the last delimiter as part of the segment
		if (!parsed_input && itr != str.rend()) {
			itr++;
			itr = seek_delim(itr, str.rend(), '+');

			const auto fallback_window = sv(itr.base(), segment_rbegin.base());
			std::cout<<"fallback_window: "<<fallback_window<<std::endl;
			parsed_input = libv::ctrl::parse_input_optional(fallback_window);
		}

		if (parsed_input)
			inputs.emplace_back(std::move(*parsed_input));
		else
			// Both first and second pass failed
			return std::nullopt;

		if (itr != str.rend()) {
			// Skip the used delimiter token
			itr++;

			if (itr == str.rend())
				// Skipping the token caused exhaustion, there is a missing segment
				return std::nullopt;
		}
	}

	if (inputs.empty())
		return std::nullopt;

	// Reverse fill result
	libv::ctrl::Combination result;
	for (auto it = inputs.rbegin(); it != inputs.rend(); ++it)
		result.add_input(*it);

	return result;
}
