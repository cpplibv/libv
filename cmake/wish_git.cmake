#

# --------------------------------------------------------------------------------------------------

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
