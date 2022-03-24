// Project: libv.utility, File: src/libv/utility/flag_enum.hpp

#pragma once

// std
#include <cstdint>
#include <type_traits>


namespace libv {

// -------------------------------------------------------------------------------------------------

// TODO P5: bit abstraction could be improved with bits abstraction, this should be in its own header.
//			Note: investigate the possibility of bits being std::bitset<?>
//			Note: bit(i) could be -> bits, makes life easier
//			bit(1, 2, 3) -> bits;
//			bit(1) | bit(2) -> bits;
//			flag_enum | bit(1) -> flag_enum;                     DONE
//			flag_enum | bits -> flag_enum;
//			Motivation:
//			flag_enum<...> green = bit(1);                     DONE
//			flag_enum<...> blue  = green | bit(2);             DONE
//			flag_enum<...> blue  = green | bit(2) | bit(3);    DONE
//			flag_enum<...> alpha = bit(1, 2) ^ blue | bit(4);
//
//bits bit(int index) {
//	return bits(1 << index);
//}

struct bit {
private:
	int index_;
public:
	constexpr inline explicit bit(int index) noexcept : index_(index) {}
	constexpr inline int index() const noexcept {
		return index_;
	}
};

// -------------------------------------------------------------------------------------------------

/// @usage \code
/// using colorflag = flag_enum<uint8_t, struct colorflag_tag>;
/// static constexpr colorflag red   = bit(0);
/// static constexpr colorflag green = bit(1);
/// static constexpr colorflag blue  = bit(2);
/// static constexpr colorflag alpha = bit(3);
/// \endcode
///
template<typename Underlying, typename Tag>
struct flag_enum {
	static_assert(std::is_unsigned_v<Underlying>, "Underlying type must be an unsigned integral type");

private:
	Underlying value_;

public:
	using value_type = Underlying;

	constexpr inline flag_enum() noexcept : value_(0) { }
	constexpr inline flag_enum(const flag_enum& rhs) noexcept = default;
	constexpr inline flag_enum(flag_enum&& rhs) noexcept = default;
	constexpr inline explicit flag_enum(const Underlying value) noexcept : value_(value) { }

	constexpr inline flag_enum(const bit bit_index) noexcept : value_(static_cast<Underlying>(Underlying{1} << bit_index.index())) { }

	constexpr inline flag_enum& operator=(const flag_enum& rhs) & noexcept = default;
	constexpr inline flag_enum& operator=(flag_enum&& rhs) & noexcept = default;

public:
	constexpr static inline flag_enum all() noexcept {
		return flag_enum(~Underlying{0});
	}
	constexpr static inline flag_enum none() noexcept {
		return flag_enum(Underlying{0});
	}

public:
	constexpr inline bool match_mask(const flag_enum mask) const noexcept {
		return (value_ & mask.value_) == mask.value_;
	}
	constexpr inline bool match_mask(const flag_enum mask, const flag_enum other) const noexcept {
		return (value_ & mask.value_) == (other.value_ & mask.value_);
	}
	constexpr inline bool match_full(const flag_enum other) const noexcept {
		return value_ == other.value_;
	}
	constexpr inline bool match_any(const flag_enum other) const noexcept {
		return value_ & other.value_;
	}

public:
	constexpr inline explicit operator Underlying() const noexcept {
		return value_;
	}
	constexpr inline Underlying value() const noexcept {
		return value_;
	}
	constexpr inline explicit operator bool() const noexcept {
		return value_ != 0;
	}

	constexpr inline flag_enum& set(const flag_enum f) noexcept {
		value_ |= f.value_;
		return *this;
	}
	constexpr inline flag_enum& set(const bit f) noexcept {
		value_ |= (Underlying{1} << f.index());
		return *this;
	}
	constexpr inline flag_enum& reset(const flag_enum f) noexcept {
		value_ &= static_cast<Underlying>(~f.value_);
		return *this;
	}
	constexpr inline flag_enum& reset(const bit f) noexcept {
		value_ &= ~(Underlying{1} << f.index());
		return *this;
	}
	constexpr inline flag_enum& flip(const flag_enum f) noexcept {
		value_ ^= f.value_;
		return *this;
	}
	constexpr inline flag_enum& flip(const bit f) noexcept {
		value_ ^= (Underlying{1} << f.index());
		return *this;
	}
	constexpr inline flag_enum& clear() noexcept {
		value_ = none();
		return *this;
	}

	constexpr inline flag_enum& set_to(const flag_enum f, bool value) noexcept {
		if (value)
			set(f);
		else
			reset(f);
		return *this;
	}

	constexpr inline flag_enum operator~() const noexcept {
		return flag_enum(static_cast<Underlying>(~value_));
	}

	constexpr inline bool operator==(const flag_enum& f) const noexcept = default;

	constexpr inline flag_enum& operator|=(const flag_enum f) & noexcept {
		value_ |= f.value_;
		return *this;
	}
	constexpr inline flag_enum& operator&=(const flag_enum f) & noexcept {
		value_ &= f.value_;
		return *this;
	}
	constexpr inline flag_enum& operator^=(const flag_enum f) & noexcept {
		value_ ^= f.value_;
		return *this;
	}

	constexpr inline flag_enum& operator|=(const bit rhs) & noexcept {
		value_ = static_cast<Underlying>(value_ | 1 << rhs.index());
		return *this;
	}
	constexpr inline flag_enum& operator&=(const bit rhs) & noexcept {
		value_ = static_cast<Underlying>(value_ & 1 << rhs.index());
		return *this;
	}
	constexpr inline flag_enum& operator^=(const bit rhs) & noexcept {
		value_ = static_cast<Underlying>(value_ ^ 1 << rhs.index());
		return *this;
	}

	friend constexpr inline flag_enum operator|(const flag_enum lhs, const flag_enum rhs) noexcept {
		return flag_enum(lhs.value_ | rhs.value_);
	}
	friend constexpr inline flag_enum operator&(const flag_enum lhs, const flag_enum rhs) noexcept {
		return flag_enum(lhs.value_ & rhs.value_);
	}
	friend constexpr inline flag_enum operator^(const flag_enum lhs, const flag_enum rhs) noexcept {
		return flag_enum(lhs.value_ ^ rhs.value_);
	}

	friend constexpr inline flag_enum operator|(const bit lhs, const flag_enum rhs) noexcept {
		return flag_enum(static_cast<Underlying>(Underlying{1} << lhs.index()) | rhs.value_);
	}
	friend constexpr inline flag_enum operator&(const bit lhs, const flag_enum rhs) noexcept {
		return flag_enum(static_cast<Underlying>(Underlying{1} << lhs.index()) & rhs.value_);
	}
	friend constexpr inline flag_enum operator^(const bit lhs, const flag_enum rhs) noexcept {
		return flag_enum(static_cast<Underlying>(Underlying{1} << lhs.index()) ^ rhs.value_);
	}

	friend constexpr inline flag_enum operator|(const flag_enum lhs, const bit rhs) noexcept {
		return flag_enum(lhs.value_ | static_cast<Underlying>(Underlying{1} << rhs.index()));
	}
	friend constexpr inline flag_enum operator&(const flag_enum lhs, const bit rhs) noexcept {
		return flag_enum(lhs.value_ & static_cast<Underlying>(Underlying{1} << rhs.index()));
	}
	friend constexpr inline flag_enum operator^(const flag_enum lhs, const bit rhs) noexcept {
		return flag_enum(lhs.value_ ^ static_cast<Underlying>(Underlying{1} << rhs.index()));
	}

	template <typename OStream>
	friend inline OStream& operator<<(OStream& os, flag_enum val) {
		// Workaround for char ostreaming
		if constexpr (std::is_same_v<Underlying, uint8_t>)
			os << static_cast<uint16_t>(val.value_);
		else
			os << val.value_;

		return os;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
