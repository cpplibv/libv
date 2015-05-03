if(NOT DEFINED PROCESSOR_COUNT)
	set(PROCESSOR_COUNT 1) # Unknown

	if(NOT WIN32)
		execute_process(COMMAND "nproc" OUTPUT_VARIABLE info)
	else()
		set(PROCESSOR_COUNT "$ENV{NUMBER_OF_PROCESSORS}")
	endif()
endif()