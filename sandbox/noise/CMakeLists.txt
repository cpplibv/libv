
wish_create_executable(
		TARGET sandbox_libv_noise_view
		OUTPUT_NAME noise
		SOURCE *.cpp
		LINK libv::ui libv::frame libv::ctrl libv::log libv::rev libv::sun libv::noise
)

wish_resource_mapping(
		TARGET sandbox_libv_noise_view
		FILE_NAME wish/resource_path.hpp
		FUNCTION_NAME wish::resource_path

		GLOB res/style.lua
		GLOB res/shader/noise_gen.cs

#		GLOB ../../res/font/arialn.ttf
#		GLOB ../../res/font/calibril.ttf
		GLOB ../../res/font/consola.ttf
		GLOB ../../res/shader/rev_sandbox/particle.fs
		GLOB ../../res/shader/rev_sandbox/particle.vs

		GLOB ../../res/shader/lib/noise/noise.glsl
		GLOB ../../res/shader/lib/noise/__fastnoiselite.glsl

		REPLACE ../../res/font/ res/font/
		REPLACE ../../res/shader/rev_sandbox/ res/shader/
)

wish_package(
		TARGET sandbox_libv_noise_view
		PACKAGE_DESTINATION package/sandbox/noise
)
