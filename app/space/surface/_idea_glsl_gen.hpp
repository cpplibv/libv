// Project: libv, File: app/space/surface/_idea_glsl_gen.hpp

enum class Type {
	float_
	double_
	int_
	uint_
	vec2...
	vec3...
	vec4...
};

struct Input {
	Type type;
	Node* node;
	std::string uniform;
	std::string attribute;
};

//template <typename T>
struct InputVec2 {
	Type type;
	std::array<Input<T>, 2> ins{type};
};

struct NodeWarp {
	static inline int nextID = 0;
	std::string node_name = "warp" + nextID++;
//	InputVec2 input;
	input x, y;
//	output x, y;
	std::string glsl_output_var_;

	error connect(intput x, input y) {
		check(input.x.glsl_output_type() == type::float_);
		check(input.y.glsl_output_type() == type::float_);
	}
	std::string glsl_output_type() {
		return "vec2";
	}
	std::string glsl_output_var() {
		return glsl_output_var_;
	}
	std::string glsl() {
		return format("{} {} = warp({}, {}, ...)\n",
				glsl_output_type(),
				glsl_output_var(),
				input.x.glsl_output_var(),
				input.y.glsl_output_var()
		);
	}
};

// =================================================================================================

void foo() {
	//	add(warp(perlin()), perlin(), uniform_special_value)

	float uniform_special_value = ...;

	vec2 input_coordinates = fragmentUV;

	vec2 v000 = warp(input_coordinates.x, input_coordinates.y, ...);
	float v001 = perlin(v000.x, v000.y, ...);
	float v002 = perlin(input_coordinates.x, input_coordinates.y, ...);
	float v003 = v001 + v002 + uniform_special_value;

	return v003;
}
