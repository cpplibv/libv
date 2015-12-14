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

struct TestRM {
	static int instanceNumber;
	TestRM(int n, int m) : n0(n), n1(m) {
		++instanceNumber;
	}
	~TestRM() {
		--instanceNumber;
	}
	bool operator<(const TestRM& r) const {
		return n0 + n1 < r.n0 + r.n1;
	}
	int n0;
	int n1;
private:
	friend bool operator<(const std::tuple<int, int>& t, const TestRM& r) {
		return std::get<0>(t) + std::get<1>(t) < r.n0 + r.n1;
	}
	friend bool operator<(const TestRM& r, const std::tuple<int, int>& t) {
		return r.n0 + r.n1 < std::get<0>(t) + std::get<1>(t);
	}
};

// -------------------------------------------------------------------------------------------------

struct LoadableTestResource {
	int n;
	int loaded = 0;
	int* unloaded = nullptr;
	LoadableTestResource(int n) : n(n) { }
	void load(std::shared_ptr<LoadableTestResource>) {
		++loaded;
	}
	void unload(std::shared_ptr<LoadableTestResource>) {
		if (unloaded)
			++(*unloaded);
	}
	bool operator<(const LoadableTestResource& r) const {
		return n < r.n;
	}
	friend bool operator<(int t, const LoadableTestResource& r) {
		return t < r.n;
	}
	friend bool operator<(const LoadableTestResource& r, int t) {
		return r.n < t;
	}
};

// -------------------------------------------------------------------------------------------------

struct LoadableTestResourceB {
	int n;
	int argCount;
	template<typename... Args>
	LoadableTestResourceB(int n, Args...) : n(n), argCount(sizeof...(Args) + 1) { }
	void load(std::shared_ptr<LoadableTestResourceB>) { }
	void unload(std::shared_ptr<LoadableTestResourceB>) { }
	bool operator<(const LoadableTestResourceB& r) const {
		return n < r.n;
	}
	friend bool operator<(int t, const LoadableTestResourceB& r) {
		return t < r.n;
	}
	friend bool operator<(const LoadableTestResourceB& r, int t) {
		return r.n < t;
	}
	friend bool operator<(const std::tuple<int, int>& t, const LoadableTestResourceB& r) {
		return std::get<0>(t) < r.n;
	}
	friend bool operator<(const LoadableTestResourceB& r, const std::tuple<int, int>& t) {
		return r.n < std::get<0>(t);
	}
};