# File: target.cmake, Created on 2017. 04. 14. 16:49, Author: Vader


# --------------------------------------------------------------------------------------------------

function(create_executable)
    cmake_parse_arguments(arg "" "TARGET" "SOURCE;OBJECT;LINK;GROUP" ${ARGN})


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


	foreach(group ${arg_GROUP})
		set(group_members ${${group}})
		list(APPEND group_members ${arg_TARGET})
		set(${group} ${group_members} PARENT_SCOPE)
	endforeach()

#	message("Target: ${arg_TARGET}")
#	message("	Glob:   ${arg_SOURCE}")
#	message("	Source: ${matching_sources}")
#	message("	Object: ${arg_OBJECT}")
#	message("	Link:   ${arg_LINK}")
#	message("	Group:  ${arg_GROUP}")
endfunction()

# --------------------------------------------------------------------------------------------------

function(create_library)
    cmake_parse_arguments(arg "" "TARGET" "SOURCE;OBJECT;LINK;GROUP" ${ARGN})


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


	add_library(${arg_TARGET} STATIC ${matching_sources} ${target_objects})
	target_link_libraries(${arg_TARGET} ${arg_LINK})


	foreach(group ${arg_GROUP})
		set(group_members ${${group}})
		list(APPEND group_members ${arg_TARGET})
		set(${group} ${group_members} PARENT_SCOPE)
	endforeach()

#	message("Target: ${arg_TARGET}")
#	message("	Glob:   ${arg_SOURCE}")
#	message("	Source: ${matching_sources}")
#	message("	Object: ${arg_OBJECT}")
#	message("	Link:   ${arg_LINK}")
#	message("	Group:  ${arg_GROUP}")
endfunction()

# --------------------------------------------------------------------------------------------------

function(create_object)
    cmake_parse_arguments(arg "" "TARGET" "SOURCE;OBJECT;GROUP" ${ARGN})


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


	foreach(group ${arg_GROUP})
		set(group_members ${${group}})
		list(APPEND group_members ${arg_TARGET})
		set(${group} ${group_members} PARENT_SCOPE)
	endforeach()

#	message("Target: ${arg_TARGET}")
#	message("	Glob:   ${arg_SOURCE}")
#	message("	Source: ${matching_sources}")
#	message("	Object: ${arg_OBJECT}")
#	message("	Group:  ${arg_GROUP}")
endfunction()

# --------------------------------------------------------------------------------------------------
