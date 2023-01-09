
wish_create_executable(
		TARGET sandbox_libv_rev_particle
		OUTPUT_NAME rev_particle
		SOURCE *.cpp
		LINK libv::ui libv::frame libv::ctrl libv::log libv::rev libv::sun libv::noise
)

wish_resource_mapping(
		TARGET sandbox_libv_rev_particle
		FILE_NAME wish/resource_path.hpp
		FUNCTION_NAME wish::resource_path

		GLOB res/style.lua
#		GLOB ../../res/font/arialn.ttf
#		GLOB ../../res/font/calibril.ttf
		GLOB ../../res/font/consola.ttf
		GLOB ../../res/shader/rev_sandbox/particle.fs
		GLOB ../../res/shader/rev_sandbox/particle.vs

		REPLACE ../../res/font/ res/font/
		REPLACE ../../res/shader/rev_sandbox/ res/shader/
)

wish_package(
		TARGET sandbox_libv_rev_particle
		PACKAGE_DESTINATION package/sandbox/libv_rev_particle
)
