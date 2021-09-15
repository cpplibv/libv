local abs = math.abs
local cos = math.cos
local max = math.max
local min = math.min
local pi = math.pi
local sin = math.sin
local sqrt = math.sqrt

-- -------------------------------------------------------------------------------------------------

local rgba = function(r, g, b, a)
	return vec4f(r, g, b, a);
end

-- -------------------------------------------------------------------------------------------------

local box = function(pos, size)
	return { type = "rounded_box", pos = pos, size = size, corner = 0, sharpness = 1 }
end

local rounded_box = function(pos, size, corner, sharpness)
	return { type = "rounded_box", pos = pos, size = size, corner = corner, sharpness = sharpness }
end

local glow = function(size, falloff, color)
	return { type = "glow", size = size, falloff = falloff, color = color }
end

-- -------------------------------------------------------------------------------------------------

local merge = function(base, derived)
	local result = {}
	for k,v in pairs(base) do
		result[k] = v
	end
	for k,v in pairs(derived) do
		if (type(k) == "number") then
			table.insert(result, v)
		else
			result[k] = v
		end
	end
	return result
end

-- -------------------------------------------------------------------------------------------------

register_var("test_size", 0, 8, 0.1, 1);
register_var("test_falloff", 0.1, 16, 0.1, 1);

register_var("normal_os_size", 0, 8, 0.1, 3.5);
register_var("normal_is_size", 0, 8, 0.1, -3.5);
register_var("normal_oe_size", 0, 8, 0.1, 1.5);
register_var("normal_ie_size", 0, 8, 0.1, -1.5);

-- -------------------------------------------------------------------------------------------------

main = function (theme)
	local base_button = {
		rounded_box(vec2f(8, 8), vec2f(48, 48), 5.5, 16),
	}

	theme.texture_size(64, 64);

	theme.atlas("button.normal", merge(base_button, {
		--glow(3.5, 3, rgba(0, 0, 0, 0.5)),
		--glow(-1.5, 3, rgba(0.8, 0.8, 0.8, 0.8)),
		glow(test_size, test_falloff, rgba(1, 0, 0, 1)),
		glow(-test_size, test_falloff, rgba(0, 0, 1, 1)),
	}))
	--theme.atlas("button.normal", merge(base_button, {
	--	--glow(3.5, 3, rgba(0, 0, 0, 0.5)),
	--	--glow(-1.5, 3, rgba(0.8, 0.8, 0.8, 0.8)),
	--	glow( 2.5, 3, rgba(1, 0, 0, 1)),
	--	glow(-2.5, 3, rgba(0, 0, 1, 1)),
	--}))

	theme.atlas("button.pressed", merge(base_button, {
		glow( 2.5, 1, rgba(1, 0, 0, 1)),
		glow(-2.5, 1, rgba(0, 0, 1, 1)),
	}))

	theme.atlas("button.hover", merge(base_button, {
		glow( 3.5, 3, rgba(0, 1, 0, 0.5)),
		glow(-1.5, 3, rgba(0.8, 0.8, 0.8, 0.8)),
	}))

	theme.atlas("button.inactive", merge(base_button, {
		glow( 3.5, 3, rgba(0, 0, 1, 0.5)),
		glow(-1.5, 3, rgba(0.8, 0.8, 0.8, 0.8)),
	}))
end



---- outer_shadow
--add_effect(6.5, 3, vec4f(0.2, 0.2, 0.5, 0.5))
---- inner edge
--add_effect(-1.5, 1, vec4f(0.8, 0.8, 1, 0.8))

-- =================================================================================================

AlignHorizontal = {
	right
}

Anchor = {
	buttom_right
}

font = function(path)
	return path
end

size = function(str)
	return str
end

-- =================================================================================================

local foo = function(theme)
	theme.style("version-watermark", {
		align = AlignHorizontal.right,
		font = font("consola.ttf"),
		font_color = rgba(187/255, 191/255, 195/255, 0.75),
		font_size = FontSize { 10 },
		anchor = Anchor.buttom_right,
		size = size("d, d"),
	})

	theme.style("scroll-bar", {
		--align = 		AlignHorizontal.right,
		--font = 			font("consola.ttf"),
		--font_color = 	rgba(187, 191, 195, 0.75),
		--font_size =     FontSize{10},
		--anchor =		Anchor.buttom_right,
		--size =          size("d, d"),
		slider = {
			bar = {
			},
		},
		button_arrow_plus = {
		},
		--button_arrow_minus = {
		--},
	})

	theme.style("scroll-bar > button-increment", {
	})

	theme.style("scroll-bar > button-decrement", state.pressed, {
	})

	theme.style("button-small", {
		--style->set("align", libv::ui::AlignHorizontal::right);
		--style->set("font", context().font("consola.ttf"));
		--style->set("font_color", libv::parse::parse_color_or_throw("rgba(187, 191, 195, 75%)"));
		--style->set("font_size", libv::ui::FontSize{10});
		--style->set("anchor_target", libv::ui::Anchor{1.0f, 0.0f, 0.0f});
		--style->set("anchor_parent", libv::ui::Anchor{1.0f, 0.0f, 0.0f});
		--style->set("size", libv::ui::parse_size_or_throw("d, d"));
	})

	theme.style("overlay.notification.bubble", {
		--style->set("align", libv::ui::AlignHorizontal::center);
		--style->set("align_vertical", libv::ui::AlignVertical::center);
		--//		style->set("orientation", libv::ui::Orientation::down);
		--style->set("bg_color", libv::parse::parse_color_or_throw("rgba(255, 255, 255, 35%)"));
		--style->set("bg_shader", context().shaderQuad());
		--style->set("font", context().font("consola.ttf"));
		--style->set("font_color", libv::parse::parse_color_or_throw("rgba(187, 191, 195, 75%)"));
		--style->set("font_size", libv::ui::FontSize{16});
		--//		style->set("anchor_target", libv::ui::ANCHOR_TOP_CENTER);
		--//		style->set("anchor_parent", libv::ui::ANCHOR_TOP_CENTER);
		--style->set("size", libv::ui::parse_size_or_throw("d, d"));
	})

	theme.style("overlay.notification.text", {
		--//		style->set("align", libv::ui::AlignHorizontal::center);
		--//		style->set("align_vertical", libv::ui::AlignVertical::top);
		--//		style->set("orientation", libv::ui::Orientation::down);
		--//		style->set("bg_color", libv::parse::parse_color_or_throw("rgba(255, 255, 255, 35%)"));
		--//		style->set("bg_shader", context().shaderQuad());
		--//		style->set("font", context().font("consola.ttf"));
		--//		style->set("font_color", libv::parse::parse_color_or_throw("rgba(187, 191, 195, 75%)"));
		--//		style->set("font_size", libv::ui::FontSize{16});
		--//		style->set("anchor_target", libv::ui::ANCHOR_TOP_CENTER);
		--//		style->set("anchor_parent", libv::ui::ANCHOR_TOP_CENTER);
		--//		style->set("size", libv::ui::parse_size_or_throw("d, d"));
	})


	---fx-blend-mode
	--		[ add | blue | color-burn | color-dodge | darken | difference | exclusion | green | hard-light | lighten | multiply | overlay | red | screen | soft-light | src-atop | src-in | src-out | src-over ] 	null
	---fx-cursor
	--		[ null | crosshair | default | hand | move | e-resize | h-resize | ne-resize | nw-resize | n-resize | se-resize | sw-resize | s-resize | w-resize | v-resize | text | wait ] | <url> 	null 	  	inherits
	---fx-effect
	--		<effect> 	null

	--Effect
	--
	--	Drop Shadow
	--
	--		A high-level effect that renders a shadow of the given content behind the content.
	--
	--			dropshadow( <blur-type> , <color> , <number> , <number> , <number> , <number> )
	--
	--			<blur-type> = [ gaussian | one-pass-box | three-pass-box | two-pass-box ]
	--			<color> The shadow Color.
	--			<number> The radius of the shadow blur kernel. In the range [0.0 ... 127.0], typical value 10.
	--			<number> The spread of the shadow. The spread is the portion of the radius where the contribution of the source material will be 100%. The remaining portion of the radius will have a contribution controlled by the blur kernel. A spread of 0.0 will result in a distribution of the shadow determined entirely by the blur algorithm. A spread of 1.0 will result in a solid growth outward of the source material opacity to the limit of the radius with a very sharp cutoff to transparency at the radius. Values should be in the range [0.0 ... 1.0].
	--			<number> The shadow offset in the x direction, in pixels.
	--			<number> The shadow offset in the y direction, in pixels.
	--
	--	Inner Shadow
	--
	--		A high-level effect that renders a shadow inside the edges of the given content.
	--
	--			innershadow( <blur-type> , <color> , <number> , <number> , <number> , <number> )
	--
	--			<blur-type> = [ gaussian | one-pass-box | three-pass-box | two-pass-box ]
	--			<color> The shadow Color.
	--			<number> The radius of the shadow blur kernel. In the range [0.0 ... 127.0], typical value 10.
	--			<number> The choke of the shadow. The choke is the portion of the radius where the contribution of the source material will be 100%. The remaining portion of the radius will have a contribution controlled by the blur kernel. A choke of 0.0 will result in a distribution of the shadow determined entirely by the blur algorithm. A choke of 1.0 will result in a solid growth inward of the shadow from the edges to the limit of the radius with a very sharp cutoff to transparency inside the radius. Values should be in the range [0.0 ... 1.0].
	--			<number> The shadow offset in the x direction, in pixels.
	--			<number> The shadow offset in the y direction, in pixels.

	--dropshadow(two-pass-box , rgba(0, 0, 0, 0.2), 5, 0.0 , 0, 0);
	--dropshadow(three-pass-box, rgba(0, 0, 0, 0.5), 10, 0.0, 0, 3);
	--dropshadow(gaussian, #66afe9, 10, 0, 0, 0);
	--innershadow(gaussian, #204d74, 10, 0, 0, 3);

	--<color> | <linear-gradient> | <radial-gradient>
	--
	--	Linear Gradients <linear-gradient>
	--
	--		linear-gradient( [ [from <point> to <point>] | [ to <side-or-corner>], ]? [ [ repeat | reflect ], ]? <color-stop>[, <color-stop>]+)
	--
	--		where <side-or-corner> = [left | right] || [top | bottom]
	--
	--		Linear gradient creates a gradient going though all the stop colors along the line between the "from" <point> and the "to" <point>. If the points are percentages, then they are relative to the size of the area being filled. Percentage and length sizes can not be mixed in a single gradient function.
	--
	--		If neither repeat nor reflect are given, then the CycleMethod defaults "NO_CYCLE".
	--		If neither [from <point> to <point>] nor [ to <side-or-corner> ] are given, then the gradient direction defaults to 'to bottom'.
	--		Stops are per W3C color-stop syntax and are normalized accordingly.
	--
	--		This example will create a gradient from top left to bottom right of the filled area with red at the top left corner and black at the bottom right.
	--
	--		linear-gradient(to bottom right, red, black)
	--
	--		This is equivalent to:
	--
	--		linear-gradient(from 0% 0% to 100% 100%, red 0%, black 100%)
	--
	--		This more complex example will create a 50px high bar at the top with a 3 color gradient with white underneath for the rest of the filled area.
	--
	--		linear-gradient(from 0px 0px to 0px 50px, gray, darkgray 50%, dimgray 99%, white)
	--
	--		linear (<size>, <size>) to (<size>, <size>) stops [ (<number>,<color>) ]+ [ repeat | reflect ]?
	--
	--
	--	Radial Gradients <radial-gradient>
	--
	--		radial-gradient([ focus-angle <angle>, ]? [ focus-distance <percentage>, ]? [ center <point>, ]? radius [ <length> | <percentage> ] [ [ repeat | reflect ], ]? <color-stop>[, <color-stop>]+)
	--
	--		If neither repeat nor reflect are given, then the CycleMethod defaults "NO_CYCLE".
	--		Stops are per W3C color-stop syntax and are normalized accordingly.
	--
	--		Following are examples of the use of radial-gradient:
	--
	--		radial-gradient(radius 100%, red, darkgray, black)
	--
	--		radial-gradient(focus-angle 45deg, focus-distance 20%, center 25% 25%, radius 50%, reflect, gray, darkgray 75%, dimgray)


	--/*******************************************************************************
	--*                                                                             *
	--* CheckBox                                                                    *
	--*                                                                             *
	--******************************************************************************/
	--.check-box > .box {
	---fx-background-radius: 3;
	--/*-fx-padding: 0.166667em 0.166667em 0.25em 0.25em; !* 2 2 3 3 *!*/
	---fx-padding:0;
	---fx-border-color: #56c0e0;
	---fx-border-radius: 3;
	---fx-background-color: white;
	--}
	--.check-box > .box > .mark {
	---fx-background-color: null;
	---fx-padding: 0.416667em 0.416667em 0.5em 0.5em; /* 5 5 6 6 */
	---fx-shape: "M927.936 272.992l-68.288-68.288c-12.608-12.576-32.96-12.576-45.536 0l-409.44 409.44-194.752-196.16c-12.576-12.576-32.928-12.576-45.536 0l-68.288 68.288c-12.576 12.608-12.576 32.96 0 45.536l285.568 287.488c12.576 12.576 32.96 12.576 45.536 0l500.736-500.768c12.576-12.544 12.576-32.96 0-45.536z";
	---fx-background-insets: -3 -3 1 0;
	--}
	--.check-box {
	---fx-label-padding: 0.2em 0.0em 0.3em 0.416667em; /* 0 0 0 5 */
	---fx-text-fill: -fx-text-background-color;
	---fx-padding: 0 0 2 0;
	--}
	--.check-box:indeterminate > .box {
	---fx-padding: 0;
	--}
	--.check-box:selected > .box > .mark {
	---fx-background-color: linear-gradient(to bottom, #56c0e0, #40b8dc);
	--}
	--
	--/*******************************************************************************
	--*                                                                             *
	--* RadioButton                                                                 *
	--*                                                                             *
	--******************************************************************************/
	--
	--.radio-button {
	---fx-label-padding: 0.0em 0.0em 0.1em 0.416667em; /* 0 0 0 5 */
	---fx-text-fill: -fx-text-background-color;
	---fx-padding: 0 0 .5 0;
	--}
	--.radio-button > .radio,
	--.radio-button:focused > .radio  {
	---fx-border-color: #56c0e0;
	---fx-border-radius: 1em;
	---fx-background-radius: 1.0em; /* large value to make sure this remains circular */
	---fx-padding: 1 2 3 2;
	---fx-background-color: white;
	--}
	--.radio-button > .radio > .dot {
	---fx-background-color: transparent;
	---fx-background-radius: 1.0em; /* large value to make sure this remains circular */
	---fx-padding: 0.333333em; /* 4 -- radius of the inner black dot when selected */
	---fx-background-insets: 3 2 1 2;
	--}
	--.radio-button:selected > .radio,.radio-button:hover > .radio {
	---fx-fill-color: #56c0e0;
	--}
	--.radio-button:pressed > .radio {
	---fx-background-color: #50c0e2;
	--}
	--.radio-button:selected > .radio > .dot {
	---fx-background-color: #56c0e0;
	--}
	--
	--/*common things*/
	--.check-box:hover > .box,
	--.check-box:selected > .box,
	--.radio-button:hover > .radio,
	--.radio-button:selected > .radio {
	---fx-background-color: linear-gradient(to bottom, white, #efefef);
	--}
	--
	--.check-box:pressed > .box,
	--.radio-button:pressed > .radio {
	---fx-background-color: #50c0e2;
	--}
	--
	--/*******************************************************************************
	--*                                                                             *
	--* Slider                                                                      *
	--*                                                                             *
	--******************************************************************************/
	--
	--.slider .thumb {
	---fx-background-color: #dad9da, white, #3fbadf;
	---fx-background-insets: 0, 1, 5;
	---fx-effect: dropshadow(two-pass-box , rgba(0, 0, 0, 0.2), 5, 0.0 , 0, 0);
	--}
	--.slider .thumb:hover {
	---fx-effect: dropshadow(two-pass-box , rgba(0, 0, 0, 0.4), 5, 0.0 , 0, 0);
	--}
	--.slider .track {
	---fx-background-color: #dad9da, white;
	---fx-background-insets: -1, 0, 1;
	---fx-pref-height: 5;
	--}
	--.slider:vertical .track {
	---fx-pref-width: 5;
	--}

end
