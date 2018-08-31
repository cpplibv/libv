# File: target.cmake, Created on 2017. 04. 14. 16:49, Author: Vader


# --- Debug ----------------------------------------------------------------------------------------

set(__wish_global_debug 0)

macro(wish_enable_debug)
	set(__wish_global_debug 1)
endmacro()

macro(wish_disable_debug)
	set(__wish_global_debug 0)
endmacro()

# --- Group ----------------------------------------------------------------------------------------

set(__wish_current_group_stack)

macro(wish_create_group arg_GROUP)
	list(APPEND __wish_current_group_stack ${arg_GROUP})
endmacro()

macro(wish_end_group)
	list(REMOVE_AT __wish_current_group_stack -1)
endmacro()

# --- Group ----------------------------------------------------------------------------------------

set(__wish_static_link_std)

macro(wish_static_link_std)
	set(__wish_static_link_std -static)
endmacro()

# --- IDE ------------------------------------------------------------------------------------------

set(__wish_external_include_directories)
set(__wish_external_defines)

## Creates wish_ide target that can be used to obtain various informations IDE
function(wish_create_ide_target)
	# TODO P5: list targets / libraries / executables
	add_custom_target(wish_ide
		COMMAND ${CMAKE_COMMAND} -E echo "External include directories:"
		COMMAND ${CMAKE_COMMAND} -E echo "'${__wish_external_include_directories}'"
		COMMAND ${CMAKE_COMMAND} -E echo "External defines:"
		COMMAND ${CMAKE_COMMAND} -E echo "'${__wish_external_defines}'"
	)
endfunction()

# --- External -------------------------------------------------------------------------------------

## Defines get_NAME for fetching the ExternalProject and ext_NAME as lightweight INTERFACE target
## Unrecognized parameters after INCLUDE_DIR, LINK or DEFINE are forbidden.
function(wish_create_external)
	cmake_parse_arguments(arg "DEBUG;SKIP_CONFIGURE_AND_BUILD;SKIP_CONFIGURE;SKIP_BUILD" "NAME" "INCLUDE_DIR;LINK;DEFINE" ${ARGN})

	# options
	set(command_str_configure)
	set(command_str_build)
	if(arg_SKIP_CONFIGURE_AND_BUILD OR arg_SKIP_CONFIGURE)
		set(command_str_configure "CONFIGURE_COMMAND;echo;\"Skipping configure...\"")
	endif()
	if(arg_SKIP_CONFIGURE_AND_BUILD OR arg_SKIP_BUILD)
		set(command_str_build "BUILD_COMMAND;echo;\"Skipping build...\"")
	endif()

	# add
	ExternalProject_Add(
		get_${arg_NAME}
		PREFIX ${PATH_EXT_SRC}/${arg_NAME}
		CMAKE_ARGS
			-DCMAKE_INSTALL_PREFIX=${PATH_EXT}/${arg_NAME}
		#GIT_SHALLOW 1 # shallow fetch is not possible as long as SHA tags are used
		EXCLUDE_FROM_ALL 1
		${command_str_configure}
		${command_str_build}
		${arg_UNPARSED_ARGUMENTS}
	)
	add_library(ext_${arg_NAME} INTERFACE)

	# include
	if(NOT arg_INCLUDE_DIR)
		list(APPEND arg_INCLUDE_DIR include)
	endif()
	set(temp_list ${__wish_external_include_directories})
	foreach(var_include ${arg_INCLUDE_DIR})
		target_include_directories(ext_${arg_NAME} SYSTEM INTERFACE ${PATH_EXT}/${arg_NAME}/${var_include})
		list(APPEND temp_list ${PATH_EXT_IDE}/${arg_NAME}/${var_include})
	endforeach()
	set(__wish_external_include_directories ${temp_list} PARENT_SCOPE)

	# link
	if(arg_LINK)
		link_directories(${PATH_EXT}/${arg_NAME}/lib)
	endif()
	target_link_libraries(ext_${arg_NAME} INTERFACE ${arg_LINK})

	set(temp_list ${__wish_external_defines})
	foreach(var_define ${arg_DEFINE})
		target_compile_definitions(ext_${arg_NAME} INTERFACE -D${var_define})
		list(APPEND temp_list ${var_define})
	endforeach()
	set(__wish_external_defines ${temp_list} PARENT_SCOPE)

	# group
	foreach(group ${__wish_current_group_stack})
		set(group_members ${${group}})
		list(APPEND group_members get_${arg_NAME})
		set(${group} ${group_members} PARENT_SCOPE)
	endforeach()

	# debug
	if(arg_DEBUG OR __wish_global_debug)
		message("External target: ext_${arg_NAME}, get_${arg_NAME}")
		message("	Name      : ${arg_NAME}")
		message("	Define    : ${arg_DEFINE}")
		message("	ExtDir    : ${PATH_EXT}/${arg_NAME}")
		message("	ExtSource : ${PATH_EXT_SRC}/${arg_NAME}")
		message("	IncludeDir: ${arg_INCLUDE_DIR}")
		message("	Link      : ${arg_LINK}")
		message("	SkipCfg   : ${arg_SKIP_CONFIGURE}")
		message("	SkipBld   : ${arg_SKIP_BUILD}")
		message("	SkipCfgBld: ${arg_SKIP_CONFIGURE_AND_BUILD}")
		message("	Unparsed  : ${arg_UNPARSED_ARGUMENTS}")
		message("	Group     : ${__wish_current_group_stack}")
	endif()
endfunction()

# --- Executable -----------------------------------------------------------------------------------

function(wish_create_executable)
	cmake_parse_arguments(arg "DEBUG" "TARGET" "SOURCE;OBJECT;LINK" ${ARGN})

	# check
	if(NOT arg_SOURCE AND NOT arg_OBJECT)
		message(FATAL_ERROR "At least one SOURCE or OBJECT should be given.")
	endif()

	# glob
	file(GLOB_RECURSE matching_sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${arg_SOURCE})
	foreach(obj ${arg_OBJECT})
		list(APPEND matching_sources $<TARGET_OBJECTS:${obj}>)
	endforeach()

	# add
	add_executable(${arg_TARGET} ${matching_sources})
	target_link_libraries(${arg_TARGET} ${arg_LINK} ${__wish_static_link_std})
	target_link_libraries(${arg_TARGET} ${obj})

	# group
	foreach(group ${__wish_current_group_stack})
		set(group_members ${${group}})
		list(APPEND group_members ${arg_TARGET})
		set(${group} ${group_members} PARENT_SCOPE)
	endforeach()

	# debug
	if(arg_DEBUG OR __wish_global_debug)
		message("Executable target: ${arg_TARGET}")
		message("	Glob      : ${arg_SOURCE}")
		message("	Source    : ${matching_sources}")
		message("	Object    : ${arg_OBJECT}")
		message("	Link      : ${arg_LINK}")
		message("	Group     : ${__wish_current_group_stack}")
	endif()
endfunction()

# --- Library --------------------------------------------------------------------------------------

function(wish_create_library)
	cmake_parse_arguments(arg "DEBUG;STATIC;INTERFACE" "TARGET" "SOURCE;OBJECT;LINK" ${ARGN})

	# check
	list(GET arg_TARGET 0 arg_target_name)
#	if(NOT arg_SOURCE AND NOT arg_OBJECT)
#		message(FATAL_ERROR "At least one SOURCE or OBJECT should be given.")
#		# TODO P5: Target might be INTERFACE
#	endif()

	# glob
	if(arg_SOURCE)
		file(GLOB_RECURSE matching_sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${arg_SOURCE})
	endif()
	foreach(obj ${arg_OBJECT})
		list(APPEND matching_sources $<TARGET_OBJECTS:${obj}>)
	endforeach()

	# add
	if(arg_STATIC)
		add_library(${arg_TARGET} STATIC ${matching_sources} ${target_objects})
		target_link_libraries(${arg_TARGET} ${arg_LINK} ${__wish_static_link_std})
	elseif(arg_INTERFACE)
		add_library(${arg_TARGET} INTERFACE ${matching_sources} ${target_objects})
		target_link_libraries(${arg_TARGET} INTERFACE ${arg_LINK})
	else()
		message(FATAL_ERROR "Library has to be either STATIC or INTERFACE")
	endif()

#	add_library(${arg_TARGET} $<IF:$<BOOL:${arg_STATIC}>,"STATIC",""> $<IF:$<BOOL:${arg_INTERFACE}>,"INTERFACE",""> ${matching_sources} ${target_objects})
#	target_link_libraries(${arg_TARGET} $<IF:$<BOOL:${arg_INTERFACE}>,"INTERFACE",""> ${arg_LINK})

	# group
	foreach(group ${__wish_current_group_stack})
		set(group_members ${${group}})
		list(APPEND group_members ${arg_target_name})
		set(${group} ${group_members} PARENT_SCOPE)
	endforeach()

	# debug
	if(arg_DEBUG OR __wish_global_debug)
		message("Library target: ${arg_TARGET}")
		message("	Glob      : ${arg_SOURCE}")
		message("	Source    : ${matching_sources}")
		message("	Object    : ${arg_OBJECT}")
		message("	Link      : ${arg_LINK}")
		message("	Static    : ${arg_STATIC}")
		message("	Interface : ${arg_INTERFACE}")
		message("	Group     : ${__wish_current_group_stack}")
	endif()
endfunction()

# --- Object ---------------------------------------------------------------------------------------

function(wish_create_object)
	cmake_parse_arguments(arg "DEBUG" "TARGET" "SOURCE;OBJECT;LINK" ${ARGN})

	# check
	if(NOT arg_SOURCE AND NOT arg_OBJECT)
		message(FATAL_ERROR "At least one SOURCE or OBJECT should be given.")
	endif()

	# glob
	file(GLOB_RECURSE matching_sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${arg_SOURCE})
	foreach(obj ${arg_OBJECT})
		list(APPEND matching_sources $<TARGET_OBJECTS:${obj}>)
	endforeach()

	# add
	add_library(${arg_TARGET} OBJECT ${matching_sources})

	# group
	foreach(group ${__wish_current_group_stack})
		set(group_members ${${group}})
		list(APPEND group_members ${arg_TARGET})
		set(${group} ${group_members} PARENT_SCOPE)
	endforeach()

	# debug
	if(arg_DEBUG OR __wish_global_debug)
		message("Object target: ${arg_TARGET}")
		message("	Glob      : ${arg_SOURCE}")
		message("	Source    : ${matching_sources}")
		message("	Link      : ${arg_LINK}")
		message("	Object    : ${arg_OBJECT}")
		message("	Group     : ${__wish_current_group_stack}")
	endif()
endfunction()

# --------------------------------------------------------------------------------------------------
