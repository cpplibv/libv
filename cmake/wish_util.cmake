#

# --------------------------------------------------------------------------------------------------

if(NOT DEFINED WISH_PROCESSOR_COUNT)
	set(WISH_PROCESSOR_COUNT 1) # Unknown

	if(NOT WIN32)
		execute_process(COMMAND "nproc" OUTPUT_VARIABLE WISH_PROCESSOR_COUNT)
	else()
		set(WISH_PROCESSOR_COUNT "$ENV{NUMBER_OF_PROCESSORS}")
	endif()
endif()

# --------------------------------------------------------------------------------------------------
