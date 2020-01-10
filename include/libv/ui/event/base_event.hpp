// File:   interest.hpp Author: Vader Created on 15 April 2019, 06:10

#pragma once


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct BaseEvent {
private:
	mutable bool stop_propagation_ = false;

public:
	constexpr inline void stop_propagation() const noexcept {
		stop_propagation_ = true;
	}

	[[nodiscard]] constexpr inline bool propagation_stopped() const noexcept {
		return stop_propagation_;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
