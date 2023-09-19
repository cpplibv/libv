//

#pragma once

#ifndef BUILTIN_BLOCK_IDENTIFIER_MODEL
#define BUILTIN_BLOCK_IDENTIFIER_MODEL
#endif


layout(std140) uniform Model300 {
	mat4 matMV;
	mat4 matM;
	mat4 matMInverse;

	// === 192 Byte

	// 64 Byte extra space
} BUILTIN_BLOCK_IDENTIFIER_MODEL;
