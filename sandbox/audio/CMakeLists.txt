
wish_create_executable(
		TARGET sandbox_libv_audio
		OUTPUT_NAME audio
		SOURCE *.cpp
#		LINK libv::ui libv::frame libv::ctrl libv::log libv::sun libv::noise
		LINK libv::audio libv::res libv::log
		LINK ext_soloud # Wont be needed if libv::audio completes the wrapping
)

wish_resource_mapping(
		TARGET sandbox_libv_audio
		MAPPING_FILE wish/resource_mapping.hpp
		MAPPING_FUNCTION wish::resource_mapping

		RESOURCE res/style.lua
)

wish_package(
		TARGET sandbox_libv_audio
		PACKAGE_DESTINATION package/sandbox/libv_audio
)
