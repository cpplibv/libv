// Project: libv.ui, File: test/libv/ui/test_event_mouse.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <catch/catch.hpp>
// ext
#include <range/v3/algorithm/equal.hpp>
// std
#include <utility>
// pro
#include <libv/ui/context/context_mouse.hpp>
#include <libv/ui/event/mouse_watcher.hpp>
#include <libv/ui/flag.hpp>


// -------------------------------------------------------------------------------------------------

struct CallCounter {
	int call_count = 0;

	template <typename... Args>
	int operator()(Args&&...) {
		return ++call_count;
	}
};

TEST_CASE("mouse subscribe and unsubscribe watcher test", "[libv.ui.event.mouse]") {
	libv::ui::ContextMouse table;

	CallCounter callback0;
	libv::ui::MouseWatcher watcher0;
	watcher0.cb_button = std::ref(callback0);
	watcher0.cb_movement = std::ref(callback0);
	watcher0.cb_scroll = std::ref(callback0);
	table.subscribe(watcher0, libv::ui::Flag::mask_watchMouse, {0, 0}, {10, 10}, libv::ui::MouseOrder{0});

	table.event_button(libv::input::MouseButton::Left, libv::input::Action::press);
	CHECK(callback0.call_count == 1);

	table.event_position(libv::vec2f{0, 0});
	CHECK(callback0.call_count == 2);

	table.event_scroll(libv::vec2f{15, 0});
	CHECK(callback0.call_count == 3);

	table.unsubscribe(watcher0);

	table.event_button(libv::input::MouseButton::Left, libv::input::Action::press);
	table.event_position(libv::vec2f{0, 0});
	table.event_scroll(libv::vec2f{15, 0});
	CHECK(callback0.call_count == 3);
}

TEST_CASE("mouse multiple sub, notify with correct order", "[libv.ui.event.mouse]") {
	std::vector<int> call_order;
	libv::ui::ContextMouse table;

	libv::ui::MouseWatcher watcher0;
	watcher0.cb_button = [&](auto&) { call_order.emplace_back(0); };
	watcher0.cb_movement = [&](auto&) { call_order.emplace_back(0); };
	watcher0.cb_scroll = [&](auto&) { call_order.emplace_back(0); };
	table.subscribe(watcher0, libv::ui::Flag::mask_watchMouse, {0, 0}, {10, 10}, libv::ui::MouseOrder{0});

	libv::ui::MouseWatcher watcher2;
	watcher2.cb_button = [&](auto&) { call_order.emplace_back(2); };
	watcher2.cb_movement = [&](auto&) { call_order.emplace_back(2); };
	watcher2.cb_scroll = [&](auto&) { call_order.emplace_back(2); };
	table.subscribe(watcher2, libv::ui::Flag::mask_watchMouse, {0, 0}, {10, 10}, libv::ui::MouseOrder{2});

	libv::ui::MouseWatcher watcher1;
	watcher1.cb_button = [&](auto&) { call_order.emplace_back(1); };
	watcher1.cb_movement = [&](auto&) { call_order.emplace_back(1); };
	watcher1.cb_scroll = [&](auto&) { call_order.emplace_back(1); };
	table.subscribe(watcher1, libv::ui::Flag::mask_watchMouse, {0, 0}, {10, 10}, libv::ui::MouseOrder{1});

	table.event_button(libv::input::MouseButton::Left, libv::input::Action::press);
	CHECK(ranges::equal(call_order, std::array{2, 1, 0}));

	table.event_position(libv::vec2f{0, 0});
	CHECK(ranges::equal(call_order, std::array{2, 1, 0, 2, 1, 0}));

	table.event_scroll(libv::vec2f{15, 0});
	CHECK(ranges::equal(call_order, std::array{2, 1, 0, 2, 1, 0, 2, 1, 0}));

	table.unsubscribe(watcher1);

	table.event_button(libv::input::MouseButton::Left, libv::input::Action::press);
	table.event_position(libv::vec2f{0, 0});
	table.event_scroll(libv::vec2f{15, 0});

	CHECK(ranges::equal(call_order, std::array{2, 1, 0, 2, 1, 0, 2, 1, 0, 2, 0, 2, 0, 2, 0}));

	table.unsubscribe(watcher0);
	table.unsubscribe(watcher2);
}

TEST_CASE("mouse multiple sub, only notify intersected ones", "[libv.ui.event.mouse]") {
	libv::ui::ContextMouse table;

	CallCounter callback0;
	libv::ui::MouseWatcher watcher0;
	watcher0.cb_button = std::ref(callback0);
	watcher0.cb_movement = std::ref(callback0);
	watcher0.cb_scroll = std::ref(callback0);
	table.subscribe(watcher0, libv::ui::Flag::mask_watchMouse, {0, 0}, {10, 10}, libv::ui::MouseOrder{0});

	CallCounter callback1;
	libv::ui::MouseWatcher watcher1;
	watcher1.cb_button = std::ref(callback1);
	watcher1.cb_movement = std::ref(callback1);
	watcher1.cb_scroll = std::ref(callback1);
	table.subscribe(watcher1, libv::ui::Flag::mask_watchMouse, {5, 5}, {10, 10}, libv::ui::MouseOrder{0});

	table.event_position(libv::vec2f{0, 0});
	table.event_button(libv::input::MouseButton::Left, libv::input::Action::press);
	table.event_scroll(libv::vec2f{15, 0});

	CHECK(callback0.call_count == 3);
	CHECK(callback1.call_count == 0);

	table.event_position(libv::vec2f{5, 5});
	table.event_button(libv::input::MouseButton::Left, libv::input::Action::press);
	table.event_scroll(libv::vec2f{15, 0});

	CHECK(callback0.call_count == 6);
	CHECK(callback1.call_count == 3);

	table.event_position(libv::vec2f{10, 10});
	table.event_button(libv::input::MouseButton::Left, libv::input::Action::press);
	table.event_scroll(libv::vec2f{15, 0});

	CHECK(callback0.call_count == 7);
	CHECK(callback1.call_count == 6);

	table.unsubscribe(watcher0);
	table.unsubscribe(watcher1);
}

TEST_CASE("mouse never intersect region", "[libv.ui.event.mouse]") {
	libv::ui::ContextMouse table;
	table.event_position(libv::vec2f{0, 0});

	CallCounter callback0;
	libv::ui::MouseWatcher watcher0;
	watcher0.cb_button = std::ref(callback0);
	watcher0.cb_movement = std::ref(callback0);
	watcher0.cb_scroll = std::ref(callback0);

	SECTION("Should not intersect zero sized region") {
		table.subscribe(watcher0, libv::ui::Flag::mask_watchMouse, {5, 5}, {0, 0}, libv::ui::MouseOrder{0});

		table.event_position(libv::vec2f{4, 4});
		CHECK(callback0.call_count == 0);

		table.event_position(libv::vec2f{5, 5});
		CHECK(callback0.call_count == 0);

		table.event_position(libv::vec2f{6, 6});
		CHECK(callback0.call_count == 0);
	}

	SECTION("Should intersect one sized region") {
		table.subscribe(watcher0, libv::ui::Flag::mask_watchMouse, {5, 5}, {1, 1}, libv::ui::MouseOrder{0});

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
		table.subscribe(watcher0, libv::ui::Flag::mask_watchMouse, {5, 5}, {2, 2}, libv::ui::MouseOrder{0});

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
	libv::ui::ContextMouse table;
	table.event_position(libv::vec2f{0, 0});

	CallCounter callback0;
	libv::ui::MouseWatcher watcher0;
	watcher0.cb_button = std::ref(callback0);
	watcher0.cb_movement = std::ref(callback0);
	watcher0.cb_scroll = std::ref(callback0);

	table.subscribe(watcher0, libv::ui::Flag::mask_watchMouse, {0, 0}, {5, 5}, libv::ui::MouseOrder{0});
	CHECK(callback0.call_count == 0);
	table.event_update();
	CHECK(callback0.call_count == 1);
}

TEST_CASE("mouse update", "[libv.ui.event.mouse]") {
	libv::ui::ContextMouse table;

	CallCounter callback0;
	libv::ui::MouseWatcher watcher0;
	watcher0.cb_button = std::ref(callback0);
	watcher0.cb_movement = std::ref(callback0);
	watcher0.cb_scroll = std::ref(callback0);
	CallCounter callback1;
	libv::ui::MouseWatcher watcher1;
	watcher1.cb_button = std::ref(callback1);
	watcher1.cb_movement = std::ref(callback1);
	watcher1.cb_scroll = std::ref(callback1);
	table.event_position(libv::vec2f{0, 0});

	SECTION("simple moving watcher") {
		table.subscribe(watcher0, libv::ui::Flag::mask_watchMouse, {0, 0}, {10, 10}, libv::ui::MouseOrder{0});
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
		table.subscribe(watcher0, libv::ui::Flag::mask_watchMouse, {0, 0}, {10, 10}, libv::ui::MouseOrder{0});
		CHECK(callback0.call_count == 0);
		CHECK(callback1.call_count == 0);
		table.event_update();
		CHECK(callback0.call_count == 1);
		CHECK(callback1.call_count == 0);

		table.subscribe(watcher1, libv::ui::Flag::mask_watchMouse, {0, 0}, {10, 10}, libv::ui::MouseOrder{1});
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
	libv::ui::ContextMouse table;

	CallCounter callback0;
	libv::ui::MouseWatcher watcher0;
	watcher0.cb_button = std::ref(callback0);
	watcher0.cb_movement = std::ref(callback0);
	watcher0.cb_scroll = std::ref(callback0);
	table.event_position(libv::vec2f{0, 0});

	table.subscribe(watcher0, libv::ui::Flag::mask_watchMouse, {0, 0}, {10, 10}, libv::ui::MouseOrder{0});
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
