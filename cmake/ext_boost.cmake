#

# --------------------------------------------------------------------------------------------------

set(Boost_Version 1.58.0)
string(REPLACE "." "_" Boost_Version_Underscore ${Boost_Version})

set(ext_boost_bootstrap_command)
if(UNIX)
  set(ext_boost_bootstrap_command ./bootstrap.sh)
  set(ext_boost_b2_command ./b2)
else()
  if(WIN32)
    set(ext_boost_bootstrap_command bootstrap.bat)
    set(ext_boost_b2_command b2.exe)
  endif()
endif()

#message( STATUS "Boost_Version: " ${Boost_Version} )
ExternalProject_Add(ext_boost
	PREFIX ${PATH_EXT_SRC}/boost
	URL http://downloads.sourceforge.net/project/boost/boost/${Boost_Version}/boost_${Boost_Version_Underscore}.zip
	URL_MD5 b0605a9323f1e960f7434dbbd95a7a5c
	CONFIGURE_COMMAND bootstrap.sh gcc
	BUILD_COMMAND b2
		install
		--toolset=gcc
		--variant=release
		--address-model=64
		--link=static
		--prefix=.
		--threading=multi
		--with-program_options
	BUILD_IN_SOURCE 1  #???
	INSTALL_COMMAND ""
)
set_property(TARGET ext_boost PROPERTY FOLDER "Externals")

ExternalProject_Get_Property(ext_boost source_dir)
ExternalProject_Get_Property(ext_boost binary_dir)
set(Boost_INCLUDE_DIRS ${source_dir})
#set(Boost_LIBRARIES debug;${binary_dir}/stage/lib/libboost_program_options-vc110-mt-gd-1_49.lib;optimized;${binary_dir}/stage/lib/libboost_program_options-vc110-mt-1_49.lib)
