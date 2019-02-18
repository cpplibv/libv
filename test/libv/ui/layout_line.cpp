//// File: %<%NAME%>%.%<%EXTENSION%>%, Created on %<%DATE%>% %<%TIME%>%, Author: %<%USER%>%
//
//#include <catch/catch.hpp>
//
//#include <libv/ui/layout_line.hpp>
//#include <libv/ui/ui.hpp>
//#include <libv/log/log.hpp>
//#include <libv/utility/approxing.hpp>
//
//#include "layout_test_utils.hpp"
//
//// -------------------------------------------------------------------------------------------------
//
//struct TestPanel : libv::ui::ComponentStatic<TestPanel> {
//	libv::ui::LayoutLine layout;
//	Quad sub0;
//	Quad sub1;
//	Quad sub2;
//	template <typename T> void ui_access(T&& access) {
//		access(layout);
//		access(sub0);
//		access(sub1);
//		access(sub2);
//	}
//};
//
//// -------------------------------------------------------------------------------------------------
//
//TEST_CASE("layout line: positioning with alignment", "[iris.ui]") {
//	libv::ui::UI ui;
//	ui.setSize(400.f, 300.f, 0.f);
//	ui.setPosition(0.f, 0.f, 0.f);
//
//	const auto root = std::make_shared<TestPanel>();
//	ui.add(root);
//
//	auto& panl = *root.get();
//	auto& sub0 = panl.sub0;
//	auto& sub1 = panl.sub1;
//	auto& sub2 = panl.sub2;
//
//	panl.propertySize = libv::ui::Size{libv::ui::ratio(1.f), libv::ui::ratio(1.f)};
//	sub0.propertySize = libv::ui::Size{libv::ui::pixel(40.f), libv::ui::pixel(60.f)};
//	sub1.propertySize = libv::ui::Size{libv::ui::pixel(50.f), libv::ui::pixel(50.f)};
//	sub2.propertySize = libv::ui::Size{libv::ui::pixel(60.f), libv::ui::pixel(40.f)};
//
//	panl.layout.alignHorizontal = libv::ui::Alignment::LEFT;
//	panl.layout.alignVertical = libv::ui::Alignment::TOP;
//	panl.layout.orient = libv::ui::Orientation::LEFT_TO_RIGHT;
//
//	SECTION("alignment: TOP_LEFT") {
//		panl.layout.alignVertical = libv::ui::Alignment::TOP;
//		panl.layout.alignHorizontal = libv::ui::Alignment::LEFT;
//		ui.layout();
//
//		CHECK(sub0.position == ApproxVec3f(0.f, 240.f, 0.f));
//		CHECK(sub1.position == ApproxVec3f(40.f, 250.f, 0.f));
//		CHECK(sub2.position == ApproxVec3f(90.f, 260.f, 0.f));
//	}
//
//	SECTION("alignment: TOP_CENTER") {
//		panl.layout.alignVertical = libv::ui::Alignment::TOP;
//		panl.layout.alignHorizontal = libv::ui::Alignment::CENTER;
//		ui.layout();
//
//		CHECK(sub0.position == ApproxVec3f(125.f, 240.f, 0.f));
//		CHECK(sub1.position == ApproxVec3f(165.f, 250.f, 0.f));
//		CHECK(sub2.position == ApproxVec3f(215.f, 260.f, 0.f));
//	}
//
//	SECTION("alignment: TOP_RIGHT") {
//		panl.layout.alignVertical = libv::ui::Alignment::TOP;
//		panl.layout.alignHorizontal = libv::ui::Alignment::RIGHT;
//		ui.layout();
//
//		CHECK(sub0.position == ApproxVec3f(250.f, 240.f, 0.f));
//		CHECK(sub1.position == ApproxVec3f(290.f, 250.f, 0.f));
//		CHECK(sub2.position == ApproxVec3f(340.f, 260.f, 0.f));
//	}
//
//	SECTION("alignment: TOP_JUSTIFY") {
//		panl.layout.alignVertical = libv::ui::Alignment::TOP;
//		panl.layout.alignHorizontal = libv::ui::Alignment::JUSTIFY;
//		ui.layout();
//
//		CHECK(sub0.position == ApproxVec3f(0.f, 240.f, 0.f));
//		CHECK(sub1.position == ApproxVec3f(165.f, 250.f, 0.f));
//		CHECK(sub2.position == ApproxVec3f(340.f, 260.f, 0.f));
//	}
//
//	SECTION("alignment: CENTER_LEFT") {
//		panl.layout.alignVertical = libv::ui::Alignment::CENTER;
//		panl.layout.alignHorizontal = libv::ui::Alignment::LEFT;
//		ui.layout();
//
//		CHECK(sub0.position == ApproxVec3f(0.f, 120.f, 0.f));
//		CHECK(sub1.position == ApproxVec3f(40.f, 125.f, 0.f));
//		CHECK(sub2.position == ApproxVec3f(90.f, 130.f, 0.f));
//	}
//
//	SECTION("alignment: CENTER_CENTER") {
//		panl.layout.alignVertical = libv::ui::Alignment::CENTER;
//		panl.layout.alignHorizontal = libv::ui::Alignment::CENTER;
//		ui.layout();
//
//		CHECK(sub0.position == ApproxVec3f(125.f, 120.f, 0.f));
//		CHECK(sub1.position == ApproxVec3f(165.f, 125.f, 0.f));
//		CHECK(sub2.position == ApproxVec3f(215.f, 130.f, 0.f));
//	}
//
//	SECTION("alignment: CENTER_RIGHT") {
//		panl.layout.alignVertical = libv::ui::Alignment::CENTER;
//		panl.layout.alignHorizontal = libv::ui::Alignment::RIGHT;
//		ui.layout();
//
//		CHECK(sub0.position == ApproxVec3f(250.f, 120.f, 0.f));
//		CHECK(sub1.position == ApproxVec3f(290.f, 125.f, 0.f));
//		CHECK(sub2.position == ApproxVec3f(340.f, 130.f, 0.f));
//	}
//
//	SECTION("alignment: CENTER_JUSTIFY") {
//		panl.layout.alignVertical = libv::ui::Alignment::CENTER;
//		panl.layout.alignHorizontal = libv::ui::Alignment::JUSTIFY;
//		ui.layout();
//
//		CHECK(sub0.position == ApproxVec3f(0.f, 120.f, 0.f));
//		CHECK(sub1.position == ApproxVec3f(165.f, 125.f, 0.f));
//		CHECK(sub2.position == ApproxVec3f(340.f, 130.f, 0.f));
//	}
//
//	SECTION("alignment: BOTTOM_LEFT") {
//		panl.layout.alignVertical = libv::ui::Alignment::BOTTOM;
//		panl.layout.alignHorizontal = libv::ui::Alignment::LEFT;
//		ui.layout();
//
//		CHECK(sub0.position == ApproxVec3f(0.f, 0.f, 0.f));
//		CHECK(sub1.position == ApproxVec3f(40.f, 0.f, 0.f));
//		CHECK(sub2.position == ApproxVec3f(90.f, 0.f, 0.f));
//	}
//
//	SECTION("alignment: BOTTOM_CENTER") {
//		panl.layout.alignVertical = libv::ui::Alignment::BOTTOM;
//		panl.layout.alignHorizontal = libv::ui::Alignment::CENTER;
//		ui.layout();
//
//		CHECK(sub0.position == ApproxVec3f(125.f, 0.f, 0.f));
//		CHECK(sub1.position == ApproxVec3f(165.f, 0.f, 0.f));
//		CHECK(sub2.position == ApproxVec3f(215.f, 0.f, 0.f));
//	}
//
//	SECTION("alignment: BOTTOM_RIGHT") {
//		panl.layout.alignVertical = libv::ui::Alignment::BOTTOM;
//		panl.layout.alignHorizontal = libv::ui::Alignment::RIGHT;
//		ui.layout();
//
//		CHECK(sub0.position == ApproxVec3f(250.f, 0.f, 0.f));
//		CHECK(sub1.position == ApproxVec3f(290.f, 0.f, 0.f));
//		CHECK(sub2.position == ApproxVec3f(340.f, 0.f, 0.f));
//	}
//
//	SECTION("alignment: BOTTOM_JUSTIFY") {
//		panl.layout.alignVertical = libv::ui::Alignment::BOTTOM;
//		panl.layout.alignHorizontal = libv::ui::Alignment::JUSTIFY;
//		ui.layout();
//
//		CHECK(sub0.position == ApproxVec3f(0.f, 0.f, 0.f));
//		CHECK(sub1.position == ApproxVec3f(165.f, 0.f, 0.f));
//		CHECK(sub2.position == ApproxVec3f(340.f, 0.f, 0.f));
//	}
//
//	CHECK(panl.position == ApproxVec3f(0.f, 0.f, 0.f));
//	CHECK(panl.size == ApproxVec3f(400.f, 300.f, 0.f));
//	CHECK(sub0.size == ApproxVec3f(40.f, 60.f, 0.f));
//	CHECK(sub1.size == ApproxVec3f(50.f, 50.f, 0.f));
//	CHECK(sub2.size == ApproxVec3f(60.f, 40.f, 0.f));
//}
//
//TEST_CASE("layout line: positioning with orientation", "[iris.ui]") {
//	libv::ui::UI ui;
//	ui.setSize(400.f, 300.f, 0.f);
//	ui.setPosition(0.f, 0.f, 0.f);
//
//	const auto root = std::make_shared<TestPanel>();
//	ui.add(root);
//
//	auto& panl = *root.get();
//	auto& sub0 = panl.sub0;
//	auto& sub1 = panl.sub1;
//	auto& sub2 = panl.sub2;
//
//	panl.propertySize = libv::ui::Size{libv::ui::ratio(1.f), libv::ui::ratio(1.f)};
//	sub0.propertySize = libv::ui::Size{libv::ui::pixel(40.f), libv::ui::pixel(60.f)};
//	sub1.propertySize = libv::ui::Size{libv::ui::pixel(50.f), libv::ui::pixel(50.f)};
//	sub2.propertySize = libv::ui::Size{libv::ui::pixel(60.f), libv::ui::pixel(40.f)};
//
//	panl.layout.alignHorizontal = libv::ui::Alignment::LEFT;
//	panl.layout.alignVertical = libv::ui::Alignment::TOP;
//	panl.layout.orient = libv::ui::Orientation::LEFT_TO_RIGHT;
//
//	SECTION("orient: BOTTOM_TO_TOP") {
//		panl.layout.orient = libv::ui::Orientation::BOTTOM_TO_TOP;
//		ui.layout();
//
//		CHECK(sub0.position == ApproxVec3f(0.f, 150.f, 0.f));
//		CHECK(sub1.position == ApproxVec3f(0.f, 210.f, 0.f));
//		CHECK(sub2.position == ApproxVec3f(0.f, 260.f, 0.f));
//	}
//
//	SECTION("orient: LEFT_TO_RIGHT") {
//		panl.layout.orient = libv::ui::Orientation::LEFT_TO_RIGHT;
//		ui.layout();
//
//		CHECK(sub0.position == ApproxVec3f(0.f, 240.f, 0.f));
//		CHECK(sub1.position == ApproxVec3f(40.f, 250.f, 0.f));
//		CHECK(sub2.position == ApproxVec3f(90.f, 260.f, 0.f));
//	}
//
//	SECTION("orient: RIGHT_TO_LEFT") {
//		panl.layout.orient = libv::ui::Orientation::RIGHT_TO_LEFT;
//		ui.layout();
//
//		CHECK(sub0.position == ApproxVec3f(110.f, 240.f, 0.f));
//		CHECK(sub1.position == ApproxVec3f(60.f, 250.f, 0.f));
//		CHECK(sub2.position == ApproxVec3f(0.f, 260.f, 0.f));
//	}
//
//	SECTION("orient: TOP_TO_BOTTOM") {
//		panl.layout.orient = libv::ui::Orientation::TOP_TO_BOTTOM;
//		ui.layout();
//
//		CHECK(sub0.position == ApproxVec3f(0.f, 240.f, 0.f));
//		CHECK(sub1.position == ApproxVec3f(0.f, 190.f, 0.f));
//		CHECK(sub2.position == ApproxVec3f(0.f, 150.f, 0.f));
//	}
//
//	CHECK(panl.position == ApproxVec3f(0.f, 0.f, 0.f));
//	CHECK(panl.size == ApproxVec3f(400.f, 300.f, 0.f));
//	CHECK(sub0.size == ApproxVec3f(40.f, 60.f, 0.f));
//	CHECK(sub1.size == ApproxVec3f(50.f, 50.f, 0.f));
//	CHECK(sub2.size == ApproxVec3f(60.f, 40.f, 0.f));
//}
//
//TEST_CASE("layout line: sizing one quad", "[iris.ui]") {
//	libv::ui::UI ui;
//	ui.setSize(400.f, 300.f, 0.f);
//	ui.setPosition(0.f, 0.f, 0.f);
//
//	const auto quad = std::make_shared<Quad>();
//	ui.add(quad);
//
//	SECTION("ratio") {
//		quad->propertySize = libv::ui::Size{libv::ui::ratio(1.f), libv::ui::ratio(1.f)};
//		ui.layout();
//		CHECK(quad->size == ApproxVec3f(400.f, 300.f, 0.f));
//	}
//	SECTION("pixel") {
//		quad->propertySize = libv::ui::Size{libv::ui::pixel(100.f), libv::ui::pixel(90.f)};
//		ui.layout();
//		CHECK(quad->size == ApproxVec3f(100.f, 90.f, 0.f));
//	}
//	SECTION("percent") {
//		quad->propertySize = libv::ui::Size{libv::ui::percent(50.f), libv::ui::percent(10.f)};
//		ui.layout();
//		CHECK(quad->size == ApproxVec3f(200.f, 30.f, 0.f));
//	}
//	SECTION("content") {
//		quad->propertySize = libv::ui::Size{libv::ui::content(), libv::ui::content()};
//		ui.layout();
//		CHECK(quad->size == ApproxVec3f(0.f, 0.f, 0.f));
//	}
//	// Test the rest of the combinations:
//	//	ratio, pixel
//	//	ratio, percent
//	//	ratio, content
//	//	pixel, percent
//	//	pixel, content
//	//	percent, content
//}
//
//// Clean up these tests:
//
//TEST_CASE("layout line: basic ui layout with children", "[iris.ui]") {
//	libv::ui::UI ui;
//	ui.setSize(400.f, 300.f, 0.f);
//	ui.setPosition(0.f, 0.f, 0.f);
//
//	const auto root = std::make_shared<Nest3<Quad, Quad, Quad>>();
//
//	auto& panl = *root.get();
//	auto& sub0 = panl.sub0;
//	auto& sub1 = panl.sub1;
//	auto& sub2 = panl.sub2;
//
//	SECTION("miscellaneous") {
//		panl.propertySize = libv::ui::Size{libv::ui::ratio(1.f), libv::ui::ratio(1.f)};
//		sub0.propertySize = libv::ui::Size{libv::ui::pixel(50.f), libv::ui::pixel(50.f)};
//		sub1.propertySize = libv::ui::Size{libv::ui::percent(50.f), libv::ui::content()};
//		sub2.propertySize = libv::ui::Size{libv::ui::ratio(1.f), libv::ui::ratio(1.f)};
//
//		ui.add(root);
//		ui.layout();
//
//		CHECK(panl.position == ApproxVec3f(0.f, 0.f, 0.f));
//		CHECK(panl.size == ApproxVec3f(400.f, 300.f, 0.f));
//
//		CHECK(sub0.position == ApproxVec3f(0.f, 250.f, 0.f));
//		CHECK(sub0.size == ApproxVec3f(50.f, 50.f, 0.f));
//		CHECK(sub1.position == ApproxVec3f(50.f, 300.f, 0.f));
//		CHECK(sub1.size == ApproxVec3f(200.f, 0.f, 0.f));
//		CHECK(sub2.position == ApproxVec3f(250.f, 0.f, 0.f));
//		CHECK(sub2.size == ApproxVec3f(150.f, 300.f, 0.f));
//	}
//
//	SECTION("pixel-percent-ratio inside content") {
//		panl.propertySize = libv::ui::Size{libv::ui::content(), libv::ui::content()};
//		sub0.propertySize = libv::ui::Size{libv::ui::pixel(100.f), libv::ui::pixel(50.f)};
//		sub1.propertySize = libv::ui::Size{libv::ui::percent(20.f), libv::ui::content()};
//		sub2.propertySize = libv::ui::Size{libv::ui::ratio(1.f), libv::ui::ratio(1.f)};
//
//		ui.add(root);
//		ui.layout();
//
//		CHECK(panl.position == ApproxVec3f(0.f, 250.f, 0.f));
//		CHECK(panl.size == ApproxVec3f(125.f, 50.f, 0.f));
//
//		CHECK(sub0.position == ApproxVec3f(0.f, 250.f, 0.f));
//		CHECK(sub0.size == ApproxVec3f(100.f, 50.f, 0.f));
//		CHECK(sub1.position == ApproxVec3f(100.f, 300.f, 0.f));
//		CHECK(sub1.size == ApproxVec3f(25.f, 0.f, 0.f));
//		CHECK(sub2.position == ApproxVec3f(125.f, 250.f, 0.f));
//		CHECK(sub2.size == ApproxVec3f(0.f, 50.f, 0.f));
//	}
//}
//
//TEST_CASE("layout line: basic ui layout with transitive nesting of content", "[iris.ui]") {
//	libv::ui::UI ui;
//	ui.setSize(400.f, 300.f, 0.f);
//	ui.setPosition(0.f, 0.f, 0.f);
//
//	const auto root = std::make_shared<Nest1<Nest1<Nest1<Quad>>>>();
//
//	auto& panl = *root.get();
//	auto& sub0 = panl.sub0;
//	auto& sub1 = panl.sub0.sub0;
//	auto& sub2 = panl.sub0.sub0.sub0;
//
//	SECTION("content-content-content-pixel") {
//		panl.propertySize = libv::ui::Size{libv::ui::content(), libv::ui::content()};
//		sub0.propertySize = libv::ui::Size{libv::ui::content(), libv::ui::content()};
//		sub1.propertySize = libv::ui::Size{libv::ui::content(), libv::ui::content()};
//		sub2.propertySize = libv::ui::Size{libv::ui::pixel(250.f), libv::ui::pixel(250.f)};
//
//		ui.add(root);
//		ui.layout();
//
//		CHECK(panl.position == ApproxVec3f(0.f, 50.f, 0.f));
//		CHECK(panl.size == ApproxVec3f(250.f, 250.f, 0.f));
//
//		CHECK(sub0.position == ApproxVec3f(0.f, 50.f, 0.f));
//		CHECK(sub0.size == ApproxVec3f(250.f, 250.f, 0.f));
//		CHECK(sub1.position == ApproxVec3f(0.f, 50.f, 0.f));
//		CHECK(sub1.size == ApproxVec3f(250.f, 250.f, 0.f));
//		CHECK(sub2.position == ApproxVec3f(0.f, 50.f, 0.f));
//		CHECK(sub2.size == ApproxVec3f(250.f, 250.f, 0.f));
//	}
//}
//
//TEST_CASE("layout line: basic ui layout", "[iris.ui]") {
//	libv::ui::UI ui;
//	ui.setSize(400.f, 300.f, 0.f);
//	ui.setPosition(0.f, 0.f, 0.f);
//
//	const auto root = std::make_shared<
//		Nest3<
//			Nest1<Quad>,
//			Quad,
//			Nest1<Quad>
//		>
//	>();
//
//	auto& panl = *root.get();
//	auto& sub0 = panl.sub0;
//	auto& sin0 = panl.sub0.sub0;
//	auto& sub1 = panl.sub1;
//	auto& sub2 = panl.sub2;
//	auto& sin2 = panl.sub2.sub0;
//
//	SECTION("content-ratio-content inside percent") {
//		panl.propertySize = libv::ui::Size{libv::ui::percent(100.f), libv::ui::pixel(25.f)};
//		sin0.propertySize = libv::ui::Size{libv::ui::pixel(30.f), libv::ui::pixel(15.f)};
//		sub0.propertySize = libv::ui::Size{libv::ui::content(), libv::ui::percent(100.f)};
//		sub1.propertySize = libv::ui::Size{libv::ui::ratio(1.f), libv::ui::percent(100.f)};
//		sub2.propertySize = libv::ui::Size{libv::ui::content(), libv::ui::percent(100.f)};
//		sin2.propertySize = libv::ui::Size{libv::ui::pixel(40.f), libv::ui::pixel(20.f)};
//
//		ui.add(root);
//		ui.layout();
//
//		CHECK(panl.position == ApproxVec3f(0.f, 275.f, 0.f));
//		CHECK(panl.size == ApproxVec3f(400.f, 25.f, 0.f));
//
//		CHECK(sub0.position == ApproxVec3f(0.f, 275.f, 0.f));
//		CHECK(sub0.size == ApproxVec3f(30.f, 25.f, 0.f));
//		CHECK(sub1.position == ApproxVec3f(30.f, 275.f, 0.f));
//		CHECK(sub1.size == ApproxVec3f(330.f, 25.f, 0.f));
//		CHECK(sub2.position == ApproxVec3f(360.f, 275.f, 0.f));
//		CHECK(sub2.size == ApproxVec3f(40.f, 25.f, 0.f));
//	}
//}
