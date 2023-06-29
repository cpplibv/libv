
wish_create_executable(
		TARGET sandbox_libv_rev_particle
		OUTPUT_NAME rev_particle
		SOURCE *.cpp
		LINK libv::ui libv::frame libv::ctrl libv::log libv::rev libv::sun libv::noise
)

wish_resource_mapping(
		TARGET sandbox_libv_rev_particle
		MAPPING_FILE wish/resource_mapping.hpp
		MAPPING_FUNCTION wish::resource_mapping

		RESOURCE res/style.lua
#		RESOURCE @/res/font/arialn.ttf
#		RESOURCE @/res/font/calibril.ttf
		RESOURCE @/res/font/consola.ttf
		RESOURCE @/res/shader/rev_sandbox/particle.fs
		RESOURCE @/res/shader/rev_sandbox/particle.vs
)

wish_package(
		TARGET sandbox_libv_rev_particle
		PACKAGE_DESTINATION package/sandbox/libv_rev_particle
)
