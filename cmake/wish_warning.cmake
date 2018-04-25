#

# --- Warning -----------------------------------------------------------------------------------

# @Usage
#	wish_warning(
#		MSVC /Wall
#
#		Clang -Weverything
#		Clang -Wmissing-override
#
#		GNU -Wall
#		GNU -Wextra
#		GNU -Wpedantic
#		GNU VERSION_GREATER 7.0 -Wduplicated-branches
#	)
function(wish_warning)

	set(compilers MSVC Clang GNU)
	set(conditions VERSION_LESS VERSION_GREATER VERSION_EQUAL VERSION_LESS_EQUAL VERSION_GREATER_EQUAL)

	set(cursor_compiler_id)
	set(cursor_condition)
	set(cursor_condition_version)
	set(i 0)

	set(warnings)

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
					set(warnings "${warnings}${arg} ")
				endif()
			endif()
		endif()
	endwhile()

	message(STATUS "Warnings: ${warnings}")
endfunction()
