// Project: libv.ui, File: src/libv/ui/event/base_event.hpp, Author: Császár Mátyás [Vader]

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
