// Project: libv.sig, File: src/libv/sig/accumulator.hpp

#pragma once

namespace libv {

template <typename T>
class Accumulator {
protected:
	Accumulator() { }
	virtual ~Accumulator() { }
public:
	static Accumulator<T>* get() {
		static Accumulator<T> instance;
		return &instance;
	}
	virtual T begin() {
		return T();
	}
	virtual bool add(T& result, const T& value) {
		result += value;
		return true;
	}
};

template <>
struct Accumulator<void> {
	static Accumulator<void>* get() {
		return nullptr;
	}
};

template <typename T>
class AccumulatorLogicalAnd final : public Accumulator<T> {
public:
	static Accumulator<T>* get() {
		static AccumulatorLogicalAnd<T> instance;
		return &instance;
	}
	virtual T begin() override {
		return 1;
	}
	virtual bool add(T& result, const T& value) override {
		result = result && value;
		return false;
	}
};

template <typename T>
class AccumulatorLogicalOr final : public Accumulator<T> {
public:
	static Accumulator<T>* get() {
		static AccumulatorLogicalOr<T> instance;
		return &instance;
	}
	virtual T begin() override {
		return 1;
	}
	virtual bool add(T& result, const T& value) override {
		result = result || value;
		return false;
	}
};

} // namespace libv
