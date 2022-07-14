#

include_guard(GLOBAL)

# --- System ---------------------------------------------------------------------------------------

if(NOT DEFINED WISH_PROCESSOR_COUNT)
	set(WISH_PROCESSOR_COUNT 1) # Unknown

	include(ProcessorCount)
	ProcessorCount(WISH_PROCESSOR_COUNT)
endif()

# --- Date Time ------------------------------------------------------------------------------------

#string(TIMESTAMP WISH_DATE_LONG "%Y.%m.%d.")
#string(TIMESTAMP WISH_DATE_SHORT "%Y%m%d")
#string(TIMESTAMP WISH_TIME_LONG "%H:%M")
#string(TIMESTAMP WISH_TIME_SHORT "%H%M")

# --- Git ------------------------------------------------------------------------------------------

# Get the current working branch
execute_process(
	COMMAND git rev-parse --abbrev-ref HEAD
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	OUTPUT_VARIABLE WISH_GIT_BRANCH
	OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Get the latest abbreviated commit hash of the working branch
execute_process(
	COMMAND git rev-parse --short=8 HEAD
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	OUTPUT_VARIABLE WISH_GIT_COMMIT_HASH
	OUTPUT_STRIP_TRAILING_WHITESPACE
)

# --------------------------------------------------------------------------------------------------
