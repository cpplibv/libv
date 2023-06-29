
wish_create_executable(
		TARGET sandbox_libv_noise_view
		OUTPUT_NAME noise
		SOURCE *.cpp
		LINK libv::ui libv::frame libv::ctrl libv::log libv::rev libv::sun libv::noise
)

wish_resource_mapping(
		TARGET sandbox_libv_noise_view
		MAPPING_FILE wish/resource_mapping.hpp
		MAPPING_FUNCTION wish::resource_mapping

		RESOURCE res/style.lua
		RESOURCE res/shader/noise_gen.cs
#		RESOURCE @/res/font/arialn.ttf
#		RESOURCE @/res/font/calibril.ttf
		RESOURCE @/res/font/consola.ttf
		RESOURCE @/res/shader/lib/noise/__fastnoiselite.glsl
		RESOURCE @/res/shader/lib/noise/noise.glsl
		RESOURCE @/res/texture/light_border.png
		RESOURCE @/res/texture/stripes_border.png
)

wish_package(
		TARGET sandbox_libv_noise_view
		PACKAGE_DESTINATION package/sandbox/noise
)
