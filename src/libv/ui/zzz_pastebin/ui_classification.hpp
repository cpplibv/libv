// Project: libv.ui, File: src/libv/ui/zzz_pastebin/ui_classification.hpp, Author: Császár Mátyás [Vader]

#pragma once


struct Size {};
struct Module {};
template <typename CRTP> struct Hive {};
struct Layout : Module { Size size; };

// ------

struct Render_StretchPlane : Module {};
struct Render_ImagePlane : Module {};
struct Event_Keyboard : Module {};
struct Event_Mouse : Module {};
//struct Event_MousePosition : Module {};
struct Focus : Module {};
struct Layout_Fix : Layout {};
struct Layout_Float : Layout {};
struct Layout_Flow : Layout {};
struct Layout_Layer : Layout {};
struct Layout_ToolTip : Layout {};

/* ------  Notes:
Properties:
3 Type of default behavior for inheritance:
- Lock: forced inheritance (enable/disable)
- Normal: only effecting current (background, padding, show/hide)
- Overridable: inheritance until overridden (font, font_color)
Dependencies:
Render -> Layout
Event -> Layout
Focus ->
------ */

struct Label : public Hive<Label> {
    // String2D, Something that requires complex binding of size and layouting
};

struct ToolTip : public Hive<ToolTip> {
	Render_StretchPlane background;
	Label text;
	Layout_Flow layout;
};

struct ToolTip_Host : public Hive<ToolTip_Host> {
	// This tooltip should follow the mouse, there should be ones that does not
	ToolTip toolTip;
	Layout_ToolTip layout;
	Event_Mouse eventMouse;
};

struct CivIcon : public Hive<CivIcon> {
	Render_ImagePlane plane;
	Layout_Layer layout;

	ToolTip_Host toolTip;
	Event_Mouse eventMouse;
};

struct LoadingBar : public Hive<LoadingBar> {

};

struct DeclarationOfWarIcon : public Hive<DeclarationOfWarIcon> {
	CivIcon left;
	CivIcon right;
	Render_ImagePlane roundBackground;

	Layout_Layer layout;

	ToolTip toolTip;
	Event_Mouse eventMouse;
};

struct RightSidePanel : public Hive<RightSidePanel> {
	std::vector<DeclarationOfWarIcon> notifications;

    struct Layout_DroppingNewEntriesFromSky {};
	Layout_DroppingNewEntriesFromSky layout;

	template <typename T>
    void ui_access(T& access) {
        access(layout);

		for (auto& notification : notifications) {
			access(notification);
		}
    }
};

struct PlanetHud : public Hive<PlanetHud> {
	Label name;
};

struct Button : public Hive<Button> {
	Label text;
	Render_StretchPlane plane;

	Layout_Layer layout;

	Focus focus;
	Event_Keyboard eventKeyboard;
	Event_Mouse eventMouse;

    template <typename T>
    void ui_access(T& access) {
        access(text);
        access(plane);
        access(layout);
        access(focus);
        access(eventKeyboard);
        access(eventMouse);
    }
};

struct InputField : public Hive<InputField> {
    Label text;
    Render_StretchPlane plane;

    Layout_Layer layout;

    Focus focus;
    Event_Keyboard eventKeyboard;
    Event_Mouse eventMouse;

    template <typename T>
    void ui_access(T& access) {
        access(text);
        access(plane);
        access(layout);
        access(focus);
        access(eventKeyboard);
        access(eventMouse);
    }
};
