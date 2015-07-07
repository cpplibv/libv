// File:   resource_manager_test_util.hpp

#pragma once

struct TestRA {
	static int instanceNumber;
	TestRA(const std::string& n) : n(n) {
		++instanceNumber;
	}
	const std::string& getn() const {
		return n;
	}
	~TestRA() {
		--instanceNumber;
	}
	bool operator<(const TestRA& r) const {
		return n < r.n;
	}
private:
	std::string n;
	friend bool operator<(const std::string& t, const TestRA& r) {
		return t < r.n;
	}
	friend bool operator<(const TestRA& r, const std::string& t) {
		return r.n < t;
	}
};

struct TestRB {
	static int instanceNumber;
	TestRB(const std::string& n, const std::string& m) : n(n), m(m) {
		++instanceNumber;
	}
	const std::string& getn() const {
		return n;
	}
	~TestRB() {
		--instanceNumber;
	}
	bool operator<(const TestRB& r) const {
		return n < r.n;
	}
private:
	std::string n;
	TestRA m;
	friend bool operator<(const std::string& t, const TestRB& r) {
		return t < r.n;
	}
	friend bool operator<(const TestRB& r, const std::string& t) {
		return r.n < t;
	}
};

struct NonCopyable {
	NonCopyable() {}
	NonCopyable(const NonCopyable&) = delete;
};
struct TestRNonCopyArged {
	TestRNonCopyArged(const NonCopyable&) {
	}
	bool operator<(const TestRNonCopyArged&) const {
		return true;
	}
private:
	friend bool operator<(const NonCopyable&, const TestRNonCopyArged&) {
		return true;
	}
	friend bool operator<(const TestRNonCopyArged&, const NonCopyable&) {
		return false;
	}
};