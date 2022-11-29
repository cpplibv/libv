
if (NOT DEFINED WISH_REQUEST_VERSION)
	message(FATAL_ERROR "Wish: WISH_REQUEST_VERSION is not defined (Example: v5.0.5)\n"
			"  Usage:\n"
			"    set(WISH_REQUEST_VERSION v5.0.5)\n"
			"    include(cmake/wish.cmake)\n")
endif ()


set(wish_path_install ${CMAKE_SOURCE_DIR}/cmake)


#message("WISH_REQUEST_VERSION ${WISH_REQUEST_VERSION}")
#message("WISH_CURRENT_VERSION ${WISH_CURRENT_VERSION}")

if (NOT WISH_REQUEST_VERSION STREQUAL WISH_CURRENT_VERSION)
	if (NOT DEFINED WISH_CURRENT_VERSION)
		message(STATUS "Installing Wish ${WISH_REQUEST_VERSION}...")
	else ()
		message(STATUS "Updating Wish from ${WISH_CURRENT_VERSION} to ${WISH_REQUEST_VERSION}...")
	endif ()

	file(LOCK ${wish_path_install}/wish/wish.lock GUARD FILE)

	set(wish_path_lite ${CMAKE_CURRENT_BINARY_DIR}/wish_lite.zip)

	file(GLOB_RECURSE wish_installed_files ${wish_path_install}/wish/**.cmake)
	file(REMOVE ${wish_installed_files})
	file(DOWNLOAD https://github.com/VaderY/wish/releases/download/${WISH_REQUEST_VERSION}/wish_lite.zip ${wish_path_lite})
	file(ARCHIVE_EXTRACT INPUT ${wish_path_lite} DESTINATION ${wish_path_install})
	file(WRITE ${wish_path_install}/wish/.gitignore "*")
	file(REMOVE ${wish_path_lite})

	set(WISH_CURRENT_VERSION ${WISH_REQUEST_VERSION} CACHE STRING "Currently installed Wish version identifier. Used to detect and requests updates" FORCE)
endif ()

include(${wish_path_install}/wish/wish_core.cmake)
