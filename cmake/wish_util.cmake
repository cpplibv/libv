#

# --------------------------------------------------------------------------------------------------

if(NOT DEFINED WISH_PROCESSOR_COUNT)
	set(WISH_PROCESSOR_COUNT 1) # Unknown

	include(ProcessorCount)
	ProcessorCount(WISH_PROCESSOR_COUNT)
endif()

# --------------------------------------------------------------------------------------------------
