#

include_guard(GLOBAL)

# --- Warning -----------------------------------------------------------------------------------

function(_aux_wish_collect_compiler_flags output)
	set(result)

	set(compilers MSVC Clang GNU)
	set(conditions VERSION_LESS VERSION_GREATER VERSION_EQUAL VERSION_LESS_EQUAL VERSION_GREATER_EQUAL)

	set(cursor_compiler_id)
	set(cursor_condition)
	set(cursor_condition_version)
	set(i 0)

	list(LENGTH ARGN argc)
	while(i LESS argc)
		list(GET ARGN ${i} arg)
		math(EXPR i ${i}+1)

		if(arg IN_LIST compilers)
			set(cursor_compiler_id ${arg})
			set(cursor_condition)
			set(cursor_condition_version)

		elseif(arg IN_LIST conditions)
			set(cursor_condition ${arg})
			set(cursor_condition_version)

			if(i LESS argc) # Just ignore the last unfinished condition
				list(GET ARGN ${i} cursor_condition_version)
			endif()

			math(EXPR i ${i}+1) # extra token consumed for version

		else()
			if(CMAKE_CXX_COMPILER_ID MATCHES ${cursor_compiler_id})
				if(CMAKE_CXX_COMPILER_VERSION ${cursor_condition} ${cursor_condition_version})
					add_compile_options(${arg})
					set(result "${result}${arg} ")
				endif()
			endif()
		endif()
	endwhile()

	set(${output} ${result} PARENT_SCOPE)
endfunction()

# -------------------------------------------------------------------------------------------------

# @Usage
#	wish_warning(
#		MSVC  /Wall
#		Clang -Weverything
#		GNU   -Wall
#		GNU   -Wextra
#		GNU VERSION_GREATER 7.0 -Wduplicated-branches
#	)
function(wish_warning)
	_aux_wish_collect_compiler_flags(flags ${ARGV})
	message(STATUS "Warning flags: ${flags}")
endfunction()

# -------------------------------------------------------------------------------------------------

# @Usage
#	wish_compiler_flags(
#		GNU VERSION_LESS 13.0 -fcoroutines
#	)
function(wish_compiler_flags versioned_flag_list)
	_aux_wish_collect_compiler_flags(flags ${ARGV})
	message(STATUS "Compiler flags: ${flags}")
endfunction()

# -------------------------------------------------------------------------------------------------
