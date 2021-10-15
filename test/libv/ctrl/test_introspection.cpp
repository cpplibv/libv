// Project: libv.ctrl, File: test/libv/ctrl/test_introspection.cpp

// hpp
#include <catch/catch.hpp>
// pro
#include <libv/ctrl/binding.hpp>
#include <libv/ctrl/controls.hpp>
#include <libv/ctrl/feature.hpp>
#include <libv/ctrl/parse.hpp>
#include <libv/ctrl/sequence.hpp>
#include "mock_queue.hpp"


// -------------------------------------------------------------------------------------------------

namespace {

namespace lc = libv::ctrl;

auto parse(std::string_view str) {
	return lc::parse_sequence_or_throw(str);
}

struct TestContext0 {
	size_t access_count = 0;
};
struct TestContext1 {
	size_t access_count = 0;
};
struct TestContext2 {
	size_t access_count = 0;
};
struct TestContext3 {
	size_t access_count = 0;
};

const auto sink_callback = [](auto...) {
};

} // namespace

// -------------------------------------------------------------------------------------------------

TEST_CASE("Feature introspection", "[libv.control]") {
	TestContext0 context0;
	TestContext1 context1;
	TestContext2 context2;
	lc::Controls controls;

	controls.feature_action<TestContext0>("test.action", sink_callback);
	controls.feature_analog<TestContext1>("test.analog", sink_callback);
	controls.feature_binary<TestContext2>("test.binary", sink_callback);
	controls.feature_action<TestContext3>("var.test.action", sink_callback);

	controls.bind("test.action", parse("A + S"));
	controls.bind("test.action", parse("Ctrl + A"));
	controls.bind("test.analog", parse("Ctrl + A"));
	controls.bind("test.analog", parse("Ctrl + MouseX"));
	controls.bind("test.binary", parse("Ctrl + A, A"));

	controls.context_enter(&context0);
	controls.context_enter(&context1);

	MockQueue<std::reference_wrapper<const lc::Feature>> output;

	SECTION("is_feature") {
		CHECK(controls.is_feature("test.action"));
		CHECK(controls.is_feature("test.analog"));
		CHECK(!controls.is_feature("test2.action"));
		CHECK(!controls.is_feature("test.action2"));
		CHECK(!controls.is_feature("test2.action2"));
	}

	SECTION("foreach_features") {
		controls.foreach_features(output.callback());
		output.sort_by([](const auto& v) { return v.get().name(); });

		REQUIRE(output.queue_size() == 4);
		CHECK(output.front().get().name() == "test.action");
		CHECK(output.front().get().is_action());

		output.pop_front();
		CHECK(output.front().get().name() == "test.analog");
		CHECK(output.front().get().is_analog());

		output.pop_front();
		CHECK(output.front().get().name() == "test.binary");
		CHECK(output.front().get().is_binary());

		output.pop_front();
		CHECK(output.front().get().name() == "var.test.action");
		CHECK(output.front().get().is_action());
	}

	SECTION("foreach_features_in_context") {
		controls.foreach_features_in_context(output.callback());
		output.sort_by([](const auto& v) { return v.get().name(); });

		REQUIRE(output.queue_size() == 2);
		CHECK(output.consume_front()->get().name() == "test.action");
		CHECK(output.consume_front()->get().name() == "test.analog");

		controls.context_leave<TestContext1>();
		controls.context_enter(&context2);

		controls.foreach_features_in_context(output.callback());
		output.sort_by([](const auto& v) { return v.get().name(); });

		REQUIRE(output.queue_size() == 2);
		CHECK(output.consume_front()->get().name() == "test.action");
		CHECK(output.consume_front()->get().name() == "test.binary");
	}

	SECTION("foreach_features_search") {
		controls.foreach_features_search(output.callback(), "test2", lc::search_mode::contains);
		REQUIRE(output.queue_size() == 0);
		controls.foreach_features_search(output.callback(), "test", lc::search_mode::contains);
		output.sort_by([](const auto& v) { return v.get().name(); });
		REQUIRE(output.queue_size() == 4);
		CHECK(output.consume_front()->get().name() == "test.action");
		CHECK(output.consume_front()->get().name() == "test.analog");
		CHECK(output.consume_front()->get().name() == "test.binary");
		CHECK(output.consume_front()->get().name() == "var.test.action");

		controls.foreach_features_search(output.callback(), "test2", lc::search_mode::starts_with);
		REQUIRE(output.queue_size() == 0);
		controls.foreach_features_search(output.callback(), "test", lc::search_mode::starts_with);
		output.sort_by([](const auto& v) { return v.get().name(); });
		REQUIRE(output.queue_size() == 3);
		CHECK(output.consume_front()->get().name() == "test.action");
		CHECK(output.consume_front()->get().name() == "test.analog");
		CHECK(output.consume_front()->get().name() == "test.binary");

		controls.foreach_features_search(output.callback(), "test", lc::search_mode::ends_with);
		REQUIRE(output.queue_size() == 0);
		controls.foreach_features_search(output.callback(), "action", lc::search_mode::ends_with);
		output.sort_by([](const auto& v) { return v.get().name(); });
		REQUIRE(output.queue_size() == 2);
		CHECK(output.consume_front()->get().name() == "test.action");
		CHECK(output.consume_front()->get().name() == "var.test.action");

		controls.foreach_features_search(output.callback(), "test", lc::search_mode::matches);
		REQUIRE(output.queue_size() == 0);
		controls.foreach_features_search(output.callback(), "test.action", lc::search_mode::matches);
		output.sort_by([](const auto& v) { return v.get().name(); });
		REQUIRE(output.queue_size() == 1);
		CHECK(output.consume_front()->get().name() == "test.action");

		controls.foreach_features_search(output.callback(), "test2", lc::search_mode::not_contains);
		REQUIRE(output.clear() == 4);
		controls.foreach_features_search(output.callback(), "test", lc::search_mode::not_contains);
		output.sort_by([](const auto& v) { return v.get().name(); });
		REQUIRE(output.queue_size() == 0);

		controls.foreach_features_search(output.callback(), "test2", lc::search_mode::not_starts_with);
		REQUIRE(output.clear() == 4);
		controls.foreach_features_search(output.callback(), "test", lc::search_mode::not_starts_with);
		output.sort_by([](const auto& v) { return v.get().name(); });
		REQUIRE(output.queue_size() == 1);
		CHECK(output.consume_front()->get().name() == "var.test.action");

		controls.foreach_features_search(output.callback(), "test", lc::search_mode::not_ends_with);
		REQUIRE(output.clear() == 4);
		controls.foreach_features_search(output.callback(), "action", lc::search_mode::not_ends_with);
		output.sort_by([](const auto& v) { return v.get().name(); });
		REQUIRE(output.queue_size() == 2);
		CHECK(output.consume_front()->get().name() == "test.analog");
		CHECK(output.consume_front()->get().name() == "test.binary");

		controls.foreach_features_search(output.callback(), "test", lc::search_mode::not_matches);
		REQUIRE(output.clear() == 4);
		controls.foreach_features_search(output.callback(), "test.action", lc::search_mode::not_matches);
		REQUIRE(output.queue_size() == 3);
		output.sort_by([](const auto& v) { return v.get().name(); });
		CHECK(output.consume_front()->get().name() == "test.analog");
		CHECK(output.consume_front()->get().name() == "test.binary");
		CHECK(output.consume_front()->get().name() == "var.test.action");
	}
}

TEST_CASE("Binding introspection", "[libv.control]") {
	TestContext0 context0;
	TestContext1 context1;
	lc::Controls controls;

	controls.feature_action<TestContext0>("test.action", sink_callback);
	controls.feature_analog<TestContext1>("test.analog", sink_callback);
	controls.feature_binary<TestContext2>("test.binary", sink_callback);
	controls.feature_action<TestContext3>("var.test.action", sink_callback);

	controls.bind("test.action", parse("A + S"));
	controls.bind("test.action", parse("Ctrl + A"));
	controls.bind("test.analog", parse("Ctrl + A"));
	controls.bind("test.analog", parse("Ctrl + MouseX"));
	controls.bind("test.binary", parse("Ctrl + A, A"));

	controls.context_enter(&context0);
	controls.context_enter(&context1);

	MockQueue<std::reference_wrapper<const lc::Binding>> output;

	SECTION("foreach_bindings") {
		controls.foreach_bindings(output.callback());
		output.sort_by([](const auto& v) { return std::tuple(v.get().feature_name(), v.get().sequence()); });

		REQUIRE(output.queue_size() == 5);
		CHECK(output.front().get().feature_name() == "test.action");
		CHECK(output.front().get().sequence() == parse("Ctrl + A"));
		output.pop_front();
		CHECK(output.front().get().feature_name() == "test.action");
		CHECK(output.front().get().sequence() == parse("A + S"));
		output.pop_front();
		CHECK(output.front().get().feature_name() == "test.analog");
		CHECK(output.front().get().sequence() == parse("Ctrl + A"));
		output.pop_front();
		CHECK(output.front().get().feature_name() == "test.analog");
		CHECK(output.front().get().sequence() == parse("Ctrl + MouseX"));
		output.pop_front();
		CHECK(output.front().get().feature_name() == "test.binary");
		CHECK(output.front().get().sequence() == parse("Ctrl + A, A"));
	}

	SECTION("foreach_bindings_search sequence") {
		controls.foreach_bindings_search(output.callback(), parse("A + S"), lc::search_mode::contains);
		CHECK(output.clear() == 1);

		controls.foreach_bindings_search(output.callback(), parse("A"), lc::search_mode::contains);
		CHECK(output.clear() == 4);

		controls.foreach_bindings_search(output.callback(), parse("A"), lc::search_mode::not_contains);
		CHECK(output.clear() == 1);

		controls.foreach_bindings_search(output.callback(), parse("Ctrl + A"), lc::search_mode::starts_with);
		CHECK(output.clear() == 3);

		controls.foreach_bindings_search(output.callback(), parse("LCtrl"), lc::search_mode::starts_with);
		CHECK(output.clear() == 4);

		controls.foreach_bindings_search(output.callback(), parse("LCtrl"), lc::search_mode::ends_with);
		CHECK(output.clear() == 3);
	}

	SECTION("foreach_bindings_search name") {
		controls.foreach_bindings_search(output.callback(), "test", lc::search_mode::contains);
		CHECK(output.clear() == 5);

		controls.foreach_bindings_search(output.callback(), "test2", lc::search_mode::contains);
		CHECK(output.clear() == 0);

		controls.foreach_bindings_search(output.callback(), "action", lc::search_mode::contains);
		CHECK(output.clear() == 2);

		controls.foreach_bindings_search(output.callback(), "action", lc::search_mode::ends_with);
		CHECK(output.clear() == 2);

		controls.foreach_bindings_search(output.callback(), "action", lc::search_mode::not_contains);
		CHECK(output.clear() == 3);
	}
}
