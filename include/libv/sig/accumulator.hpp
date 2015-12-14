// File: Accumulator.hpp, Created on 2014. augusztus 9. 16:07, Author: Vader

#pragma once

namespace libv {

template <typename T>
class Accumulator {
protected:
	Accumulator() { }
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
class AccumulatorLogicalAnd : public Accumulator<T> {
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
class AccumulatorLogicalOr : public Accumulator<T> {
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

} //namespace libv
