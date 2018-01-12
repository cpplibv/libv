# File: target.cmake, Created on 2017. 04. 14. 16:49, Author: Vader


# --------------------------------------------------------------------------------------------------

set(__wish_current_groups)

macro(create_group arg_GROUP)
	list(APPEND __wish_current_groups ${arg_GROUP})
endmacro()

macro(end_group)
	list(REMOVE_AT __wish_current_groups -1)
endmacro()

# --------------------------------------------------------------------------------------------------

function(create_executable)
    cmake_parse_arguments(arg "DEBUG" "TARGET" "SOURCE;OBJECT;LINK" ${ARGN})


    list(LENGTH arg_SOURCE number_found)
	if(number_found EQUAL 0)
		list(LENGTH arg_OBJECT number_found)
		if(number_found EQUAL 0)
			message(FATAL_ERROR "At least one SOURCE or OBJECT should be given.")
		endif()
    endif()


	file(GLOB_RECURSE matching_sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${arg_SOURCE})
	foreach(obj ${arg_OBJECT})
		list(APPEND matching_sources $<TARGET_OBJECTS:${obj}>)
	endforeach()


	add_executable(${arg_TARGET} ${matching_sources})
	target_link_libraries(${arg_TARGET} ${arg_LINK})


	foreach(group ${__wish_current_groups})
		set(group_members ${${group}})
		list(APPEND group_members ${arg_TARGET})
		set(${group} ${group_members} PARENT_SCOPE)
	endforeach()

	if (arg_DEBUG)
		message("Target: ${arg_TARGET}")
		message("	Glob:   ${arg_SOURCE}")
		message("	Source: ${matching_sources}")
		message("	Object: ${arg_OBJECT}")
		message("	Link:   ${arg_LINK}")
		message("	Group:  ${__wish_current_groups}")
	endif()
endfunction()

# --------------------------------------------------------------------------------------------------

function(create_library)
    cmake_parse_arguments(arg "DEBUG;STATIC;INTERFACE" "TARGET" "SOURCE;OBJECT;LINK" ${ARGN})


    list(GET arg_TARGET 0 arg_target_name)
#    list(LENGTH arg_SOURCE number_found)
#	if(number_found EQUAL 0)
#		list(LENGTH arg_OBJECT number_found)
#		if(number_found EQUAL 0)
#			message(FATAL_ERROR "At least one SOURCE or OBJECT should be given.")
#			# TODO P5: Target might be INTERFACE
#		endif()
#    endif()

    list(LENGTH arg_SOURCE number_found)
	if (NOT number_found EQUAL 0)
		file(GLOB_RECURSE matching_sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${arg_SOURCE})
    endif()
	foreach(obj ${arg_OBJECT})
		list(APPEND matching_sources $<TARGET_OBJECTS:${obj}>)
	endforeach()


	if (arg_STATIC)
		add_library(${arg_TARGET} STATIC ${matching_sources} ${target_objects})
		target_link_libraries(${arg_TARGET} ${arg_LINK})
    elseif(arg_INTERFACE)
		add_library(${arg_TARGET} INTERFACE ${matching_sources} ${target_objects})
		target_link_libraries(${arg_TARGET} INTERFACE ${arg_LINK})
    endif()

#	add_library(${arg_TARGET} $<IF:$<BOOL:${arg_STATIC}>,"STATIC",""> $<IF:$<BOOL:${arg_INTERFACE}>,"INTERFACE",""> ${matching_sources} ${target_objects})
#	target_link_libraries(${arg_TARGET} $<IF:$<BOOL:${arg_INTERFACE}>,"INTERFACE",""> ${arg_LINK})

	foreach(group ${__wish_current_groups})
		set(group_members ${${group}})
		list(APPEND group_members ${arg_target_name})
		set(${group} ${group_members} PARENT_SCOPE)
	endforeach()

	if (arg_DEBUG)
		message("Target: ${arg_TARGET}")
		message("	Glob:   ${arg_SOURCE}")
		message("	Source: ${matching_sources}")
		message("	Object: ${arg_OBJECT}")
		message("	Link:   ${arg_LINK}")
		message("	Group:  ${__wish_current_groups}")
		message("	Static: ${arg_STATIC}")
		message("	Interf: ${arg_INTERFACE}")
	endif()
endfunction()

# --------------------------------------------------------------------------------------------------

function(create_object)
    cmake_parse_arguments(arg "DEBUG" "TARGET" "SOURCE;OBJECT" ${ARGN})


    list(LENGTH arg_SOURCE number_found)
	if(number_found EQUAL 0)
		list(LENGTH arg_OBJECT number_found)
		if(number_found EQUAL 0)
			message(FATAL_ERROR "At least one SOURCE or OBJECT should be given.")
		endif()
    endif()


	file(GLOB_RECURSE matching_sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${arg_SOURCE})
	foreach(obj ${arg_OBJECT})
		list(APPEND matching_sources $<TARGET_OBJECTS:${obj}>)
	endforeach()


	add_library(${arg_TARGET} OBJECT ${matching_sources})


	foreach(group ${__wish_current_groups})
		set(group_members ${${group}})
		list(APPEND group_members ${arg_TARGET})
		set(${group} ${group_members} PARENT_SCOPE)
	endforeach()

	if (arg_DEBUG)
		message("Target: ${arg_TARGET}")
		message("	Glob:   ${arg_SOURCE}")
		message("	Source: ${matching_sources}")
		message("	Object: ${arg_OBJECT}")
		message("	Group:  ${__wish_current_groups}")
	endif()
endfunction()

# --------------------------------------------------------------------------------------------------
