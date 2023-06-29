

wish_create_executable(
	TARGET test_star
	SOURCE *.cpp
	SOURCE ../star/game/surface/*.cpp
	LINK   ext_catch

	LINK   libv::log libv::sys
	LINK   libv::frame libv::ctrl libv::ui
	LINK   libv::sun libv::rev libv::glr libv::color
	LINK   libv::serial
	LINK   libv::noise
)
