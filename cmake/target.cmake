# File: target.cmake, Created on 2017. 04. 14. 16:49, Author: Vader


# --------------------------------------------------------------------------------------------------

function(create_executable)
    set(arguments ${ARGN})

	set(arg_sources)
	set(arg_targets)
	set(arg_links)
	set(arg_groups)

	set(selected_arg_list)

	foreach(argument ${arguments})
		if(argument STREQUAL "SOURCE")
			set(selected_arg_list arg_sources)
		elseif(argument STREQUAL "TARGET")
			set(selected_arg_list arg_targets)
		elseif(argument STREQUAL "LINK")
			set(selected_arg_list arg_links)
		elseif(argument STREQUAL "GROUP")
			set(selected_arg_list arg_groups)
		else()
			if("${selected_arg_list}" STREQUAL "")
				message(FATAL_ERROR "Uncategorized argument: ${arguments}")
			else()
				list(APPEND ${selected_arg_list} ${argument})
			endif()
		endif()
	endforeach()


    list(LENGTH arg_sources number_found)
	if(number_found EQUAL 0)
		message(FATAL_ERROR "At least one SOURCE should be given.")
    endif()


    list(LENGTH arg_targets number_found)
	if(number_found EQUAL 1)
		add_executable(${arg_targets} ${arg_sources})
	else()
		message(FATAL_ERROR "Exactly one TARGET should be given. Passed: ${arg_targets}")
    endif()


	target_link_libraries(${arg_targets} ${arg_links})


	foreach(group ${arg_groups})
		set(group_members ${${group}})
		list(APPEND group_members ${arg_targets})
		set(${group} ${group_members} PARENT_SCOPE)
	endforeach()

	message("Target: ${arg_targets}")
	message("	Source: ${arg_sources}")
	message("	Link:   ${arg_links}")
	message("	Group:  ${arg_groups}")
endfunction()

# --------------------------------------------------------------------------------------------------

function(create_library)
    set(arguments ${ARGN})

	set(arg_sources)
	set(arg_targets)
	set(arg_links)
	set(arg_groups)

	set(selected_arg_list)

	foreach(argument ${arguments})
		if(argument STREQUAL "SOURCE")
			set(selected_arg_list arg_sources)
		elseif(argument STREQUAL "TARGET")
			set(selected_arg_list arg_targets)
		elseif(argument STREQUAL "LINK")
			set(selected_arg_list arg_links)
		elseif(argument STREQUAL "GROUP")
			set(selected_arg_list arg_groups)
		else()
			if("${selected_arg_list}" STREQUAL "")
				message(FATAL_ERROR "Uncategorized argument: ${arguments}")
			else()
				list(APPEND ${selected_arg_list} ${argument})
			endif()
		endif()
	endforeach()


    list(LENGTH arg_sources number_found)
	if(number_found EQUAL 0)
		message(FATAL_ERROR "At least one SOURCE should be given.")
    endif()


    list(LENGTH arg_targets number_found)
	if(number_found EQUAL 1)
		# TODO P5: Add option to STATIC and other flags to be passed into add_library
		add_library(${arg_targets} STATIC ${arg_sources})
	else()
		message(FATAL_ERROR "Exactly one TARGET should be given. Passed: ${arg_targets}")
    endif()


	target_link_libraries(${arg_targets} ${arg_links})


	foreach(group ${arg_groups})
		set(group_members ${${group}})
		list(APPEND group_members ${arg_targets})
		set(${group} ${group_members} PARENT_SCOPE)
	endforeach()

	message("Target: ${arg_targets}")
	message("	Source: ${arg_sources}")
	message("	Link:   ${arg_links}")
	message("	Group:  ${arg_groups}")
endfunction()

# --------------------------------------------------------------------------------------------------
