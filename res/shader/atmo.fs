#version 330 core

const float pi = 3.14159265358979323846264338327950288;
const float e = 2.71828182845904523536028747135266249;

//uniform sampler2D textureSamplerAtmo;

in vec3 fragmentPositionW;

uniform float planetRadius;
uniform vec3 planetPosition;
uniform vec3 cameraPositionW;

uniform float atmosphereRadius; /// upper limit of the atmosphere
uniform float atmosphereFactor; /// recomended range [-8, -1] used as e^factor (handles [-inf, inf])

//uniform float atmosphereDensity;
//uniform float atmosphereThickness;


// -------------------------------------------------------------------------------------------------
//
//               dive <------------------------------------->
//
//                               ..................
//              enter       .....                  ....       exit
//                  \    ...                           ...    /
//                   \ ..                                 .. /
// -ray----x---------.x------------------------------------>x.      ^ ^
//        /         .                                         .     | | lowest altitude
//       /         .                  #######                  .    | v
//  camera        .               ####       ####               .   |
//               .              ##               ##              .  |
//               .             #                   #             .  |
//               .            #                     #            .  | ray distance
//               .            #          x          #            .  v
//               .            #         /           #            .
//               .             #      center       #             .
//               .              ##               ##              .
//                .               ####       ####               .
//                 .             /    #######                  .    ^
//                  .           /                             .     | atmosphere height
//                   ..       planet                        ..      |
//                     ..                                 ..        |
//                       ...                           ...          |
//                          .....                  ....             |
//                         /     ..................                 v
//                        /
//                     atmosphere        <----------> planet radius
//
// -------------------------------------------------------------------------------------------------

//                                      /                         /
//                               ....../...........              /
//                          .....     /            ....         /
//                       ...         /                 ...     /
//                     ..           /                     ..  /
// -ray----x---------..------------/----------------------->./
//        /         .             /                         / .
//       /         .             /    #######              /   .
//  camera        .             / ####       ####         /     .
//               .              ##               ##      /       .
//               .             #                   #    /        .
//               .            #                     #  /         .
//               .            #          x          # /          .
//               .            #                     #/           .
//               .             #                   #             .
//               .              ##               ##              .
//                .               ####       ####               .
//                 .                  #######                  .
//                  .                                         .
//                   ..        ^                            ..
//                     ..     / sun direction             ..
//                       ... /                         ...
//                          /....                  ....
//                         /     ..................
//                 sun ---x

//                                             ^
//                               ............./....
//                          .....            /     ....
//                       ...                /          ...
//                     ..                  /              ..
//                   ..                   /                 ..
//                  .                    /                    .
//                 .                  ##/####                  .
//                .               #### /     ####               .
//               .              ##    /          ##              .
//               .             #     /             #             .
//               .            #     /               #            .
//               .            #    /     x          #            .
//               .            #   /                 #            .
//               .             # /                 #             .
//               .              /#               ##              .
//                .            /  ####       ####               .
//                 .          /       #######                  .
//                  .        /                                .
//                   ..     /                               ..
//                     ..  /                              ..
//                       ./.                           ...
//                       /  .....                  ....
//                      /        ..................
//            camera --x
//                    /


//https://gist.github.com/BeRo1985/5042cc79e55012a2c724aecf880c8be9

/// from and to vectors has to be unit vectors
vec4 quaternionFromToRotation(vec3 from, vec3 to) {
  return vec4(cross(from, to), dot(from, to));
}

vec3 q_rotate(vec3 v, vec4 q) {
	return v + 2.0 * cross(q.xyz, cross(q.xyz, v) + q.w * v);
}

vec4 q_inverse(vec4 q) {
 return vec4(-q.xyz, q.w) / length(q);
}

//vec3 solve_quadratic(float a, float b, float c) {
//	float test = b * b - 4.0 * a * c;
//
//	if (test > 0.0) {
//		float sqrtTest = sqrt(test);
//		float t0 = (-b - sqrtTest) / (2.0 * a);
//		float t1 = (-b + sqrtTest) / (2.0 * a);
//		return vec3(test, t0, t1);
//
//	} else if (test == 0.0) {
//		float t0 = (-b - sqrtTest) / (2.0 * a);
//		return vec3(test, t0, 0.0);
//
//	} else {
//		return vec3(test, 0.0, 0.0);
//	}
//}


void main() {
	//color = texture(textureSkySampler, normalize(fragmentPosW - eyePosW)).rgba;

	vec3 camera = cameraPositionW;
	//vec3 enter;
	vec3 exit = fragmentPositionW;
	vec3 center = planetPosition;

	float atmosphereHeight = atmosphereRadius - planetRadius;
	float rayLength = length(exit - camera);
	float rayDistance = length(cross(center - camera, center - exit)) / length(exit - camera);
	vec3 rayDirection = normalize(exit - camera);
	float dive = sqrt(planetRadius * planetRadius + rayDistance * rayDistance) * 2.0;
	float lowestAltitude = rayDistance - planetRadius;

	float lowestAltitudeRatio = lowestAltitude / atmosphereHeight;
	float maxDensity = (pow(e, lowestAltitudeRatio * atmosphereFactor) - pow(e, atmosphereFactor)) / (1.0 - pow(e, atmosphereFactor));
	//color = vec4(pow(texture(textureSkySampler, normalize(fragmentPosW - eyePosW)).rgb, vec3(1/2.2)), 1);

	for (int i = 0; i < num_sun; ++i) {
		//float shadowEnter;
		//float shadowExit;
		//vec3 sun.direction;
		//vec3 sun.position;

		vec4 toShadowSpaceRotation = quaternionFromToRotation(sun.direction, rayDirection);
		vec3 toShadowSpacePosition = center - camera;

		vec3 cameraL = rotate_q(camera + toShadowSpacePosition, toShadowSpaceRotation);
		vec3 exitL = rotate_q(exit + toShadowSpacePosition, toShadowSpaceRotation);
		//vec3 centerL = rotate_q(center + toShadowSpacePosition, toShadowSpaceRotation);

		vec2 p1 = cameraL.xy;
		vec2 p2 = exitL.xy;
		vec2 p3 = vec2(0.0, 0.0); // = centerL.xy;
		float r = planetRadius;

		vec2 d = p2 - p1;
		float a = dot(d, d);
		float b = 2.0 * dot(d, p1 - center);
		float c = dot(p3, p3) + dot(p1, p1) - 2.0 * dot(p3, p1) - r * r;

		float test = b * b - 4.0 * a * c;

		if (test > 0.0) { // Tangent case is irrelevant
			float sqrtTest = sqrt(test);
			float t0 = (-b - sqrtTest) / (2.0 * a);
			float t1 = (-b + sqrtTest) / (2.0 * a);

			t0 = max(t0, 0.0); // Exclude behind the camera
			t1 = min(t1, 1.0); // Exclude outside of the atmosphere

			t0 = t0 * dive / rayLength + (1 - dive / rayLength);
			t1 = t1 * dive / rayLength + (1 - dive / rayLength);
			//vec2 hitp0 = p2 + t0 * (p2 - p1);
			//vec2 hitp2 = p2 + t1 * (p2 - p1);
		}

		//float shadowEnter;
		//float shadowExit;
	}
}

/*
// main
void main() {
    float entryCameraDot = dot(directionEntry, directionCamera);
    float lowestIntersection = sqrt(planetRadiusAtmosphere * planetRadiusAtmosphere - entryCameraDot * entryCameraDot);

    float atmosphereRatio = 1.0 / (planetRadiusAtmosphere - planetRadiusSurface) / planetRadiusAtmosphere;
    float rayInAtmosphere = (1.0 - lowestIntersection) * atmosphereRatio;

    if (rayInAtmosphere > 1.0)
        rayInAtmosphere *= 0.5;
    else
        rayInAtmosphere = 1.0 - rayInAtmosphere;

    gl_FragColor.rgb = mix(color1, color2, rayInAtmosphere);
	gl_FragColor.a = (1.0 - rayInAtmosphere) * atmosphereThickness;
}

*/





//                               ..................
//                          .....                  ....
//                       ...                           ...
//                     ..                                 ..
//                   ..                                     ..
//                  .                                         .
//                 .                  #######                  .
//                .               ####       ####               .
//               .              ##               ##              .
//               .             #                   #             .
//               .            #                     #            .
//               .            #                     #            .
//               .            #                     #            .
//               .             #                   #             .
//               .              ##               ##              .
//                .               ####       ####               .
//                 .                  #######                  .
//                  .                                         .
//                   ..                                     ..
//                     ..                                 ..
//                       ...                           ...
//                          .....                  ....
//                               ..................
