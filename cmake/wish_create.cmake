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

# --- External -------------------------------------------------------------------------------------

# Defines get_NAME for fetching the ExternalProject and ext_NAME as INTERFACE target
function(wish_create_external)
	cmake_parse_arguments(arg "DEBUG" "NAME" "INCLUDEDIR;LINK;DEFINE" ${ARGN})

	ExternalProject_Add(
		get_${arg_NAME}
		PREFIX ${PATH_EXT_SRC}/${arg_NAME}
		GIT_SHALLOW 1
		EXCLUDE_FROM_ALL 1
		${arg_UNPARSED_ARGUMENTS}
	)

	# add
	add_library(ext_${arg_NAME} INTERFACE)

	# include
	if(NOT arg_INCLUDEDIR)
		list(APPEND arg_INCLUDEDIR include)
	endif()
	foreach(var_include ${arg_INCLUDEDIR})
		target_include_directories(ext_${arg_NAME} SYSTEM INTERFACE ${PATH_EXT}/${arg_NAME}/${var_include})
	endforeach()

	# link
	link_directories(${PATH_EXT}/${arg_NAME}/lib)
	target_link_libraries(ext_${arg_NAME} INTERFACE ${arg_LINK})

	foreach(var_define ${arg_DEFINE})
		target_compile_definitions(ext_${arg_NAME} INTERFACE -D${var_define})
	endforeach()

	# group
	foreach(group ${__wish_current_group_stack})
		set(group_members ${${group}})
		list(APPEND group_members get_${arg_NAME})
		set(${group} ${group_members} PARENT_SCOPE)
	endforeach()

	if(arg_DEBUG OR __wish_global_debug)
		message("External target: ext_${arg_NAME}, get_${arg_NAME}")
		message("	Name      : ${arg_NAME}")
		message("	IncludeDir: ${arg_INCLUDEDIR}")
		message("	Link      : ${arg_LINK}")
		message("	Define    : ${arg_DEFINE}")
		message("	Unparsed  : ${arg_UNPARSED_ARGUMENTS}")
		message("	Group     : ${__wish_current_group_stack}")
	endif()
endfunction()

# --- Executable -----------------------------------------------------------------------------------

function(wish_create_executable)
	cmake_parse_arguments(arg "DEBUG" "TARGET" "SOURCE;OBJECT;LINK" ${ARGN})


	if(NOT arg_SOURCE AND NOT arg_OBJECT)
		message(FATAL_ERROR "At least one SOURCE or OBJECT should be given.")
	endif()


	file(GLOB_RECURSE matching_sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${arg_SOURCE})
	foreach(obj ${arg_OBJECT})
		list(APPEND matching_sources $<TARGET_OBJECTS:${obj}>)
	endforeach()


	add_executable(${arg_TARGET} ${matching_sources})
	target_link_libraries(${arg_TARGET} ${arg_LINK})


	foreach(group ${__wish_current_group_stack})
		set(group_members ${${group}})
		list(APPEND group_members ${arg_TARGET})
		set(${group} ${group_members} PARENT_SCOPE)
	endforeach()

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


	list(GET arg_TARGET 0 arg_target_name)
#	if(NOT arg_SOURCE AND NOT arg_OBJECT)
#		message(FATAL_ERROR "At least one SOURCE or OBJECT should be given.")
#		# TODO P5: Target might be INTERFACE
#	endif()

	if(arg_SOURCE)
		file(GLOB_RECURSE matching_sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${arg_SOURCE})
	endif()
	foreach(obj ${arg_OBJECT})
		list(APPEND matching_sources $<TARGET_OBJECTS:${obj}>)
	endforeach()


	if(arg_STATIC)
		add_library(${arg_TARGET} STATIC ${matching_sources} ${target_objects})
		target_link_libraries(${arg_TARGET} ${arg_LINK})
	elseif(arg_INTERFACE)
		add_library(${arg_TARGET} INTERFACE ${matching_sources} ${target_objects})
		target_link_libraries(${arg_TARGET} INTERFACE ${arg_LINK})
	endif()

#	add_library(${arg_TARGET} $<IF:$<BOOL:${arg_STATIC}>,"STATIC",""> $<IF:$<BOOL:${arg_INTERFACE}>,"INTERFACE",""> ${matching_sources} ${target_objects})
#	target_link_libraries(${arg_TARGET} $<IF:$<BOOL:${arg_INTERFACE}>,"INTERFACE",""> ${arg_LINK})

	foreach(group ${__wish_current_group_stack})
		set(group_members ${${group}})
		list(APPEND group_members ${arg_target_name})
		set(${group} ${group_members} PARENT_SCOPE)
	endforeach()

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
	cmake_parse_arguments(arg "DEBUG" "TARGET" "SOURCE;OBJECT" ${ARGN})


	if(NOT arg_SOURCE AND NOT arg_OBJECT)
		message(FATAL_ERROR "At least one SOURCE or OBJECT should be given.")
	endif()


	file(GLOB_RECURSE matching_sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${arg_SOURCE})
	foreach(obj ${arg_OBJECT})
		list(APPEND matching_sources $<TARGET_OBJECTS:${obj}>)
	endforeach()


	add_library(${arg_TARGET} OBJECT ${matching_sources})


	foreach(group ${__wish_current_group_stack})
		set(group_members ${${group}})
		list(APPEND group_members ${arg_TARGET})
		set(${group} ${group_members} PARENT_SCOPE)
	endforeach()

	if(arg_DEBUG OR __wish_global_debug)
		message("Object target: ${arg_TARGET}")
		message("	Glob      :   ${arg_SOURCE}")
		message("	Source    : ${matching_sources}")
		message("	Object    : ${arg_OBJECT}")
		message("	Group     : ${__wish_current_group_stack}")
	endif()
endfunction()

# --------------------------------------------------------------------------------------------------
