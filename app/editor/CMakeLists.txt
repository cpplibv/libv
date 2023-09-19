#

wish_create_executable(
		TARGET editor
		SOURCE editor_main.cpp
		LINK ext_ctre
		LINK libv::arg
		LINK libv::log libv::sys
		LINK libv::utility
		LINK libv::range
#		LINK libv::mt
#		LINK libv::frame libv::ctrl libv::ui
#		LINK libv::sun libv::rev libv::glr libv::color
#		LINK libv::serial
#		LINK libv::noise
)

wish_package(
		TARGET editor
		PACKAGE_DESTINATION package/editor
)
