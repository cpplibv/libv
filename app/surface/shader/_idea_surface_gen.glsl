#version 330 core

//#include <block/matrices.glsl>


in vec3 fragmentPositionW;

out float height;




//baseMicro = function(seed)
//	return add{
//		constant {
//			value = 0.5,
//		},
//		simplexFractal {
//			seed = seed,
//			octaves = 1, amplitude = 0.5, frequency = 0.2, lacunarity = 2.0, persistence = 0.5,
//		},
//	}
//end
//
//microRange = mul {
//	constant {
//		value = -0.5,
//	},
//	pow {
//		exponent = 0.2,
//
//		min {
//			baseMicro(2222),
//			min {
//				--baseMicro(3333),
//				--min {
//					baseMicro(4444),
//					min {
//						baseMicro(5555),
//						baseMicro(6666),
//					},
//				--},
//			},
//		},
//	},
//}


float simplexFractal(vec2 coord, int fractalParams) {
	return 0;
}

float coord(vec2 coord, float xw, float yw) {
	return 0;
}

vec2 simplexWarp(vec2 coord, int fractalParams) {
	return vec2(0, 0);
}

void main() {
	int fractalParams = 0;
	int warpParams = 0;
	vec2 c0 = vec2(x, y);

	vec2 c1 = simplexWarp(c0, warpParams);        // Node warp

	float v0 = simplexFractal(c1, fractalParams); // Node simplexFractal
	const float v1 = 0.5;                         // Node constant
	float v2 = v0 + v1;                           // Node add

	float v3 = simplexFractal(c1, fractalParams); // Node simplexFractal
	const float v4 = 0.5;                         // Node constant
	float v5 = v3 + v4;                           // Node add

	float v6 = min(v2, v5);                       // Node min

	float v7 = simplexFractal(c1, fractalParams); // Node simplexFractal
	const float v8 = 0.5;                         // Node constant
	float v9 = v7 + v8;                           // Node add

	float v10 = min(v6, v9);                      // Node min

	const float v21 = 0.5;                        // Node constant
	float v22 = pow(v10, v21);                    // Node pow

	float v30 = coord(c1, 0, 1);                  // Coord
	float v31 = v22 + v30;                        // Node add

	height = v50;
}
