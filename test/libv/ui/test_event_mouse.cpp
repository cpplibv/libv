// Project: libv.ui, File: test/libv/ui/test_event_mouse.cpp

// hpp
#include <catch/catch.hpp>
// ext
#include <range/v3/algorithm/equal.hpp>
// std
#include <functional>
#include <utility>
// pro
#include <libv/ui/component/detail/core_component.hpp>
#include <libv/ui/context/context_mouse.hpp>
#include <libv/ui/event/event_mouse.hpp>


// -------------------------------------------------------------------------------------------------

struct TestCoreComponent : public libv::ui::CoreComponent {
	int num_event = 0;

	int num_button = 0;
	int num_movement = 0;
	int num_scroll = 0;

	std::function<void(const libv::ui::EventMouseButton&)> cb_button;
	std::function<void(const libv::ui::EventMouseMovement&)> cb_movement;
	std::function<void(const libv::ui::EventMouseScroll&)> cb_scroll;

	bool pass_through = true;

	explicit TestCoreComponent(std::string name) :
		CoreComponent(std::move(name)) {
	}

	virtual void onMouseButton(const libv::ui::EventMouseButton& event) override {
		num_event++;
		num_button++;
		if (cb_button)
			cb_button(event);
	}

	virtual void onMouseMovement(const libv::ui::EventMouseMovement& event) override {
		num_event++;
		num_movement++;
		if (cb_movement)
			cb_movement(event);
		if (pass_through)
			event.pass_through();
	}

	virtual void onMouseScroll(const libv::ui::EventMouseScroll& event) override {
		num_event++;
		num_scroll++;
		if (cb_scroll)
			cb_scroll(event);
	}
};

void subscribe(libv::ui::ContextMouse& table, libv::ui::CoreComponent& component, libv::vec3f position, libv::vec3f size, libv::ui::MouseOrder order) {
	table.subscribe(component);
	table.update(component, position, size, order);
}

// -------------------------------------------------------------------------------------------------
// TODO P5: Test scroll area (float region)

TEST_CASE("mouse subscribe and unsubscribe watcher test", "[libv.ui.event.mouse]") {
	libv::ui::ContextMouse table;

	TestCoreComponent watcher0("watcher0");
	subscribe(table, watcher0, {0, 0, 0}, {10, 10, 0}, libv::ui::MouseOrder{0});

	table.event_update(); // Register the new subscriptions
	CHECK(watcher0.num_event == 1);

	table.event_button(libv::input::MouseButton::Left, libv::input::Action::press);
	CHECK(watcher0.num_event == 2);

	table.event_position(libv::vec2f{0, 0});
	CHECK(watcher0.num_event == 3);

	table.event_scroll(libv::vec2f{15, 0});
	CHECK(watcher0.num_event == 4);

	table.unsubscribe(watcher0);

	table.event_button(libv::input::MouseButton::Left, libv::input::Action::press);
	table.event_position(libv::vec2f{0, 0});
	table.event_scroll(libv::vec2f{15, 0});
	table.event_update();
	CHECK(watcher0.num_event == 4);
}

TEST_CASE("mouse multiple sub, notify with correct order", "[libv.ui.event.mouse]") {
	std::vector<int> call_order;
	libv::ui::ContextMouse table;

	TestCoreComponent watcher0("watcher0");
	watcher0.cb_button = [&](const auto&) { call_order.emplace_back(100); };
	watcher0.cb_movement = [&](const auto&) { call_order.emplace_back(101); };
	watcher0.cb_scroll = [&](const auto&) { call_order.emplace_back(102); };
	watcher0.pass_through = true;
	subscribe(table, watcher0, {0, 0, 0}, {10, 10, 0}, libv::ui::MouseOrder{0});

	TestCoreComponent watcher2("watcher2");
	watcher2.cb_button = [&](const auto&) { call_order.emplace_back(120); };
	watcher2.cb_movement = [&](const auto&) { call_order.emplace_back(121); };
	watcher2.cb_scroll = [&](const auto&) { call_order.emplace_back(122); };
	watcher2.pass_through = true;
	subscribe(table, watcher2, {0, 0, 0}, {10, 10, 0}, libv::ui::MouseOrder{2});

	TestCoreComponent watcher1("watcher1");
	watcher1.cb_button = [&](const auto&) { call_order.emplace_back(110); };
	watcher1.cb_movement = [&](const auto&) { call_order.emplace_back(111); };
	watcher1.cb_scroll = [&](const auto&) { call_order.emplace_back(112); };
	watcher1.pass_through = true;
	subscribe(table, watcher1, {0, 0, 0}, {10, 10, 0}, libv::ui::MouseOrder{1});

	table.event_update(); // Register the new subscriptions
	CHECK(ranges::equal(call_order, std::array{121, 111, 101}));
	call_order.clear();

	table.event_button(libv::input::MouseButton::Left, libv::input::Action::press);
	CHECK(ranges::equal(call_order, std::array{120, 110, 100}));
	call_order.clear();

	table.event_position(libv::vec2f{0, 0});
	CHECK(ranges::equal(call_order, std::array{121, 111, 101}));
	call_order.clear();

	table.event_scroll(libv::vec2f{15, 0});
	CHECK(ranges::equal(call_order, std::array{122, 112, 102}));
	call_order.clear();

	table.unsubscribe(watcher1);

	table.event_button(libv::input::MouseButton::Left, libv::input::Action::press);
	table.event_position(libv::vec2f{0, 0});
	table.event_scroll(libv::vec2f{15, 0});

	CHECK(ranges::equal(call_order, std::array{120, 100, 121, 101, 122, 102}));
	call_order.clear();

	table.unsubscribe(watcher0);
	table.unsubscribe(watcher2);
}

TEST_CASE("mouse multiple sub, only notify intersected ones", "[libv.ui.event.mouse]") {
	libv::ui::ContextMouse table;

	TestCoreComponent watcher0("watcher0");
	subscribe(table, watcher0, {0, 0, 0}, {10, 10, 0}, libv::ui::MouseOrder{0});

	TestCoreComponent watcher1("watcher1");
	subscribe(table, watcher1, {5, 5, 0}, {10, 10, 0}, libv::ui::MouseOrder{0});

	table.event_update(); // Register the new subscriptions
	CHECK(watcher0.num_event == 1);
	CHECK(watcher1.num_event == 0);

	table.event_position(libv::vec2f{0, 0});
	table.event_button(libv::input::MouseButton::Left, libv::input::Action::press);
	table.event_scroll(libv::vec2f{15, 0});

	CHECK(watcher0.num_event == 4);
	CHECK(watcher1.num_event == 0);

	table.event_position(libv::vec2f{5, 5});
	table.event_button(libv::input::MouseButton::Left, libv::input::Action::press);
	table.event_scroll(libv::vec2f{15, 0});

	CHECK(watcher0.num_event == 7);
	CHECK(watcher1.num_event == 3);

	table.event_position(libv::vec2f{10, 10});
	table.event_button(libv::input::MouseButton::Left, libv::input::Action::press);
	table.event_scroll(libv::vec2f{15, 0});

	CHECK(watcher0.num_event == 8);
	CHECK(watcher1.num_event == 6);

	table.unsubscribe(watcher0);
	table.unsubscribe(watcher1);
}

TEST_CASE("mouse never intersect region", "[libv.ui.event.mouse]") {
	libv::ui::ContextMouse table;
	table.event_position(libv::vec2f{0, 0});

	TestCoreComponent watcher0("watcher0");

	SECTION("Should not intersect zero sized region") {
		subscribe(table, watcher0, {5, 5, 0}, {0, 0, 0}, libv::ui::MouseOrder{0});
		table.event_update(); // Register the new subscriptions

		table.event_position(libv::vec2f{4, 4});
		CHECK(watcher0.num_event == 0);

		table.event_position(libv::vec2f{5, 5});
		CHECK(watcher0.num_event == 0);

		table.event_position(libv::vec2f{6, 6});
		CHECK(watcher0.num_event == 0);
	}

	SECTION("Should intersect one sized region") {
		subscribe(table, watcher0, {5, 5, 0}, {1, 1, 0}, libv::ui::MouseOrder{0});
		table.event_update(); // Register the new subscriptions

		table.event_position(libv::vec2f{4, 4});
		CHECK(watcher0.num_event == 0);

		table.event_position(libv::vec2f{5, 5});
		CHECK(watcher0.num_event == 1);

		table.event_position(libv::vec2f{6, 6});
		CHECK(watcher0.num_event == 2);

		table.event_position(libv::vec2f{7, 7});
		CHECK(watcher0.num_event == 2);
	}

	SECTION("Should intersect two sized region") {
		subscribe(table, watcher0, {5, 5, 0}, {2, 2, 0}, libv::ui::MouseOrder{0});
		table.event_update(); // Register the new subscriptions

		table.event_position(libv::vec2f{4, 4});
		CHECK(watcher0.num_event == 0);

		table.event_position(libv::vec2f{5, 5});
		CHECK(watcher0.num_event == 1);

		table.event_position(libv::vec2f{6, 6});
		CHECK(watcher0.num_event == 2);

		table.event_position(libv::vec2f{7, 7});
		CHECK(watcher0.num_event == 3);

		table.event_position(libv::vec2f{8, 8});
		CHECK(watcher0.num_event == 3);
	}
}

TEST_CASE("mouse new subscribe should receive event from enter if created on top of mouse", "[libv.ui.event.mouse]") {
	libv::ui::ContextMouse table;
	table.event_position(libv::vec2f{0, 0});

	TestCoreComponent watcher0("watcher0");

	subscribe(table, watcher0, {0, 0, 0}, {5, 5, 0}, libv::ui::MouseOrder{0});
	CHECK(watcher0.num_event == 0);
	table.event_update(); // Register the new subscriptions
	CHECK(watcher0.num_event == 1);
}

TEST_CASE("mouse update", "[libv.ui.event.mouse]") {
	libv::ui::ContextMouse table;

	TestCoreComponent watcher0("watcher0");
	TestCoreComponent watcher1("watcher1");
	table.event_position(libv::vec2f{0, 0});

	SECTION("simple moving watcher") {
		subscribe(table, watcher0, {0, 0, 0}, {10, 10, 0}, libv::ui::MouseOrder{0});
		CHECK(watcher0.num_event == 0);
		table.event_update(); // Register the new subscriptions
		CHECK(watcher0.num_event == 1);

		table.update(watcher0, {5, 5, 0}, {10, 10, 0}, libv::ui::MouseOrder{0});
		CHECK(watcher0.num_event == 1);
		table.event_update();
		CHECK(watcher0.num_event == 2);

		table.update(watcher0, {5, 5, 0}, {10, 10, 0}, libv::ui::MouseOrder{0});
		CHECK(watcher0.num_event == 2);
		table.event_update();
		CHECK(watcher0.num_event == 2);

		table.update(watcher0, {0, 0, 0}, {10, 10, 0}, libv::ui::MouseOrder{0});
		CHECK(watcher0.num_event == 2);
		table.event_update();
		CHECK(watcher0.num_event == 3);

		CHECK(watcher1.num_event == 0);
	}

	SECTION("two staged moving watcher") {
		subscribe(table, watcher0, {0, 0, 0}, {10, 10, 0}, libv::ui::MouseOrder{0});
		CHECK(watcher0.num_event == 0);
		CHECK(watcher1.num_event == 0);
		table.event_update(); // Register the new subscriptions
		CHECK(watcher0.num_event == 1);
		CHECK(watcher1.num_event == 0);

		subscribe(table, watcher1, {0, 0, 0}, {10, 10, 0}, libv::ui::MouseOrder{1});
		CHECK(watcher0.num_event == 1);
		CHECK(watcher1.num_event == 0);
		table.event_update(); // Register the new subscriptions #2
		CHECK(watcher0.num_event == 1);
		CHECK(watcher1.num_event == 1);

		table.update(watcher0, {5, 5, 0}, {10, 10, 0}, libv::ui::MouseOrder{0});
		CHECK(watcher0.num_event == 1);
		CHECK(watcher1.num_event == 1);
		table.event_update();
		CHECK(watcher0.num_event == 2);
		CHECK(watcher1.num_event == 1);

		table.update(watcher1, {5, 5, 0}, {10, 10, 0}, libv::ui::MouseOrder{0});
		CHECK(watcher0.num_event == 2);
		CHECK(watcher1.num_event == 1);
		table.event_update();
		CHECK(watcher0.num_event == 2);
		CHECK(watcher1.num_event == 2);

		table.update(watcher0, {5, 5, 0}, {10, 10, 0}, libv::ui::MouseOrder{0});
		CHECK(watcher0.num_event == 2);
		CHECK(watcher1.num_event == 2);
		table.event_update();
		CHECK(watcher0.num_event == 2);
		CHECK(watcher1.num_event == 2);

		table.update(watcher0, {0, 0, 0}, {10, 10, 0}, libv::ui::MouseOrder{0});
		CHECK(watcher0.num_event == 2);
		CHECK(watcher1.num_event == 2);
		table.event_update();
		CHECK(watcher0.num_event == 3);
		CHECK(watcher1.num_event == 2);
	}
}

TEST_CASE("mouse leave", "[libv.ui.event.mouse]") {
	libv::ui::ContextMouse table;

	TestCoreComponent watcher0("watcher0");
	table.event_position(libv::vec2f{0, 0});

	subscribe(table, watcher0, {0, 0, 0}, {10, 10, 0}, libv::ui::MouseOrder{0});
	CHECK(watcher0.num_event == 0);
	table.event_update(); // Register the new subscriptions
	CHECK(watcher0.num_event == 1);
	table.event_leave();
	CHECK(watcher0.num_event == 2);

	table.event_position(libv::vec2f{15, 15});
	CHECK(watcher0.num_event == 2);
}

//TEST_CASE("mouse absorb event from lower orders", "[libv.ui.event.mouse]") {
//
//}
//
//TEST_CASE("mouse absorb event for same orders still broadcast", "[libv.ui.event.mouse]") {
//
//}

// -------------------------------------------------------------------------------------------------
