// Project: libv.ui, File: test/libv/ui/test_event_mouse.cpp

// hpp
#include <catch2/catch_test_macros.hpp>
// ext
#include <range/v3/algorithm/equal.hpp>
// std
#include <functional>
#include <iostream>
#include <utility>
// pro
#include <libv/ui/component/component_core.hpp>
#include <libv/ui/context/context_mouse.hpp>
#include <libv/ui/context/context_ui_link.hpp>
#include <libv/ui/event/event_mouse.hpp>
#include <libv/ui/context/context_ui.hpp>
#include <libv/ui/ui.hpp>
#include <libv/log/log.hpp>


// -------------------------------------------------------------------------------------------------

// Hacky test context: UI is created to have a ContextUI but an independent ContextMouse is used during testing
class TestContextMouse : libv::ui::UI, public libv::ui::ContextMouse {
public:
	using libv::ui::ContextMouse::update; // Disambiguate UI.update and ContextMouse.update
};

enum class PassThrough {
	on,
	off,
};

enum class Interaction {
	enable,
	disable,
};

struct TestCoreComponent : public libv::ui::CoreComponent {
	int num_event = 0;

	int num_button = 0;
	int num_movement = 0;
	int num_scroll = 0;

	std::function<void(const libv::ui::EventMouseButton&)> cb_button;
	std::function<void(const libv::ui::EventMouseMovement&)> cb_movement;
	std::function<void(const libv::ui::EventMouseScroll&)> cb_scroll;

	bool pass_through = true;

	TestCoreComponent(std::string name, PassThrough pass_through, Interaction interaction) :
			CoreComponent(std::move(name)),
			pass_through(pass_through == PassThrough::on) {

		if (interaction == Interaction::disable)
			enable(false);
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

void subscribe(TestContextMouse& ctx, libv::ui::CoreComponent& component, libv::vec2f position, libv::vec2f size, libv::ui::MouseOrder order) {
	ctx.libv::ui::ContextMouse::subscribe(component);
	ctx.update(component, position, size, order);
	ctx.enable(component, component.enable());
}

// =================================================================================================

// Hacky test context: UI is created to have a ContextUI but an independent ContextMouse is used during testing
class TestContextMouse2 : libv::ui::UI, public libv::ui::ContextMouse {
	friend class TestCoreComponent2;

private:
	std::string event_history;

public:
	using libv::ui::ContextMouse::update; // Disambiguate UI.update and ContextMouse.update

	[[nodiscard]] std::string history() {
		if (event_history.ends_with(", "))
			event_history.erase(event_history.end() - 2, event_history.end());

		auto result = std::move(event_history);
		event_history.clear();
		return result;
	}
};

class TestCoreComponent2 : public libv::ui::CoreComponent {
private:
	TestContextMouse2& ctx;
	bool pass_through = true;

public:
	TestCoreComponent2(TestContextMouse2& ctx, std::string name, PassThrough pass_through, Interaction interaction) :
			CoreComponent(std::move(name)),
			ctx(ctx),
			pass_through(pass_through == PassThrough::on) {

		if (interaction == Interaction::disable)
			enable(false);
	}

	virtual void onMouseButton(const libv::ui::EventMouseButton& event) override {
		ctx.event_history += name + ":B, ";
		(void) event;
	}

	virtual void onMouseMovement(const libv::ui::EventMouseMovement& event) override {
		ctx.event_history += name + ":"
				+ (event.mouse_movement.length() > 0.01f ? "M" : "")
				+ (event.enter ? "E" : "")
				+ (event.leave ? "L" : "")
				+ (event.mouse_movement.length() <= 0.01f && !event.enter && !event.leave ? "N" : "")
				+ ", ";
		if (pass_through)
			event.pass_through();
	}

	virtual void onMouseScroll(const libv::ui::EventMouseScroll& event) override {
		ctx.event_history += name + ":S, ";
		(void) event;
	}

	void subscribe(libv::vec2f position, libv::vec2f size, libv::ui::MouseOrder order) {
		ctx.libv::ui::ContextMouse::subscribe(*this);
		ctx.update(*this, position, size, order);
		ctx.enable(*this, enable());
	}

	void update(Interaction interaction) {
		enable(interaction != Interaction::disable);
		ctx.enable(*this, enable());
	}

	void update(PassThrough pass_through) {
		this->pass_through = pass_through == PassThrough::on;
	}

	void update(PassThrough pass_through, Interaction interaction) {
		update(pass_through);
		update(interaction);
	}
};

// =================================================================================================
// TODO P5: Test scroll area (float region)

//TEST_CASE("AAA libv.ui.event.mouse log", "[libv.ui.event.mouse]") {
//	std::cout << libv::logger_stream;
//}

TEST_CASE("Mouse: subscribe and unsubscribe watcher test", "[libv.ui.event.mouse]") {
	TestContextMouse table;

	TestCoreComponent watcher0("watcher0", PassThrough::on, Interaction::enable);
	subscribe(table, watcher0, {0, 0}, {10, 10}, libv::ui::MouseOrder{0});

	table.event_update_layout(); // Register the new subscriptions
	CHECK(watcher0.num_event == 1);

	table.event_button(libv::input::MouseButton::Left, libv::input::Action::press);
	CHECK(watcher0.num_event == 2);

	table.event_position({0, 0});
	CHECK(watcher0.num_event == 3);

	table.event_scroll(libv::vec2f{15, 0});
	CHECK(watcher0.num_event == 4);

	table.unsubscribe(watcher0);

	table.event_button(libv::input::MouseButton::Left, libv::input::Action::press);
	table.event_position({0, 0});
	table.event_scroll(libv::vec2f{15, 0});
	table.event_update_layout();
	CHECK(watcher0.num_event == 4);
}

TEST_CASE("Mouse: multiple sub, notify with correct order", "[libv.ui.event.mouse]") {
	std::vector<int> call_order;
	TestContextMouse table;

	TestCoreComponent watcher0("watcher0", PassThrough::on, Interaction::enable);
	watcher0.cb_button = [&](const auto&) { call_order.emplace_back(100); };
	watcher0.cb_movement = [&](const auto&) { call_order.emplace_back(101); };
	watcher0.cb_scroll = [&](const auto&) { call_order.emplace_back(102); };
	subscribe(table, watcher0, {0, 0}, {10, 10}, libv::ui::MouseOrder{0});

	TestCoreComponent watcher2("watcher2", PassThrough::on, Interaction::enable);
	watcher2.cb_button = [&](const auto&) { call_order.emplace_back(120); };
	watcher2.cb_movement = [&](const auto&) { call_order.emplace_back(121); };
	watcher2.cb_scroll = [&](const auto&) { call_order.emplace_back(122); };
	subscribe(table, watcher2, {0, 0}, {10, 10}, libv::ui::MouseOrder{2});

	TestCoreComponent watcher1("watcher1", PassThrough::on, Interaction::enable);
	watcher1.cb_button = [&](const auto&) { call_order.emplace_back(110); };
	watcher1.cb_movement = [&](const auto&) { call_order.emplace_back(111); };
	watcher1.cb_scroll = [&](const auto&) { call_order.emplace_back(112); };
	subscribe(table, watcher1, {0, 0}, {10, 10}, libv::ui::MouseOrder{1});

	table.event_update_layout(); // Register the new subscriptions
	CHECK(ranges::equal(call_order, std::array{121, 111, 101}));
	call_order.clear();

	table.event_button(libv::input::MouseButton::Left, libv::input::Action::press);
	CHECK(ranges::equal(call_order, std::array{120, 110, 100}));
	call_order.clear();

	table.event_position({0, 0});
	CHECK(ranges::equal(call_order, std::array{121, 111, 101}));
	call_order.clear();

	table.event_scroll(libv::vec2f{15, 0});
	CHECK(ranges::equal(call_order, std::array{122, 112, 102}));
	call_order.clear();

	table.unsubscribe(watcher1);

	table.event_button(libv::input::MouseButton::Left, libv::input::Action::press);
	table.event_position({0, 0});
	table.event_scroll(libv::vec2f{15, 0});

	CHECK(ranges::equal(call_order, std::array{120, 100, 121, 101, 122, 102}));
	call_order.clear();

	table.unsubscribe(watcher0);
	table.unsubscribe(watcher2);
}

TEST_CASE("Mouse: multiple sub, only notify intersected ones", "[libv.ui.event.mouse]") {
	TestContextMouse table;

	TestCoreComponent watcher0("watcher0", PassThrough::on, Interaction::enable);
	subscribe(table, watcher0, {0, 0}, {10, 10}, libv::ui::MouseOrder{0});

	TestCoreComponent watcher1("watcher1", PassThrough::on, Interaction::enable);
	subscribe(table, watcher1, {5, 5}, {10, 10}, libv::ui::MouseOrder{0});

	table.event_update_layout(); // Register the new subscriptions
	CHECK(watcher0.num_event == 1);
	CHECK(watcher1.num_event == 0);

	table.event_position({0, 0});
	table.event_button(libv::input::MouseButton::Left, libv::input::Action::press);
	table.event_scroll(libv::vec2f{15, 0});

	CHECK(watcher0.num_event == 4);
	CHECK(watcher1.num_event == 0);

	table.event_position({5, 5});
	table.event_button(libv::input::MouseButton::Left, libv::input::Action::press);
	table.event_scroll(libv::vec2f{15, 0});

	CHECK(watcher0.num_event == 7);
	CHECK(watcher1.num_event == 3);

	table.event_position({10, 10});
	table.event_button(libv::input::MouseButton::Left, libv::input::Action::press);
	table.event_scroll(libv::vec2f{15, 0});

	CHECK(watcher0.num_event == 8);
	CHECK(watcher1.num_event == 6);

	table.unsubscribe(watcher0);
	table.unsubscribe(watcher1);
}

TEST_CASE("Mouse: never intersect region", "[libv.ui.event.mouse]") {
	TestContextMouse table;
	table.event_position({0, 0});

	TestCoreComponent watcher0("watcher0", PassThrough::on, Interaction::enable);

	SECTION("Should not intersect zero sized region") {
		subscribe(table, watcher0, {5, 5}, {0, 0}, libv::ui::MouseOrder{0});
		table.event_update_layout(); // Register the new subscriptions

		table.event_position({4, 4});
		CHECK(watcher0.num_event == 0);

		table.event_position({5, 5});
		CHECK(watcher0.num_event == 0);

		table.event_position({6, 6});
		CHECK(watcher0.num_event == 0);
	}

	SECTION("Should intersect one sized region") {
		subscribe(table, watcher0, {5, 5}, {1, 1}, libv::ui::MouseOrder{0});
		table.event_update_layout(); // Register the new subscriptions

		table.event_position({4, 4});
		CHECK(watcher0.num_event == 0);

		table.event_position({5, 5});
		CHECK(watcher0.num_event == 1);

		table.event_position({6, 6});
		CHECK(watcher0.num_event == 2);

		table.event_position({7, 7});
		CHECK(watcher0.num_event == 2);
	}

	SECTION("Should intersect two sized region") {
		subscribe(table, watcher0, {5, 5}, {2, 2}, libv::ui::MouseOrder{0});
		table.event_update_layout(); // Register the new subscriptions

		table.event_position({4, 4});
		CHECK(watcher0.num_event == 0);

		table.event_position({5, 5});
		CHECK(watcher0.num_event == 1);

		table.event_position({6, 6});
		CHECK(watcher0.num_event == 2);

		table.event_position({7, 7});
		CHECK(watcher0.num_event == 3);

		table.event_position({8, 8});
		CHECK(watcher0.num_event == 3);
	}
}

TEST_CASE("Mouse: new subscribe receives event from enter if created on top of mouse", "[libv.ui.event.mouse]") {
	TestContextMouse table;
	table.event_position({0, 0});

	TestCoreComponent watcher0("watcher0", PassThrough::on, Interaction::enable);

	subscribe(table, watcher0, {0, 0}, {5, 5}, libv::ui::MouseOrder{0});
	CHECK(watcher0.num_event == 0);
	table.event_update_layout(); // Register the new subscriptions
	CHECK(watcher0.num_event == 1);
}

TEST_CASE("Mouse: event update", "[libv.ui.event.mouse]") {
	TestContextMouse table;

	TestCoreComponent watcher0("watcher0", PassThrough::on, Interaction::enable);
	TestCoreComponent watcher1("watcher1", PassThrough::on, Interaction::enable);
	table.event_position({0, 0});

	SECTION("simple moving watcher") {
		subscribe(table, watcher0, {0, 0}, {10, 10}, libv::ui::MouseOrder{0});
		CHECK(watcher0.num_event == 0);
		table.event_update_layout(); // Register the new subscriptions
		CHECK(watcher0.num_event == 1);

		table.update(watcher0, {5, 5}, {10, 10}, libv::ui::MouseOrder{0});
		CHECK(watcher0.num_event == 1);
		table.event_update_layout();
		CHECK(watcher0.num_event == 2);

		table.update(watcher0, {5, 5}, {10, 10}, libv::ui::MouseOrder{0});
		CHECK(watcher0.num_event == 2);
		table.event_update_layout();
		CHECK(watcher0.num_event == 2);

		table.update(watcher0, {0, 0}, {10, 10}, libv::ui::MouseOrder{0});
		CHECK(watcher0.num_event == 2);
		table.event_update_layout();
		CHECK(watcher0.num_event == 3);

		CHECK(watcher1.num_event == 0);
	}

	SECTION("two staged moving watcher") {
		subscribe(table, watcher0, {0, 0}, {10, 10}, libv::ui::MouseOrder{0});
		CHECK(watcher0.num_event == 0);
		CHECK(watcher1.num_event == 0);
		table.event_update_layout(); // Register the new subscriptions
		CHECK(watcher0.num_event == 1);
		CHECK(watcher1.num_event == 0);

		subscribe(table, watcher1, {0, 0}, {10, 10}, libv::ui::MouseOrder{1});
		CHECK(watcher0.num_event == 1);
		CHECK(watcher1.num_event == 0);
		table.event_update_layout(); // Register the new subscriptions #2
		CHECK(watcher0.num_event == 2);
		CHECK(watcher1.num_event == 1);

		table.update(watcher0, {5, 5}, {10, 10}, libv::ui::MouseOrder{0});
		CHECK(watcher0.num_event == 2);
		CHECK(watcher1.num_event == 1);
		table.event_update_layout();
		CHECK(watcher0.num_event == 3);
		CHECK(watcher1.num_event == 2);

		table.update(watcher1, {5, 5}, {10, 10}, libv::ui::MouseOrder{0});
		CHECK(watcher0.num_event == 3);
		CHECK(watcher1.num_event == 2);
		table.event_update_layout();
		CHECK(watcher0.num_event == 3);
		CHECK(watcher1.num_event == 3);

		table.update(watcher0, {5, 5}, {10, 10}, libv::ui::MouseOrder{0});
		CHECK(watcher0.num_event == 3);
		CHECK(watcher1.num_event == 3);
		table.event_update_layout();
		CHECK(watcher0.num_event == 3);
		CHECK(watcher1.num_event == 3);

		table.update(watcher0, {0, 0}, {10, 10}, libv::ui::MouseOrder{0});
		CHECK(watcher0.num_event == 3);
		CHECK(watcher1.num_event == 3);
		table.event_update_layout();
		CHECK(watcher0.num_event == 4);
		CHECK(watcher1.num_event == 3);
	}
}

TEST_CASE("Mouse: event leave", "[libv.ui.event.mouse]") {
	TestContextMouse table;

	TestCoreComponent watcher0("watcher0", PassThrough::on, Interaction::enable);
	table.event_position({0, 0});

	subscribe(table, watcher0, {0, 0}, {10, 10}, libv::ui::MouseOrder{0});
	CHECK(watcher0.num_event == 0);
	table.event_update_layout(); // Register the new subscriptions
	CHECK(watcher0.num_event == 1);
	table.event_leave();
	CHECK(watcher0.num_event == 2);

	table.event_position({15, 15});
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

TEST_CASE("Mouse: Overlapping non pass-through components", "[libv.ui.event.mouse]") {
	TestContextMouse table;

	TestCoreComponent watcher0("watcher0", PassThrough::off, Interaction::enable);
	TestCoreComponent watcher1("watcher1", PassThrough::off, Interaction::enable);

	subscribe(table, watcher0, {2, 2}, {10, 10}, libv::ui::MouseOrder{0});
	subscribe(table, watcher1, {6, 6}, {10, 10}, libv::ui::MouseOrder{1});
	table.event_position({0, 0});
	table.event_update_layout(); // Register the new subscriptions

	CHECK(watcher0.num_event == 0);
	CHECK(watcher1.num_event == 0);

	table.event_position({8, 8});
	CHECK(watcher0.num_event == 0);
	CHECK(watcher1.num_event == 1);
	table.event_scroll({1, 1});
	CHECK(watcher0.num_event == 0);
	CHECK(watcher1.num_event == 2);
	table.event_button(libv::input::MouseButton::Left, libv::input::Action::press);
	CHECK(watcher0.num_event == 0);
	CHECK(watcher1.num_event == 3);

	table.event_position({4, 4});
	CHECK(watcher0.num_event == 1);
	CHECK(watcher1.num_event == 4);
	table.event_scroll({1, 1});
	CHECK(watcher0.num_event == 2);
	CHECK(watcher1.num_event == 4);
	table.event_button(libv::input::MouseButton::Left, libv::input::Action::press);
	CHECK(watcher0.num_event == 3);
	CHECK(watcher1.num_event == 4);

	table.event_position({0, 0});
	CHECK(watcher0.num_event == 4);
	CHECK(watcher1.num_event == 4);
}

// -------------------------------------------------------------------------------------------------

TEST_CASE("Mouse: Disabled component has no mouse interaction", "[libv.ui.event.mouse]") {
	TestContextMouse2 table;

	TestCoreComponent2 watcher0(table, "w0", PassThrough::off, Interaction::disable);
	watcher0.subscribe({2, 2}, {10, 10}, libv::ui::MouseOrder{0});

	table.event_position({0, 0});
	table.event_update_layout(); // Register the new subscriptions
	CHECK(table.history() == "");

	table.event_update_layout();
	table.event_position({4, 4});
	table.event_button(libv::input::MouseButton::Left, libv::input::Action::press);
	CHECK(table.history() == "");

	watcher0.update(Interaction::enable);
	table.event_update_layout();
	table.event_button(libv::input::MouseButton::Left, libv::input::Action::press);
	CHECK(table.history() == "w0:E, w0:B");

	watcher0.update(Interaction::disable);
	table.event_update_layout();
	table.event_button(libv::input::MouseButton::Left, libv::input::Action::press);
	CHECK(table.history() == "w0:L");

	watcher0.update(Interaction::enable);
	table.event_update_layout();
	table.event_position({4, 5});
	table.event_button(libv::input::MouseButton::Left, libv::input::Action::press);
	CHECK(table.history() == "w0:E, w0:M, w0:B");

	watcher0.update(Interaction::disable);
	table.event_update_layout();
	table.event_position({4, 4});
	table.event_button(libv::input::MouseButton::Left, libv::input::Action::press);
	CHECK(table.history() == "w0:L");
}

TEST_CASE("Mouse: Disabled component has no mouse interaction overlapping", "[libv.ui.event.mouse]") {
	TestContextMouse2 table;

	TestCoreComponent2 watcher0(table, "w0", PassThrough::off, Interaction::disable);
	TestCoreComponent2 watcher1(table, "w1", PassThrough::off, Interaction::disable);
	watcher0.subscribe({2, 2}, {10, 10}, libv::ui::MouseOrder{0});
	watcher1.subscribe({5, 5}, {10, 10}, libv::ui::MouseOrder{1});

	table.event_position({0, 0});
	table.event_update_layout(); // Register the new subscriptions
	CHECK(table.history() == "");

	watcher0.update(Interaction::enable);
	watcher1.update(Interaction::enable);
	table.event_update_layout();

	table.event_position({4, 4});
	CHECK(table.history() == "w0:ME");

	table.event_position({6, 6});
	CHECK(table.history() == "w1:ME, w0:ML");

	watcher1.update(Interaction::disable);
	table.event_update_layout();
	CHECK(table.history() == "w1:L");

	watcher1.update(Interaction::enable);
	table.event_update_layout();
	CHECK(table.history() == "w1:E");

	watcher0.update(Interaction::disable);
	table.event_update_layout();
	CHECK(table.history() == "w1:N");

	watcher0.update(Interaction::enable); // This check the case where w0 is enabled under w1
	table.event_update_layout();
	CHECK(table.history() == "w1:N");

	watcher1.update(PassThrough::on); // After enabling PassThrough on the upper one, the lower should receive events

	watcher0.update(Interaction::disable);
	table.event_update_layout();
	CHECK(table.history() == "w1:N");

	watcher0.update(Interaction::enable);
	table.event_update_layout();
	CHECK(table.history() == "w1:N, w0:E");

	watcher0.update(Interaction::disable);
	table.event_update_layout();
	CHECK(table.history() == "w1:N, w0:L");
}

TEST_CASE("Mouse: Disabled component has no mouse interaction overlapping middle", "[libv.ui.event.mouse]") {
	TestContextMouse2 table;

	TestCoreComponent2 watcher0(table, "w0", PassThrough::on, Interaction::disable);
	TestCoreComponent2 watcher1(table, "w1", PassThrough::on, Interaction::enable);
	TestCoreComponent2 watcher2(table, "w2", PassThrough::on, Interaction::disable);
	watcher0.subscribe({2, 2}, {10, 10}, libv::ui::MouseOrder{0});
	watcher1.subscribe({4, 4}, {10, 10}, libv::ui::MouseOrder{1});
	watcher2.subscribe({6, 6}, {10, 10}, libv::ui::MouseOrder{1});

	table.event_position({0, 0});
	table.event_update_layout(); // Register the new subscriptions
	CHECK(table.history() == "");

	table.event_position({6, 6});
	CHECK(table.history() == "w1:ME");

	table.event_button(libv::input::MouseButton::Left, libv::input::Action::press);
	CHECK(table.history() == "w1:B");
}

// -------------------------------------------------------------------------------------------------
