// Project: libv.ui, File: src/libv/ui/component/panel_card.cpp

// hpp
#include <libv/ui/component/panel_card.hpp>
// ext
#include <boost/container/flat_map.hpp>
// libv
#include <libv/algo/linear_find.hpp>
#include <libv/meta/for_constexpr.hpp>
#include <libv/utility/approx.hpp>
#include <libv/utility/min_max.hpp>
#include <libv/utility/to_underlying.hpp>
// pro
#include <libv/ui/component/panel_full_core.hpp>
#include <libv/ui/component/layout/view_layouted.hxx>
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/log.hpp>
#include <libv/ui/property_system/property_access.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct CorePanelCard : CorePanelFull {
	using base_type = CorePanelFull;
	using base_type::base_type;

public:
	CardID activeCardID = nullCardID;
	CardID nextCardID = firstCardID;
	boost::container::flat_map<CardID, Component> cardIDMapping;

public:
	struct Properties {
	} property;

	struct ChildProperties {
	};

	template <typename T> static void access_properties(T& ctx);
	template <typename T> static void access_child_properties(T& ctx);

//	static ComponentPropertyDescription description;
//	static ComponentPropertyDescription child_description;

public:
	virtual void doDetachChildren(libv::function_ref<bool(Component&)> callback) override;
	virtual void doStyle(StyleAccess& access) override;
	virtual void doStyleChild(StyleAccess& access, ChildID childID) override;
};

// -------------------------------------------------------------------------------------------------

template <typename T>
void CorePanelCard::access_properties(T& ctx) {
	(void) ctx;
}

template <typename T>
void CorePanelCard::access_child_properties(T& ctx) {
	(void) ctx;
}

// -------------------------------------------------------------------------------------------------

void CorePanelCard::doDetachChildren(libv::function_ref<bool(Component&)> callback) {
	CorePanelFull::doDetachChildren([&](Component& child) {
		const auto result = callback(child);
		if (result) {
			const auto cardIt = libv::linear_find_iterator(cardIDMapping, child, &decltype(cardIDMapping)::value_type::second);
			if (cardIt != cardIDMapping.end())
				cardIDMapping.erase(cardIt);
		}
		return result;
	});
}

void CorePanelCard::doStyle(StyleAccess& access) {
	access.self(*this);
}

void CorePanelCard::doStyleChild(StyleAccess& access, ChildID childID) {
	(void) access;
	(void) childID;
}

// =================================================================================================

core_ptr PanelCard::create_core(std::string name) {
	return create_core_ptr<CoreType>(std::move(name));
}

bool PanelCard::castable(libv::ui::core_ptr core) noexcept {
	return dynamic_cast<CoreType*>(core) != nullptr;
}

// -------------------------------------------------------------------------------------------------

void PanelCard::aux_add(CardID cardID, Component component) {
	if (self().activeCardID == nullCardID) {
		self().activeCardID = cardID;

		self().fire(EventPanelCardChange{{}, nullCardID, cardID});

	} else {
		component.core().enable(false);
		component.core().show(false);
	}

	self().cardIDMapping.insert_or_assign(cardID, component);
	self().add(std::move(component));
}

// ---

CardID PanelCard::add(Component component) {
	const auto cardID = self().nextCardID;
	self().nextCardID = CardID{libv::to_underlying(self().nextCardID) + 1};

	aux_add(cardID, std::move(component));
	return cardID;
}

void PanelCard::add(CardID cardID, Component component) {
	if (libv::to_underlying(cardID) > libv::to_underlying(self().nextCardID))
		self().nextCardID = CardID{libv::to_underlying(cardID) + 1};

	aux_add(cardID, std::move(component));
}

void PanelCard::show(CardID cardID) {
	const auto prevCardID = self().activeCardID;
	const auto nextCardID = cardID;

	if (nextCardID == prevCardID)
		return;

	if (prevCardID != nullCardID) {
		const auto prevCardIt = self().cardIDMapping.find(prevCardID);
		if (prevCardIt == self().cardIDMapping.end()) {
			log_ui.error("Internal consistency error: activeCardID {} is missing from mapping in {}", libv::to_underlying(prevCardID), path());
			assert("Internal consistency error");
			return;
		}

		auto& prevComponent = prevCardIt->second.core();

		prevComponent.enable(false);
		prevComponent.show(false);
	}

	if (nextCardID != nullCardID) {
		const auto nextCardIt = self().cardIDMapping.find(nextCardID);
		if (nextCardIt == self().cardIDMapping.end()) {
			log_ui.error("Attempted to show a card with ID {} that is not part of the panel in {}", libv::to_underlying(nextCardID), path());
			return;
		}

		auto& nextComponent = nextCardIt->second.core();

		nextComponent.enable(true);
		nextComponent.show(true);
	}

	self().activeCardID = nextCardID;
	self().flagAuto(Flag::pendingRender);

	self().fire(EventPanelCardChange{{}, prevCardID, nextCardID});
}

CardID PanelCard::getActiveCardID() {
	return self().activeCardID;
}

static inline void aux_remove(CorePanelCard& self, auto cardIt) {
	const auto cardID = cardIt->first;
	const auto prevActiveCardID = self.activeCardID;

	static_cast<CorePanelFull&>(self).remove(cardIt->second);
	self.cardIDMapping.erase(cardIt);

	if (cardID != prevActiveCardID)
		return;

	const auto findNextCondition = []<class C>(C& container, C::iterator it, auto&& condition) {
		for (auto i = it; i != container.end(); ++i)
			if (condition(*i))
				return i;

		for (auto i = it; i-- != container.begin();)
			if (condition(*i))
				return i;

		return container.end();
	};

	const auto nextCardIt = findNextCondition(self.cardIDMapping, cardIt, [](const auto& pair) {
		return !AccessParent::isPendingDetachSelf(pair.second.core());
	});

	if (nextCardIt == self.cardIDMapping.end()) {
		self.activeCardID = nullCardID;

	} else {
		nextCardIt->second.enable(true);
		nextCardIt->second.show(true);

		self.activeCardID = nextCardIt->first;
	}

	self.flagAuto(Flag::pendingRender);
	self.fire(EventPanelCardChange{{}, prevActiveCardID, self.activeCardID});
}

void PanelCard::remove(Component& component) {
	self().remove(component);

	const auto cardIt = libv::linear_find_iterator(self().cardIDMapping, component, &decltype(self().cardIDMapping)::value_type::second);
	if (cardIt == self().cardIDMapping.end()) {
		log_ui.error("Attempted to remove a component {} that is not part of the panel in {}", component.path(), path());
		return;
	}

	aux_remove(self(), cardIt);
}

void PanelCard::remove(std::string_view component_name) {
	const auto cardIt = libv::linear_find_iterator(self().cardIDMapping, component_name, [](auto& v){ return v.second.name(); });
	if (cardIt == self().cardIDMapping.end()) {
		log_ui.error("Attempted to remove a component with name {} that is not part of the panel in {}", component_name, path());
		return;
	}

	aux_remove(self(), cardIt);
}

void PanelCard::remove(CardID cardID) {
	const auto cardIt = self().cardIDMapping.find(cardID);
	if (cardIt == self().cardIDMapping.end()) {
		log_ui.error("Attempted to remove a card with ID {} that is not part of the panel in {}", libv::to_underlying(cardID), path());
		return;
	}

	aux_remove(self(), cardIt);
}

void PanelCard::clear() {
	self().clear();
	self().cardIDMapping.clear();
	self().activeCardID = nullCardID;
	self().nextCardID = firstCardID;
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
