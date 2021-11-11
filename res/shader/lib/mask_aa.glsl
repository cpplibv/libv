#pragma once

/// Anti-aliased mask creation from SDF with zero as limit
float mask_aa(in float sdf) {
	// Method A: fwidth and smoothstep
	// Best result
	// Origin: https://www.ronja-tutorials.com/post/034-2d-sdf-basics/
	float delta = fwidth(sdf) * 0.5;
	float mask_aa = smoothstep(delta, -delta, sdf);

	// Method B: division (Acegikmo)
	// Issue: Enlarges the mask with half a pixel and therefore causes some artifacts.
	// Origin: https://www.youtube.com/watch?v=mL8U8tIiRRg
	// float delta = fwidth(sdf);
	// float mask_aa = 1 - clamp(sdf / delta, 0, 1);

	// Method C: division with half pixel correction
	// Generates a bit blurrier/smoother edges, a good alternative to Method A
	// Based on: https://www.youtube.com/watch?v=mL8U8tIiRRg
	// float delta = fwidth(sdf);
	// float mask_aa = 1 - clamp(sdf / delta + 0.5, 0, 1);

	// Method D: step
	// To just disable AA
	// float mask_aa = step(sdf, 0);

	return mask_aa;
}
