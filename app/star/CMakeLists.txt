

wish_create_executable(
	TARGET star
	SOURCE *.cpp
#	GENERATE codegen app/star/message/*.in.lua
	LINK   libv::log libv::sys
	LINK   libv::frame libv::ctrl libv::ui
	LINK   libv::sun libv::rev libv::glr libv::color
	LINK   libv::serial
#	LINK   libv::lua
#	LINK   libv::net
#	LINK   libv::vm4
)

wish_resource_mapping(
		TARGET star
		FILE_NAME wish/resource_path.hpp
		FUNCTION_NAME wish::resource_path

		GLOB res/style.lua
#		GLOB ../../res/font/arialn.ttf
#		GLOB ../../res/font/calibril.ttf
		GLOB ../../res/font/consola.ttf
#		GLOB ../../res/shader/rev_sandbox/particle.fs
#		GLOB ../../res/shader/rev_sandbox/particle.vs

		REPLACE ../../res/font/ res/font/
		REPLACE ../../res/model/ res/model/
		REPLACE ../../res/script/ res/script/
		REPLACE ../../res/shader/rev_sandbox/ res/shader/
		REPLACE ../../res/texture/ res/texture/
)

#wish_package(
#		TARGET star
#		PACKAGE_DESTINATION package/sandbox/libv_rev_particle
#)
