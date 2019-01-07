// File: flag_enum.hpp - Created on 2018.04.10. 03:06 - Author: Vader

#pragma once

// std
#include <type_traits>


namespace libv {

// -------------------------------------------------------------------------------------------------

// TODO P5: bit abstraction could be improved with bits abstraction, this should be in its own header.
//			Note: investigate the possibility of bits being std::bitset<?>
//			bit(1, 2, 3) -> bits;
//			bit(1) | bit(2) -> bits;
//			bitflag | bit(1) -> bitflag;                     DONE
//			bitflag | bits -> bitflag;
//			Motivation:
//			bitflag<...> green = bit(1);                     DONE
//			bitflag<...> blue  = green | bit(2);             DONE
//			bitflag<...> blue  = green | bit(2) | bit(3);    DONE
//			bitflag<...> alpha = bit(1, 2) ^ blue | bit(4);

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
/// using colorflag = bitflag<uint8_t, struct colorflag_tag>;
/// constexpr static colorflag red   = bit(0);
/// constexpr static colorflag green = bit(1);
/// constexpr static colorflag blue  = bit(2);
/// constexpr static colorflag alpha = bit(3);
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
	constexpr inline static flag_enum all() noexcept {
		return flag_enum(~Underlying{0});
	}
	constexpr inline static flag_enum none() noexcept {
		return flag_enum(Underlying{0});
	}

public:
	constexpr inline bool match_mask(const flag_enum mask) const noexcept {
		return (value_ & mask.value_) == mask.value_;
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
		return bool{value_};
	}

	constexpr inline flag_enum operator~() const noexcept {
		return flag_enum(static_cast<Underlying>(~value_));
	}

	constexpr inline bool operator==(const flag_enum f) const noexcept {
		return value_ == f.value_;
	}
	constexpr inline bool operator!=(const flag_enum f) const noexcept {
		return value_ != f.value_;
	}

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
