#

set(STAR_BUILD_VERSION_MAJOR 0 CACHE STRING "BUILD_VERSION_MAJOR")
set(STAR_BUILD_VERSION_MINOR 0 CACHE STRING "BUILD_VERSION_MINOR")
set(STAR_BUILD_VERSION_PATCH 0 CACHE STRING "BUILD_VERSION_PATCH")
set(STAR_BUILD_VERSION_NAME "version-name" CACHE STRING "BUILD_VERSION_NAME")
set(STAR_BUILD_VARIANT "dev" CACHE STRING "BUILD_VARIANT")
set(STAR_BUILD_ID 0 CACHE STRING "BUILD_ID")
set(STAR_BUILD_DATETIME "2000-01-01T00:00:00Z" CACHE STRING "BUILD_DATETIME")
set(STAR_BUILD_UUID 00000000-0000-0000-0000-000000000000 CACHE STRING "BUILD_UUID")

wish_create_library(
		TARGET star::game STATIC
		SOURCE game/*.cpp
		CONFIGURE_SOURCE game/version.cpp.in

		#	GENERATE codegen app/star/message/*.in.lua
		#	LINK   libv::lua
		#	LINK   libv::vm4
		LINK libv::log libv::sys
		LINK libv::utility
		LINK libv::mt
		LINK libv::frame libv::ctrl libv::ui
		LINK libv::sun libv::re libv::color
		LINK libv::serial
		LINK libv::noise
)

wish_create_executable(
		TARGET star
		SOURCE star_main.cpp
		LINK star::game libv::arg
)

wish_resource_mapping(
		TARGET star
		MAPPING_FILE wish/resource_mapping.hpp
		MAPPING_FUNCTION wish::resource_mapping

		RESOURCE res/style/*.lua
		RESOURCE @/res/font/arialn.ttf
		RESOURCE @/res/font/calibril.ttf
		RESOURCE @/res/font/consola.ttf
		RESOURCE @/res/texture/light_border.png
		RESOURCE @/res/texture/stripes_border.png

		# RE Defaults (With no scene)
		# TODO P5: Consider making this set empty
		RESOURCE @/res/shader/block300/pass300.glsl
		RESOURCE @/res/shader/block300/model300.glsl
		RESOURCE @/res/shader/re/depth_only.vs
		RESOURCE @/res/shader/re/depth_only_model.vs
		RESOURCE @/res/shader/re/solid.fs
		RESOURCE @/res/shader/re/solid.vs
		RESOURCE @/res/shader/re/solid_model.vs

		# RE Defaults (With an empty scene)
		# TODO P5: Consider making this set empty
		RESOURCE @/res/shader/lib/color_spaces.glsl
		RESOURCE @/res/shader/lib/constants.glsl
		RESOURCE @/res/shader/lib/equirectangular.glsl
		RESOURCE @/res/shader/lib/gamma.glsl
		RESOURCE @/res/shader/lib/resample.glsl
		RESOURCE @/res/shader/lib/tonemap.glsl
		RESOURCE @/res/shader/re/outline/jump_flood.glsl
		RESOURCE @/res/shader/re/outline/jump_flood_init.cs
		RESOURCE @/res/shader/re/outline/jump_flood_outline.fs
		RESOURCE @/res/shader/re/outline/jump_flood_step.cs
#		RESOURCE @/res/shader/re/outline/mask.fs
#		RESOURCE @/res/shader/re/outline/mask.vs
#		RESOURCE @/res/shader/re/outline/mask_model.vs
		RESOURCE @/res/shader/re/post_process/fullscreen.vs
		RESOURCE @/res/shader/re/post_process/pp_block.glsl
		RESOURCE @/res/shader/re/post_process/pp_bloom_down.cs
		RESOURCE @/res/shader/re/post_process/pp_bloom_down_prefilter.cs
		RESOURCE @/res/shader/re/post_process/pp_bloom_up.cs
		RESOURCE @/res/shader/re/post_process/pp_combine.fs
		RESOURCE @/res/shader/re/readback_sampler.cs

		# RE Surface
		RESOURCE @/res/shader/re/editor/editor_grid.fs
		RESOURCE @/res/shader/re/editor/editor_grid.vs
		RESOURCE @/res/shader/lib/constants.glsl
		RESOURCE @/res/shader/lib/mask_aa.glsl
		RESOURCE @/res/shader/lib/remap.glsl

		# RE ALL, reduce this set:
		RESOURCE @/res/shader/block300/light300.glsl
		RESOURCE @/res/shader/re/debug/debug_pass_capture.fs
		RESOURCE @/res/shader/re/debug/debug_view_texture.fs
		RESOURCE @/res/shader/re/lens_flare.fs
		RESOURCE @/res/shader/re/lens_flare.glsl
		RESOURCE @/res/shader/re/lens_flare.vs
		RESOURCE @/res/shader/re/lighting.glsl
		RESOURCE @/res/shader/re/model_static.fs
		RESOURCE @/res/shader/re/model_static.vs
		RESOURCE @/res/shader/re/outline/jump_flood.glsl
		RESOURCE @/res/shader/re/outline/jump_flood_init.cs
		RESOURCE @/res/shader/re/outline/jump_flood_outline.fs
		RESOURCE @/res/shader/re/outline/jump_flood_step.cs
		RESOURCE @/res/shader/re/outline/mask.fs
		RESOURCE @/res/shader/re/outline/mask.vs
		RESOURCE @/res/shader/re/outline/mask_model.vs
		RESOURCE @/res/shader/re/skybox.fs
		RESOURCE @/res/shader/re/skybox.vs
		RESOURCE @/res/shader/re/test/material_test0.fs
		RESOURCE @/res/shader/re/test/material_test0.vs
		RESOURCE @/res/shader/re/test/material_test0_model.vs

		RESOURCE @/res/texture/test_pattern_metal_a_2k_base_srgb.dds
		RESOURCE @/res/texture/test_pattern_metal_a_2k_normal.dds
		RESOURCE @/res/texture/test_pattern_metal_a_2k_rmao.dds
		RESOURCE @/res/texture/particle/circle_01.png
		RESOURCE @/res/texture/particle/circle_05.png
		RESOURCE @/res/texture/particle/star_01.png
		RESOURCE @/res/texture/particle/star_03.png
		RESOURCE @/res/texture/particle/star_06.png

#		RESOURCE @/res/texture/sky/debug_x_front_uv_cube.dds
		RESOURCE @/res/texture/sky/debug_x_front_uv_cube_srgb.dds
#		RESOURCE @/res/texture/sky/debug_x_front_uv_transparent_cube.dds
		RESOURCE @/res/texture/sky/debug_x_front_uv_transparent_cube_srgb.dds

		RESOURCE @/res/texture/sky/* # For testing skys only

		# Surface
		RESOURCE @/res/texture/sky/ambience_morning_green_cube_srgb.dds
		RESOURCE @/res/texture/sky/ambience_morning_green_cube_hdr.dds
		RESOURCE @/res/texture/sky/ambience_morning_green_eqrt_srgb.dds
		RESOURCE @/res/texture/sky/ambience_morning_green_eqrt_hdr.dds
		RESOURCE @/res/texture/sky/lago_disola_cube_srgb.dds
		RESOURCE @/res/texture/sky/lago_disola_cube_hdr.dds
		RESOURCE @/res/texture/sky/lago_disola_eqrt_srgb.dds
		RESOURCE @/res/texture/sky/lago_disola_eqrt_hdr.dds
		RESOURCE @/res/texture/sky/epping_forest_01_cube_hdr.dds

		# Testing only
		RESOURCE @/res/texture/stripes_border.png
		RESOURCE @/res/texture/hexagon_metal_0001_diffuse.dds
		RESOURCE @/res/model/building_delnan_16P_tex.0038_med.game.vm4
		RESOURCE @/res/model/fighter_01_eltanin.vm4
		RESOURCE @/res/model/test_sphere.vm4
		RESOURCE @/res/model/test_node_02_med.game.vm4
		RESOURCE @/res/model/test_node_med.game.vm4
		RESOURCE @/res/model/test_instance_node_med.game.vm4
		RESOURCE @/res/model/test_multi_material.vm4
#		RESOURCE @/res/texture/mip_tester_hexagon.dds
#		RESOURCE @/res/texture/separator_bar_256x16.png
#		RESOURCE @/res/texture/*
#		RESOURCE @/res/texture/reload_test.png

#		RESOURCE @/res/shader/builtin/red.fs
)

wish_package(
		TARGET star
		PACKAGE_DESTINATION package/star
)

wish_create_executable(
		TARGET test_star
		SOURCE test/*.cpp
		LINK ext_catch star::game
)
