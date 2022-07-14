# File: target.cmake, Created on 2017. 04. 14. 16:49, Author: Vader

include_guard(GLOBAL)

include(cmake/wish_version.cmake)

# --- Options --------------------------------------------------------------------------------------

set(__wish_configure_externals 1)
macro(wish_enable_configure_externals)
	set(__wish_configure_externals 1)
endmacro()
macro(wish_disable_configure_externals)
	set(__wish_configure_externals 0)
endmacro()

set(__wish_global_debug 0)
macro(wish_enable_debug)
	set(__wish_global_debug 1)
endmacro()
macro(wish_disable_debug)
	set(__wish_global_debug 0)
endmacro()

set(__wish_static_link_std)
macro(wish_static_link_std)
	set(__wish_static_link_std -static)
endmacro()

# --- Group ----------------------------------------------------------------------------------------

set(__wish_current_group)
macro(wish_group name)
	set(__wish_current_group ${name})

	if (DEFINED __wish_group_${name})
		return()
	endif()

	set(__wish_group_${name})

	add_custom_target(${name})
	foreach(alias ${ARGN})
		add_custom_target(${alias} DEPENDS ${name})
	endforeach()
endmacro()

macro(__wish_add_member_to_group target)
	add_dependencies(${__wish_current_group} ${target})
endmacro()

# --- IDE / Build info -----------------------------------------------------------------------------

set(__wish_external_include_directories)
set(__wish_external_defines)
set(__wish_external_raw_arguments "wish_version(${wish_version})")

## Creates wish_ide target that can be used to obtain various information for IDEs
function(wish_create_ide_target)
	# TODO P5: list targets / libraries / executables
	add_custom_target(wish_ide
		COMMAND ${CMAKE_COMMAND} -E echo "External include directories:"
		COMMAND ${CMAKE_COMMAND} -E echo "${__wish_external_include_directories}"
		COMMAND ${CMAKE_COMMAND} -E echo "External defines:"
		COMMAND ${CMAKE_COMMAND} -E echo "${__wish_external_defines}"
		VERBATIM
	)

	add_custom_target(wish
		COMMAND ${CMAKE_COMMAND} -E echo "Wish version: ${wish_version}"
	)

    file(WRITE "${CMAKE_BINARY_DIR}/__wish_external_raw_arguments.new.txt" "${__wish_external_raw_arguments}")
    add_custom_target(wish_ext_lazy
		# TODO P1: Different folder for different build types on ext is bypassed (ext currently built by build/release)
		# TODO P3: Its working, but would be nice, if not a re-entering call would execute it
		#			Creating a custom target for every external would improve this
		#			This would also mean that only the touched external are tried to be rebuilt
		#			Careful with multiple custom targets as USES_TERMINAL could force being serial
		USES_TERMINAL
        COMMAND ${CMAKE_COMMAND} -E compare_files "${CMAKE_BINARY_DIR}/__wish_external_raw_arguments.new.txt" "${CMAKE_BINARY_DIR}/__wish_external_raw_arguments.old.txt" || ${CMAKE_COMMAND} --build . --target ext -- -j 3
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/__wish_external_raw_arguments.new.txt" "${CMAKE_BINARY_DIR}/__wish_external_raw_arguments.old.txt"
    )
endfunction()

# --- External -------------------------------------------------------------------------------------

## Defines get_NAME for fetching the ExternalProject and ext_NAME as lightweight INTERFACE target
## Unrecognized parameters after INCLUDE_DIR, LINK or DEFINE are forbidden.
function(wish_create_external)
	cmake_parse_arguments(arg "DEBUG;NO_GROUP;SKIP_CONFIGURE_AND_BUILD;SKIP_CONFIGURE;SKIP_BUILD" "NAME" "INCLUDE_DIR;LINK;DEFINE" ${ARGN})

	set(temp_list ${__wish_external_raw_arguments})
	list(APPEND temp_list ${ARGN})
	set(__wish_external_raw_arguments ${temp_list} PARENT_SCOPE)

	# options
	set(command_str_configure)
	set(command_str_build)
	if(arg_SKIP_CONFIGURE_AND_BUILD OR arg_SKIP_CONFIGURE)
		set(command_str_configure "CONFIGURE_COMMAND;echo;\"Skipping configure...\"")
	endif()
	if(arg_SKIP_CONFIGURE_AND_BUILD OR arg_SKIP_BUILD)
		set(command_str_build "BUILD_COMMAND;echo;\"Skipping build...\"")
	endif()

	if(__wish_configure_externals)
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
	endif()
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
	if (NOT ${arg_NO_GROUP} AND ${__wish_configure_externals})
		__wish_add_member_to_group(get_${arg_NAME})
	endif()

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
		message("	NoGroup   : ${arg_NO_GROUP}")
		message("	Group     : ${__wish_current_group}")
	endif()
endfunction()

# --- Generate ------------------------------------------------------------------------------------

set(__wish_generators "")
# __wish_generator_command_${generator_name} stores the generator commands
# __wish_generator_output_rules_${generator_name} stores the generator input -> outputs rewrite rules

function(wish_generator)
	cmake_parse_arguments(arg "" "TARGET;COMMAND" "" ${ARGN})

	set(temp_list ${__wish_generators})
	list(APPEND temp_list ${arg_TARGET})
	set(__wish_generators ${temp_list} PARENT_SCOPE)

	set(__wish_generator_command_${arg_TARGET} ${arg_COMMAND} PARENT_SCOPE)

	# Have to hand parse outputs, as list of list as argument is not yet supported...
	set(output_rules ${arg_UNPARSED_ARGUMENTS})
	list(POP_FRONT output_rules)
	set(__wish_generator_output_rules_${arg_TARGET} ${output_rules} PARENT_SCOPE)
endfunction()

function(__wish_generate out_generated_outputs)
	cmake_parse_arguments(arg "" "" "${__wish_generators}" ${ARGN})

#	# debug
#	if(arg_DEBUG OR __wish_global_debug)
#		message("wish_generate")
#		message("	__wish_generators      : ${__wish_generators}")
#		message("arg_enum     : ${arg_enum}")
#		message("arg_codegen     : ${arg_codegen}")
#		message("	ARGN                   : ${ARGN}")
#	endif()

	set(generated_outputs "")

	foreach(generator ${__wish_generators})
#		message("foreach generator     : ${generator}")
#		message("foreach ${arg_${generator}}     : ${arg_${generator}}")
##		if (NOT ${arg_${generator}})
		if (NOT arg_${generator})
			continue()
		endif()

		file(GLOB_RECURSE matching_files LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS ${arg_${generator}})

		foreach(matching_file ${matching_files})
			set(output_files_rel "")
			set(output_files_abs "") # For CMake/Ninja to properly track dependencies output has to use abs path
			set(output_rules_left ${__wish_generator_output_rules_${generator}})
			while(output_rules_left)
				list(FIND output_rules_left "OUTPUT" end_index)

				list(SUBLIST output_rules_left 0 ${end_index} output_rule)
				# Use ${output_rule} list
				string(${output_rule} output_file ${matching_file})

				if (${output_file} STREQUAL ${matching_file})
					# if the output_file rule would match the matching_file the rule does not fit (and it would lead to circle dep anyways)
#					if(arg_DEBUG OR __wish_global_debug)
#						message("skipping ${matching_file} for generation, rule did not fit")
#					endif()
				else()
					list(APPEND output_files_rel ${output_file})
					list(APPEND output_files_abs ${CMAKE_CURRENT_SOURCE_DIR}/${output_file})
					if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${output_file}")
						file(TOUCH "${CMAKE_CURRENT_SOURCE_DIR}/${output_file}")
						file(TOUCH "${matching_file}") # Touch the input file to force generation of the output
					endif()
				endif()

				# Jump to next segment
				if (${end_index} EQUAL -1)
					set(output_rules_left "")
				else()
					math(EXPR end_index_p_1 "${end_index}+1")
					list(SUBLIST output_rules_left ${end_index_p_1} -1 output_rules_left)
				endif()
			endwhile()

			add_custom_command(
					OUTPUT  ${output_files_abs}
					COMMAND ${__wish_generator_command_${generator}} ${matching_file} ${output_files_rel}
					DEPENDS ${generator} ${matching_file}
					WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
			)

			list(APPEND generated_outputs ${output_files_abs})
		endforeach()
	endforeach()

	set(${out_generated_outputs} ${generated_outputs} PARENT_SCOPE)

#	if(arg_DEBUG OR __wish_global_debug)
#		message("out_generated_outputs : ${out_generated_outputs}")
#		message("generated_outputs     : ${generated_outputs}")
#	endif()
endfunction()

# --- Executable -----------------------------------------------------------------------------------

function(wish_create_executable)
	cmake_parse_arguments(arg "DEBUG;NO_GROUP" "TARGET" "SOURCE;OBJECT;GENERATE;LINK" ${ARGN})

	# check
	if(NOT arg_SOURCE AND NOT arg_OBJECT)
		message(FATAL_ERROR "At least one SOURCE or OBJECT should be given.")
	endif()

	# generated files
	if(arg_GENERATE)
		__wish_generate(generated_outputs ${arg_GENERATE})
	endif()

	# glob
	file(GLOB_RECURSE matching_sources LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS ${arg_SOURCE})
	foreach(obj ${arg_OBJECT})
		list(APPEND matching_sources $<TARGET_OBJECTS:${obj}>)
	endforeach()

	# add
	set(every_source ${matching_sources} ${generated_outputs})
	list(REMOVE_DUPLICATES every_source)

	add_executable(${arg_TARGET} ${every_source})
	target_link_libraries(${arg_TARGET} ${arg_LINK} ${__wish_static_link_std})
	target_link_libraries(${arg_TARGET} ${obj})

	# group
	if (NOT ${arg_NO_GROUP})
		__wish_add_member_to_group(${arg_TARGET})
	endif()

	# debug
	if(arg_DEBUG OR __wish_global_debug)
		message("Executable target: ${arg_TARGET}")
		message("	Glob      : ${arg_SOURCE}")
		message("	Source    : ${matching_sources}")
		message("	Object    : ${arg_OBJECT}")
		message("	Link      : ${arg_LINK}")
		message("	NoGroup   : ${arg_NO_GROUP}")
		message("	Group     : ${__wish_current_group}")
	endif()
endfunction()

# --- Library --------------------------------------------------------------------------------------

function(wish_create_library)
	cmake_parse_arguments(arg "DEBUG;NO_GROUP;STATIC;SHARED;INTERFACE" "TARGET" "SOURCE;OBJECT;GENERATE;LINK" ${ARGN})

	# check
#	if(NOT arg_SOURCE AND NOT arg_OBJECT)
#		message(FATAL_ERROR "At least one SOURCE or OBJECT should be given.")
#		# TODO P5: Target might be INTERFACE
#	endif()

	# generated files
	if(arg_GENERATE)
		__wish_generate(generated_outputs ${arg_GENERATE})
	endif()

	# glob
	if(arg_SOURCE)
		file(GLOB_RECURSE matching_sources LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS ${arg_SOURCE})
	endif()
	foreach(obj ${arg_OBJECT})
		list(APPEND matching_sources $<TARGET_OBJECTS:${obj}>)
	endforeach()

	# add_library
	set(every_source ${matching_sources} ${target_objects} ${generated_outputs})
	list(REMOVE_DUPLICATES every_source)

	if(arg_STATIC)
		add_library(${arg_TARGET} STATIC ${every_source})
		target_link_libraries(${arg_TARGET} ${arg_LINK} ${__wish_static_link_std})
	elseif(arg_SHARED)
		add_library(${arg_TARGET} SHARED ${every_source})
		target_link_libraries(${arg_TARGET} ${arg_LINK} ${__wish_static_link_std})
	elseif(arg_INTERFACE)
		add_library(${arg_TARGET} INTERFACE ${every_source})
		target_link_libraries(${arg_TARGET} INTERFACE ${arg_LINK})
	else()
		message(FATAL_ERROR "Library has to be either STATIC, SHARED or INTERFACE")
	endif()

#	add_library(${arg_TARGET} $<IF:$<BOOL:${arg_STATIC}>,"STATIC",""> $<IF:$<BOOL:${arg_INTERFACE}>,"INTERFACE",""> ${matching_sources} ${target_objects})
#	target_link_libraries(${arg_TARGET} $<IF:$<BOOL:${arg_INTERFACE}>,"INTERFACE",""> ${arg_LINK})

	# group
	if (NOT ${arg_NO_GROUP})
		__wish_add_member_to_group(${arg_TARGET})
	endif()

	# debug
	if(arg_DEBUG OR __wish_global_debug)
		message("Library target: ${arg_TARGET}")
		message("	Glob      : ${arg_SOURCE}")
		message("	Source    : ${matching_sources}")
		message("	Object    : ${arg_OBJECT}")
		message("	Generate  : ${arg_GENERATE}")
		message("	Generated : ${generated_outputs}")
		message("	Link      : ${arg_LINK}")
		message("	Static    : ${arg_STATIC}")
		message("	Shared    : ${arg_SHARED}")
		message("	Interface : ${arg_INTERFACE}")
		message("	NoGroup   : ${arg_NO_GROUP}")
		message("	Group     : ${__wish_current_group}")
	endif()
endfunction()

# --- Object ---------------------------------------------------------------------------------------

function(wish_create_object)
	cmake_parse_arguments(arg "DEBUG;NO_GROUP" "TARGET" "SOURCE;OBJECT;GENERATE;LINK" ${ARGN})

	# check
	if(NOT arg_SOURCE AND NOT arg_OBJECT)
		message(FATAL_ERROR "At least one SOURCE or OBJECT should be given.")
	endif()

	# generated files
	if(arg_GENERATE)
		__wish_generate(generated_outputs ${arg_GENERATE})
	endif()

	# glob
	file(GLOB_RECURSE matching_sources LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS ${arg_SOURCE})
	foreach(obj ${arg_OBJECT})
		list(APPEND matching_sources $<TARGET_OBJECTS:${obj}>)
	endforeach()

	# add
	set(every_source ${matching_sources} ${generated_outputs})
	list(REMOVE_DUPLICATES every_source)

	add_library(${arg_TARGET} OBJECT ${every_source})

	# group
	if (NOT ${arg_NO_GROUP})
		__wish_add_member_to_group(${arg_TARGET})
	endif()

	# debug
	if(arg_DEBUG OR __wish_global_debug)
		message("Object target: ${arg_TARGET}")
		message("	Glob      : ${arg_SOURCE}")
		message("	Source    : ${matching_sources}")
		message("	Generate  : ${arg_GENERATE}")
		message("	Generated : ${generated_outputs}")
		message("	Link      : ${arg_LINK}")
		message("	Object    : ${arg_OBJECT}")
		message("	NoGroup   : ${arg_NO_GROUP}")
		message("	Group     : ${__wish_current_group}")
	endif()
endfunction()

# --------------------------------------------------------------------------------------------------
