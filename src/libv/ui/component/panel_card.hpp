// Project: libv.ui, File: src/libv/ui/component/panel_card.hpp

#pragma once

// pro
#include <libv/ui/component/base_panel.hpp>
#include <libv/ui/component/detail/component_api.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

enum class CardID : int32_t {};
static constexpr CardID nullCardID{-1};
static constexpr CardID firstCardID{0};

// -------------------------------------------------------------------------------------------------

struct EventPanelCardChange : BaseEvent {
	CardID prevCardID = nullCardID;
	CardID nextCardID = nullCardID;
};

template <typename ComponentT>
struct EventHostPanelCard : EventHostGeneral<ComponentT> {
	BasicEventProxy<ComponentT, EventPanelCardChange> cardChange{this->owner};
};

// -------------------------------------------------------------------------------------------------

class PanelCard : public ComponentAPI<BasePanel, PanelCard, class CorePanelCard, EventHostPanelCard> {
public:
	using ComponentAPI::ComponentAPI;
	static constexpr std::string_view component_type = "card";
	static core_ptr create_core(std::string name);

private:
	void aux_add(CardID cardID, Component component);

public:
	CardID add(Component component);
	void add(CardID cardID, Component component); /// Add or reassigns the card
//	[[nodiscard]] Component& getCard(CardID cardID);
	void show(CardID cardID);
	[[nodiscard]] CardID getActiveCardID();
	void remove(Component& component);
	void remove(std::string_view component_name);
	void remove(CardID cardID);
	void clear();
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
