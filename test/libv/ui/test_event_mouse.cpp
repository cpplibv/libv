// File: %<%NAME%>%.%<%EXTENSION%>%, Created on %<%DATE%>% %<%TIME%>%, Author: %<%USER%>%

// hpp
#include <catch/catch.hpp>
// ext
#include <range/v3/algorithm/equal.hpp>
// std
#include <utility>
// pro
//#include <libv/ui/event/event_mouse.hpp>
#include <libv/ui/event/mouse_interest.hpp>
#include <libv/ui/event/mouse_table.hpp>
#include <libv/ui/event/mouse_watcher.hpp>


// =================================================================================================
#include <iostream>
#include <libv/log/log.hpp>
TEST_CASE("enable log", "[libv.ui.event.mouse]") { std::cout << libv::logger; }
// =================================================================================================

// -------------------------------------------------------------------------------------------------

struct CallCounter {
	int call_count = 0;

	template <typename... Args>
	int operator()(Args&&...) {
		return ++call_count;
	}
};

TEST_CASE("mouse subscribe and unsubscribe watcher test", "[libv.ui.event.mouse]") {
	libv::ui::MouseTable table;

	CallCounter callback0;
	libv::ui::MouseWatcher watcher0; watcher0.callback = std::ref(callback0);
	table.subscribe(watcher0, libv::ui::MouseInterest::mask_mouse, {0, 0}, {10, 10}, libv::ui::MouseOrder{0});

	table.event_button(libv::input::Mouse::Left, libv::input::Action::press);
	CHECK(callback0.call_count == 1);

	table.event_position(libv::vec2f{0, 0});
	CHECK(callback0.call_count == 2);

	table.event_scroll(libv::vec2f{15, 0});
	CHECK(callback0.call_count == 3);

	table.unsubscribe(watcher0);

	table.event_button(libv::input::Mouse::Left, libv::input::Action::press);
	table.event_position(libv::vec2f{0, 0});
	table.event_scroll(libv::vec2f{15, 0});
	CHECK(callback0.call_count == 3);
}

TEST_CASE("mouse multiple sub, notify with correct order", "[libv.ui.event.mouse]") {
	std::vector<int> call_order;
	libv::ui::MouseTable table;

	libv::ui::MouseWatcher watcher0; watcher0.callback = [&](auto&) { call_order.emplace_back(0); };
	table.subscribe(watcher0, libv::ui::MouseInterest::mask_mouse, {0, 0}, {10, 10}, libv::ui::MouseOrder{0});

	libv::ui::MouseWatcher watcher2; watcher2.callback = [&](auto&) { call_order.emplace_back(2); };
	table.subscribe(watcher2, libv::ui::MouseInterest::mask_mouse, {0, 0}, {10, 10}, libv::ui::MouseOrder{2});

	libv::ui::MouseWatcher watcher1; watcher1.callback = [&](auto&) { call_order.emplace_back(1); };
	table.subscribe(watcher1, libv::ui::MouseInterest::mask_mouse, {0, 0}, {10, 10}, libv::ui::MouseOrder{1});

	table.event_button(libv::input::Mouse::Left, libv::input::Action::press);
	CHECK(ranges::equal(call_order, std::array{2, 1, 0}));

	table.event_position(libv::vec2f{0, 0});
	CHECK(ranges::equal(call_order, std::array{2, 1, 0, 2, 1, 0}));

	table.event_scroll(libv::vec2f{15, 0});
	CHECK(ranges::equal(call_order, std::array{2, 1, 0, 2, 1, 0, 2, 1, 0}));

	table.unsubscribe(watcher1);

	table.event_button(libv::input::Mouse::Left, libv::input::Action::press);
	table.event_position(libv::vec2f{0, 0});
	table.event_scroll(libv::vec2f{15, 0});

	CHECK(ranges::equal(call_order, std::array{2, 1, 0, 2, 1, 0, 2, 1, 0, 2, 0, 2, 0, 2, 0}));

	table.unsubscribe(watcher0);
	table.unsubscribe(watcher2);
}

TEST_CASE("mouse multiple sub, only notify intersected ones", "[libv.ui.event.mouse]") {
	libv::ui::MouseTable table;

	CallCounter callback0;
	libv::ui::MouseWatcher watcher0; watcher0.callback = std::ref(callback0);
	table.subscribe(watcher0, libv::ui::MouseInterest::mask_mouse, {0, 0}, {10, 10}, libv::ui::MouseOrder{0});

	CallCounter callback1;
	libv::ui::MouseWatcher watcher1; watcher1.callback = std::ref(callback1);
	table.subscribe(watcher1, libv::ui::MouseInterest::mask_mouse, {5, 5}, {10, 10}, libv::ui::MouseOrder{0});

	table.event_position(libv::vec2f{0, 0});
	table.event_button(libv::input::Mouse::Left, libv::input::Action::press);
	table.event_scroll(libv::vec2f{15, 0});

	CHECK(callback0.call_count == 3);
	CHECK(callback1.call_count == 0);

	table.event_position(libv::vec2f{5, 5});
	table.event_button(libv::input::Mouse::Left, libv::input::Action::press);
	table.event_scroll(libv::vec2f{15, 0});

	CHECK(callback0.call_count == 6);
	CHECK(callback1.call_count == 3);

	table.event_position(libv::vec2f{10, 10});
	table.event_button(libv::input::Mouse::Left, libv::input::Action::press);
	table.event_scroll(libv::vec2f{15, 0});

	CHECK(callback0.call_count == 7);
	CHECK(callback1.call_count == 6);

	table.unsubscribe(watcher0);
	table.unsubscribe(watcher1);
}

TEST_CASE("mouse never intersect region", "[libv.ui.event.mouse]") {
	libv::ui::MouseTable table;
	table.event_position(libv::vec2f{0, 0});

	CallCounter callback0;
	libv::ui::MouseWatcher watcher0; watcher0.callback = std::ref(callback0);

	SECTION("Should not intersect zero sized region") {
		table.subscribe(watcher0, libv::ui::MouseInterest::mask_mouse, {5, 5}, {0, 0}, libv::ui::MouseOrder{0});

		table.event_position(libv::vec2f{4, 4});
		CHECK(callback0.call_count == 0);

		table.event_position(libv::vec2f{5, 5});
		CHECK(callback0.call_count == 0);

		table.event_position(libv::vec2f{6, 6});
		CHECK(callback0.call_count == 0);
	}

	SECTION("Should intersect one sized region") {
		table.subscribe(watcher0, libv::ui::MouseInterest::mask_mouse, {5, 5}, {1, 1}, libv::ui::MouseOrder{0});

		table.event_position(libv::vec2f{4, 4});
		CHECK(callback0.call_count == 0);

		table.event_position(libv::vec2f{5, 5});
		CHECK(callback0.call_count == 1);

		table.event_position(libv::vec2f{6, 6});
		CHECK(callback0.call_count == 2);

		table.event_position(libv::vec2f{7, 7});
		CHECK(callback0.call_count == 2);
	}

	SECTION("Should intersect two sized region") {
		table.subscribe(watcher0, libv::ui::MouseInterest::mask_mouse, {5, 5}, {2, 2}, libv::ui::MouseOrder{0});

		table.event_position(libv::vec2f{4, 4});
		CHECK(callback0.call_count == 0);

		table.event_position(libv::vec2f{5, 5});
		CHECK(callback0.call_count == 1);

		table.event_position(libv::vec2f{6, 6});
		CHECK(callback0.call_count == 2);

		table.event_position(libv::vec2f{7, 7});
		CHECK(callback0.call_count == 3);

		table.event_position(libv::vec2f{8, 8});
		CHECK(callback0.call_count == 3);
	}
}

TEST_CASE("mouse new subscribe should receive event from enter if created on top of mouse", "[libv.ui.event.mouse]") {
	libv::ui::MouseTable table;
	table.event_position(libv::vec2f{0, 0});

	CallCounter callback0;
	libv::ui::MouseWatcher watcher0; watcher0.callback = std::ref(callback0);

	table.subscribe(watcher0, libv::ui::MouseInterest::mask_mouse, {0, 0}, {5, 5}, libv::ui::MouseOrder{0});
	CHECK(callback0.call_count == 0);
	table.event_update();
	CHECK(callback0.call_count == 1);
}

TEST_CASE("mouse update", "[libv.ui.event.mouse]") {
	libv::ui::MouseTable table;

	CallCounter callback0;
	libv::ui::MouseWatcher watcher0; watcher0.callback = std::ref(callback0);
	CallCounter callback1;
	libv::ui::MouseWatcher watcher1; watcher1.callback = std::ref(callback1);
	table.event_position(libv::vec2f{0, 0});

	SECTION("simple moving watcher") {
		table.subscribe(watcher0, libv::ui::MouseInterest::mask_mouse, {0, 0}, {10, 10}, libv::ui::MouseOrder{0});
		CHECK(callback0.call_count == 0);
		table.event_update();
		CHECK(callback0.call_count == 1);

		table.update(watcher0, {5, 5}, {10, 10}, libv::ui::MouseOrder{0});
		CHECK(callback0.call_count == 1);
		table.event_update();
		CHECK(callback0.call_count == 2);

		table.update(watcher0, {5, 5}, {10, 10}, libv::ui::MouseOrder{0});
		CHECK(callback0.call_count == 2);
		table.event_update();
		CHECK(callback0.call_count == 2);

		table.update(watcher0, {0, 0}, {10, 10}, libv::ui::MouseOrder{0});
		CHECK(callback0.call_count == 2);
		table.event_update();
		CHECK(callback0.call_count == 3);
	}

	SECTION("two staged moving watcher") {
		table.subscribe(watcher0, libv::ui::MouseInterest::mask_mouse, {0, 0}, {10, 10}, libv::ui::MouseOrder{0});
		CHECK(callback0.call_count == 0);
		CHECK(callback1.call_count == 0);
		table.event_update();
		CHECK(callback0.call_count == 1);
		CHECK(callback1.call_count == 0);

		table.subscribe(watcher1, libv::ui::MouseInterest::mask_mouse, {0, 0}, {10, 10}, libv::ui::MouseOrder{1});
		CHECK(callback0.call_count == 1);
		CHECK(callback1.call_count == 0);
		table.event_update();
		CHECK(callback0.call_count == 1);
		CHECK(callback1.call_count == 1);

		table.update(watcher0, {5, 5}, {10, 10}, libv::ui::MouseOrder{0});
		CHECK(callback0.call_count == 1);
		CHECK(callback1.call_count == 1);
		table.event_update();
		CHECK(callback0.call_count == 2);
		CHECK(callback1.call_count == 1);

		table.update(watcher1, {5, 5}, {10, 10}, libv::ui::MouseOrder{0});
		CHECK(callback0.call_count == 2);
		CHECK(callback1.call_count == 1);
		table.event_update();
		CHECK(callback0.call_count == 2);
		CHECK(callback1.call_count == 2);

		table.update(watcher0, {5, 5}, {10, 10}, libv::ui::MouseOrder{0});
		CHECK(callback0.call_count == 2);
		CHECK(callback1.call_count == 2);
		table.event_update();
		CHECK(callback0.call_count == 2);
		CHECK(callback1.call_count == 2);

		table.update(watcher0, {0, 0}, {10, 10}, libv::ui::MouseOrder{0});
		CHECK(callback0.call_count == 2);
		CHECK(callback1.call_count == 2);
		table.event_update();
		CHECK(callback0.call_count == 3);
		CHECK(callback1.call_count == 2);
	}
}

TEST_CASE("mouse leave", "[libv.ui.event.mouse]") {
	libv::ui::MouseTable table;

	CallCounter callback0;
	libv::ui::MouseWatcher watcher0; watcher0.callback = std::ref(callback0);
	table.event_position(libv::vec2f{0, 0});

	table.subscribe(watcher0, libv::ui::MouseInterest::mask_mouse, {0, 0}, {10, 10}, libv::ui::MouseOrder{0});
	CHECK(callback0.call_count == 0);
	table.event_update();
	CHECK(callback0.call_count == 1);
	table.event_leave();
	CHECK(callback0.call_count == 2);

	table.event_position(libv::vec2f{15, 15});
	CHECK(callback0.call_count == 2);
}

TEST_CASE("mouse absorb event from lower orders", "[libv.ui.event.mouse]") {

}

TEST_CASE("mouse absorb event for same orders still broadcast", "[libv.ui.event.mouse]") {

}

// -------------------------------------------------------------------------------------------------
