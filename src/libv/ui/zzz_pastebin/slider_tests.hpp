// Project: libv.ui, File: src/libv/ui/zzz_pastebin/slider_tests.hpp

void foo() {
	{
		auto panel = std::make_shared<libv::ui::PanelLine>(*this, "scroll-show");
		panel.style(context().style("vm4pv.scroll-show"));

		{
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar");
			tmp.value(0); tmp.value_high(+110); tmp.value_low(+10); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar");
			tmp.value(0); tmp.value_high(+110); tmp.value_low(-10); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar");
			tmp.value(0); tmp.value_high(-110); tmp.value_low(+10); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar");
			tmp.value(0); tmp.value_high(-110); tmp.value_low(-10); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar");
			tmp.value(0); tmp.value_high(+10); tmp.value_low(+110); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar");
			tmp.value(0); tmp.value_high(+10); tmp.value_low(-110); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar");
			tmp.value(0); tmp.value_high(-10); tmp.value_low(+110); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar");
			tmp.value(0); tmp.value_high(-10); tmp.value_low(-110); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar");
			tmp.value(0); tmp.value_high(100); tmp.value_low(100); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar");
			tmp.value(0); tmp.value_high(0); tmp.value_low(0); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar");
			tmp.value(0); tmp.value_high(100); tmp.value_low(0); tmp.value_range(0);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar");
			tmp.value(0); tmp.value_high(100); tmp.value_low(0); tmp.value_range(200);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar");
			tmp.value(0); tmp.value_high(0); tmp.value_low(100); tmp.value_range(0);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar");
			tmp.value(0); tmp.value_high(0); tmp.value_low(100); tmp.value_range(200);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar");
			tmp.value(10); tmp.value_high(210); tmp.value_low(10); tmp.value_range(20); tmp.value_step(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar");
			tmp.value(10); tmp.value_high(200); tmp.value_low(0); tmp.value_range(20); tmp.value_step(20);
		}
	}

	{
		auto panel = std::make_shared<libv::ui::PanelLine>(*this, "scroll-show2");
		panel.style(context().style("vm4pv.scroll-show2"));

		{
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(5); tmp.value_high(10); tmp.value_low(0); tmp.value_range(0);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(0); tmp.value_high(+110); tmp.value_low(+10); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(0); tmp.value_high(+110); tmp.value_low(-10); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(0); tmp.value_high(-110); tmp.value_low(+10); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(0); tmp.value_high(-110); tmp.value_low(-10); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(0); tmp.value_high(+10); tmp.value_low(+110); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(0); tmp.value_high(+10); tmp.value_low(-110); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(0); tmp.value_high(-10); tmp.value_low(+110); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(0); tmp.value_high(-10); tmp.value_low(-110); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(0); tmp.value_high(100); tmp.value_low(100); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(0); tmp.value_high(0); tmp.value_low(0); tmp.value_range(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(0); tmp.value_high(100); tmp.value_low(0); tmp.value_range(0);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(0); tmp.value_high(100); tmp.value_low(0); tmp.value_range(200);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(0); tmp.value_high(0); tmp.value_low(100); tmp.value_range(0);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(0); tmp.value_high(0); tmp.value_low(100); tmp.value_range(200);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(10); tmp.value_high(210); tmp.value_low(10); tmp.value_range(20); tmp.value_step(20);
		} {
			auto tmp = panel.add_ns<libv::ui::Slider>("scroll-bar", "vm4pv.scroll-bar2");
			tmp.value(10); tmp.value_high(200); tmp.value_low(0); tmp.value_range(20); tmp.value_step(20);
		}
	}
}
