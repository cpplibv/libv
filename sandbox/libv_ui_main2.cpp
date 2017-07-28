// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

// libv
#include <libv/frame/frame.hpp>
#include <libv/log/log.hpp>
// std
#include <iostream>
// pro
#include <libv/ui4/component/image.hpp>
#include <libv/ui4/component/label.hpp>
#include <libv/ui4/component/panel.hpp>
#include <libv/ui4/component/quad.hpp>
#include <libv/ui4/component/stretch.hpp>
//#include <libv/ui4/component/layout.hpp>
//#include <libv/ui4/component/mouse.hpp>
#include <libv/ui4/ui.hpp>


// -------------------------------------------------------------------------------------------------

inline libv::LoggerModule log_sandbox{libv::logger, "sandbox"};

class GameFrame : public libv::Frame {
	std::shared_ptr<libv::ui::Panel> panel;
//	std::shared_ptr<libv::ui::Button> button;
	std::shared_ptr<libv::ui::Image> image;
	std::shared_ptr<libv::ui::Label> label;
	std::shared_ptr<libv::ui::Stretch> stretch;
	std::shared_ptr<libv::ui::Quad> quad;

public:
	libv::ui::UI ui;

	GameFrame(const std::string& title, int32_t x, int32_t y) : Frame(title, x, y) {
		setPosition(POSITION_CENTER_CURRENT_MONITOR);
		setOpenGLProfile(OPENGL_PROFILE_COMPAT);
		setOpenGLVersion(3, 3);
		setOpenGLSamples(4);
		ui.attach(*this);
		ui.setSize(1280.f, 800.f); // TODO P4: auto detect size changes

		panel = std::make_shared<libv::ui::Panel>();
//		panel->setName("panel");
		panel->name = "panel";
//		panel->setSize("100%, 100%");
//		panel->setSize(libv::ui::percent(100.f), libv::ui::percent(100.f));
		panel->propertySize = libv::ui::Size{libv::ui::percent(100.f), libv::ui::percent(100.f)};;

//		button = std::make_shared<libv::ui::Button>();
//		button->setName("button");
//		button->setSize("100px, 100px");
//		button->setSize(libv::ui::pixel(100.f), libv::ui::pixel(100.f));
//		button->propertySize = libv::ui::Size{libv::ui::pixel(100.f), libv::ui::pixel(100.f)};
//		button->setText("Click me!");
//				+ ui::component::mouse([](auto& self) {
//					self.onClick([] { IRIS_LOG_UI_DEBUG("onClick"); });
//					self.onDoubleClick([] { IRIS_LOG_UI_DEBUG("onDoubleClick"); });
//					self.onMouseEnter([] { IRIS_LOG_UI_DEBUG("onMouseEnter"); });
//					self.onMouseExit([] { IRIS_LOG_UI_DEBUG("onMouseExit"); });
//				})
//		panel->add(button);

		label = std::make_shared<libv::ui::Label>();
//		label->setName("label");
		label->name = "label";
//		label->setSize("c, c");
//		label->setSize(libv::ui::content(), libv::ui::content());
		label->propertySize = libv::ui::Size{libv::ui::content(), libv::ui::content()};
		label->setText("Well I could fight this, but I may die.");
		panel->add(label);

		image = std::make_shared<libv::ui::Image>();
		image->name = "image";
//		image->setSize("c, c");
//		image->setSize(libv::ui::content(), libv::ui::content());
		image->propertySize = libv::ui::Size{libv::ui::content(), libv::ui::content()};
		image->setImageFile("res/texture/ui/test_metal.dds");
		panel->add(image);

		stretch = std::make_shared<libv::ui::Stretch>(ui);
		stretch->name = "stretch";
//		stretch->setSize("1r, 50%");
		stretch->propertySize = libv::ui::Size{libv::ui::ratio(1.f), libv::ui::percent(50.f)};
		stretch->setImageFile("res/texture/ui/stretch_border_orange_2_32px.png");
		panel->add(stretch);

		quad = std::make_shared<libv::ui::Quad>();
//		quad->setName("quad");
		quad->name = "quad";
//		quad->setSize("100px, 100px");
		quad->propertySize = libv::ui::Size{libv::ui::pixel(100.f), libv::ui::pixel(100.f)};
		quad->setColor(0.5f, 0.5f, 0.5f);
		panel->add(quad);

		ui.add(panel);

		onKey.output([&] (const libv::frame::EventKey& e) {
			if (e.action == 0)
				return;

			if (e.key == libv::frame::Key::Escape)
				closeDefault();

			if (e.key == libv::frame::Key::Backspace) {
				try {
					const auto& current = label->getText();
					auto it = current.end();
					utf8::prior(it, current.begin());
					label->setText({current.begin(), it});
				} catch (const utf8::not_enough_room&) {
					label->setText("");
					log_sandbox.warn("Label text was already empty");
				} catch (const utf8::invalid_utf8&) {
					label->setText("");
					log_sandbox.warn("Label text contains invalid utf8");
				}
			}

			if (e.key == libv::frame::Key::Enter)
				label->setText(label->getText() + '\n');
		});
		onChar.output([&] (const libv::frame::EventChar& e) {
			label->setText(label->getText() + e.utf8);
		});
	}
};

// -------------------------------------------------------------------------------------------------

int main(int, char**) {
	std::cout << libv::logger;

	{
		GameFrame frame("ECUI sandbox", 1280, 800);
		frame.show();
		frame.join();
	}

	return 0;
}
