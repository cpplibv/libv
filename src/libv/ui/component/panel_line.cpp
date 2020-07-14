// File: label.cpp - Created on 2017.11.05. 07:52 - Author: Vader

// hpp
#include <libv/ui/component/panel_line.hpp>
// ext
#include <boost/container/small_vector.hpp>
#include <range/v3/view/zip.hpp>
// libv
#include <libv/utility/approx.hpp>
#include <libv/utility/enum.hpp>
#include <libv/utility/min_max.hpp>
// pro
#include <libv/ui/core_component.hpp>
#include <libv/ui/component/base_panel.lpp>
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/context/context_style.hpp>
#include <libv/ui/layout/view_layouted.lpp>
#include <libv/ui/log.hpp>
#include <libv/ui/property.hpp>
#include <libv/ui/property_access_context.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class CorePanelLine : public CoreBasePanel {
	friend class PanelLine;
	[[nodiscard]] inline auto handler() { return PanelLine{this}; }

private:
	struct Properties {
		PropertyL<AlignHorizontal> align_horizontal;
		PropertyL<AlignVertical> align_vertical;
		PropertyL<Orientation> orientation;
	} property;

	struct ChildProperties {
	};

	template <typename T> static void access_properties(T& ctx);
	template <typename T> static void access_child_properties(T& ctx);

//	static ComponentPropertyDescription description;
//	static ComponentPropertyDescription child_description;

public:
	using CoreBasePanel::CoreBasePanel;

private:
	virtual void doStyle(ContextStyle& context) override;
	virtual void doStyle(ContextStyle& context, ChildID childID) override;
	virtual libv::vec3f doLayout1(const ContextLayout1& le) override;
	virtual void doLayout2(const ContextLayout2& le) override;
};

// -------------------------------------------------------------------------------------------------

namespace {

struct OrientationData {
	uint32_t _X_;
	uint32_t _Y_;
	uint32_t _Z_;
	libv::vec3f start;
	libv::vec3f direction;
	libv::vec3f penCorner;
};

static constexpr OrientationData OrientationTable[] = {
	{1u, 0u, 2u, {0.f, 0.f, 0.f}, {+1.f, +1.f, +1.f}, {+0.f, +0.f, +0.f}}, // BOTTOM_TO_TOP;
	{0u, 1u, 2u, {0.f, 0.f, 0.f}, {+1.f, +1.f, +1.f}, {+0.f, +0.f, +0.f}}, // LEFT_TO_RIGHT;
	{0u, 1u, 2u, {1.f, 0.f, 0.f}, {-1.f, +1.f, +1.f}, {-1.f, +0.f, +0.f}}, // RIGHT_TO_LEFT;
	{1u, 0u, 2u, {0.f, 1.f, 0.f}, {+1.f, -1.f, +1.f}, {+0.f, -1.f, +0.f}}, // TOP_TO_BOTTOM;
};

struct AlignmentData {
	float scale;
	float justifyGap;
	bool justified;
};

static constexpr AlignmentData AlignmentTableH[] = {
	{0.0f, 0.0f, false}, // Left
	{0.5f, 0.0f, false}, // Center
	{1.0f, 0.0f, false}, // Right
	{0.0f, 1.0f, true}, // Justify
	{0.0f, 1.0f, true}, // JustifyAll
};

static constexpr AlignmentData AlignmentTableV[] = {
	{1.0f, 0.0f, false}, // Top
	{0.5f, 0.0f, false}, // Center
	{0.0f, 0.0f, false}, // Bottom
	{0.0f, 1.0f, true}, // Justify
	{0.0f, 1.0f, true}, // JustifyAll
};

} // namespace

// -------------------------------------------------------------------------------------------------

template <typename T>
void CorePanelLine::access_properties(T& ctx) {
	ctx.property(
			[](auto& c) -> auto& { return c.align_horizontal; },
			AlignHorizontal::left,
			pgr::layout, pnm::align_horizontal,
			"Horizontal align of the inner content of the component"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.align_vertical; },
			AlignVertical::top,
			pgr::layout, pnm::align_vertical,
			"Vertical align of the inner content of the component"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.orientation; },
			Orientation::LEFT_TO_RIGHT,
			pgr::layout, pnm::orientation,
			"Orientation of subsequent components"
	);
}

template <typename T>
void CorePanelLine::access_child_properties(T& ctx) {
	(void) ctx;
}

// -------------------------------------------------------------------------------------------------

void CorePanelLine::doStyle(ContextStyle& ctx) {
	PropertyAccessContext<Properties> setter{property, ctx.component, ctx.style, ctx.component.context()};
	access_properties(setter);
	CoreComponent::access_properties(setter);
}

void CorePanelLine::doStyle(ContextStyle& ctx, ChildID childID) {
	(void) ctx;
	(void) childID;
}

// -------------------------------------------------------------------------------------------------

libv::vec3f CorePanelLine::doLayout1(const ContextLayout1& layout_env) {
	const auto env_size = layout_env.size - padding_size3();

	const auto& orientData = OrientationTable[libv::to_value(property.orientation())];
	const auto _X_ = orientData._X_;
	const auto _Y_ = orientData._Y_;
	const auto _Z_ = orientData._Z_;

	auto result = libv::vec3f{};
	auto pixelX = 0.f;
	auto percentX = 0.f;
	auto contentX = 0.f;

	const auto resolvePercent = [](const float fix, const float percent, auto& component) {
		if (fix == libv::Approx(0.f)) {
			return fix;
		} else if (percent == libv::Approx(100.f)) {
			log_ui.warn("Invalid sum of size percent {} with fixed width of {} during layout of {}", percent, fix, component.path());
			return fix * 2.f;
		} else {
			return fix / (1.f - percent * 0.01f);
		}
	};

	for (auto& child : children | view_layouted()) {
		const auto child_dynamic = child.size().has_dynamic() ?
				AccessLayout::layout1(child.core(), ContextLayout1{env_size}) :
				libv::vec3f{};

		pixelX += child.size()[_X_].pixel;
		percentX += child.size()[_X_].percent;
		contentX += (child.size()[_X_].dynamic ? child_dynamic[_X_] : 0.0f);

		result[_Y_] = libv::max(result[_Y_],
				resolvePercent(
				child.size()[_Y_].pixel + (child.size()[_Y_].dynamic ? child_dynamic[_Y_] : 0.0f),
				child.size()[_Y_].percent, child));

		result[_Z_] = libv::max(result[_Z_],
				resolvePercent(
				child.size()[_Z_].pixel + (child.size()[_Z_].dynamic ? child_dynamic[_Z_] : 0.0f),
				child.size()[_Z_].percent, child));
	}

	result[_X_] = resolvePercent(pixelX + contentX, percentX, *this);

	return result + padding_size3();
}

void CorePanelLine::doLayout2(const ContextLayout2& layout_env) {
	const auto env_size = layout_env.size - padding_size3();

	const auto& alignHData = AlignmentTableH[property.align_horizontal()];
	const auto& alignVData = AlignmentTableV[property.align_vertical()];
	const auto& orientData = OrientationTable[libv::to_value(property.orientation())];

	const auto _X_ = orientData._X_;
	const auto _Y_ = orientData._Y_;
	const auto _Z_ = orientData._Z_;

	// Size ---

	auto contentX = 0.f;
	auto sumRatioX = 0.f;

	// Note: This reserve may allocate more memory as children may include non-layouted components too, but that is rare and saves a pass
	boost::container::small_vector<libv::vec3f, 32> childSizes(children.size(), libv::vec3f{});

	for (auto&& [child, childSize] : ranges::view::zip(children | view_layouted(), childSizes)) {
		const auto child_dynamic = child.size().has_dynamic() ?
				AccessLayout::layout1(child.core(), ContextLayout1{env_size}) :
				libv::vec3f{};

		libv::meta::n_times_index<3>([&](auto&& i) {
			childSize[i] =
					child.size()[i].pixel +
					child.size()[i].percent * 0.01f * env_size[i] +
					(child.size()[i].dynamic ? child_dynamic[i] : 0.f);
		});

		sumRatioX += child.size()[_X_].ratio;
		contentX += childSize[_X_];
	}

	auto leftoverX = env_size[_X_] - contentX;
	auto ratioScalerX = sumRatioX > 0.f ? leftoverX / sumRatioX : 0.f;
	auto sizeContent = vec3f{};

	for (auto&& [child, childSize] : ranges::view::zip(children | view_layouted(), childSizes)) {
		childSize[_X_] += ratioScalerX * child.size()[_X_].ratio;
		if (child.size()[_Y_].ratio > 0.f) childSize[_Y_] = env_size[_Y_];
		if (child.size()[_Z_].ratio > 0.f) childSize[_Z_] = env_size[_Z_];

		sizeContent[_X_] += childSize[_X_];
		sizeContent[_Y_] = std::max(sizeContent[_Y_], childSize[_Y_]);
		sizeContent[_Z_] = std::max(sizeContent[_Z_], childSize[_Z_]);
	}

	// Position ---

	auto environmentUnused = env_size - sizeContent;
	const auto justifyGapX = alignHData.justifyGap * environmentUnused[_X_] / std::max(static_cast<float>(children.size()) - 1.0f, 1.0f);
	if (alignHData.justified)
		environmentUnused[_X_] = 0.0f;

	const auto aligmentScale = vec3f{alignHData.scale, alignVData.scale, 0.0f};

	const auto originToContent = environmentUnused * aligmentScale;
	const auto contentToStart = sizeContent * orientData.start;

	auto startToPen = vec3f{};

	for (auto&& [child, childSize] : ranges::view::zip(children | view_layouted(), childSizes)) {
		const auto penToBase = childSize * orientData.penCorner;
		auto baseToPosition = vec3f{};
		baseToPosition[_Y_] = (sizeContent[_Y_] - childSize[_Y_]) * aligmentScale[_Y_];
		const auto position = padding_LB3() + originToContent + contentToStart + startToPen + penToBase + baseToPosition;

		startToPen[_X_] += orientData.direction[_X_] * childSize[_X_] + justifyGapX;

		const auto roundedPosition = libv::vec::round(position);
		const auto roundedSize = libv::vec::round(position + childSize) - roundedPosition;

		AccessLayout::layout2(
				child.core(),
				layout_env.enter(roundedPosition, roundedSize)
		);
	}
}

// =================================================================================================

PanelLine::PanelLine(std::string name) :
	ComponentHandler<CorePanelLine, EventHostGeneral<PanelLine>>(std::move(name)) { }

PanelLine::PanelLine(GenerateName_t gen, const std::string_view type) :
	ComponentHandler<CorePanelLine, EventHostGeneral<PanelLine>>(gen, type) { }

PanelLine::PanelLine(core_ptr core) noexcept :
	ComponentHandler<CorePanelLine, EventHostGeneral<PanelLine>>(core) { }

// -------------------------------------------------------------------------------------------------

void PanelLine::align_horizontal(AlignHorizontal value) {
	AccessProperty::manual(self(), self().property.align_horizontal, value);
}

AlignHorizontal PanelLine::align_horizontal() const noexcept {
	return self().property.align_horizontal();
}

void PanelLine::align_vertical(AlignVertical value) {
	AccessProperty::manual(self(), self().property.align_vertical, value);
}

AlignVertical PanelLine::align_vertical() const noexcept {
	return self().property.align_vertical();
}

void PanelLine::orientation(Orientation value) {
	AccessProperty::manual(self(), self().property.orientation, value);
}

Orientation PanelLine::orientation() const noexcept {
	return self().property.orientation();
}

// -------------------------------------------------------------------------------------------------

void PanelLine::add(Component component) {
	self().add(std::move(component));
}

void PanelLine::remove(Component& component) {
	self().remove(component);
}

void PanelLine::clear() {
	self().clear();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
