// Project: libv.sys, File: src/libv/sys/start.cpp

// hpp
#include <libv/sys/start.hpp>
// os
#ifdef _WIN32
#	include <windows.h>
#	include <shellapi.h>
#else
#	include <cstdlib>
#endif
// pro
#include <libv/sys/log.hpp>


namespace libv {
namespace sys {

// -------------------------------------------------------------------------------------------------

void start_unsafe(std::string link) {
	log_sys.trace("Starting [{}]", link);

#ifdef _WIN32
	// ShellExecuteA is still unsafe,
	// but it's safer than: std::system(("start " + std::move(link)).c_str());

	auto result = ShellExecuteA(nullptr, "open", link.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
	auto r = reinterpret_cast<INT_PTR>(result);

	// Weird corner cases found (so far):
	// https://echo||echo - opens an empty browser
	// echo               - runs the echo command in a flickered cmd (I guess, its just command run, maybe not even weird)

	if (r > 32) {
		log_sys.trace("Starting for [{}] returned: Success", link);
	} else {
		if      (r == 0                     ) log_sys.error("Starting for [{}] returned: 0 - {}"                     , link, "The operating system is out of memory or resources.");
		else if (r == ERROR_FILE_NOT_FOUND  ) log_sys.error("Starting for [{}] returned: ERROR_FILE_NOT_FOUND - {}"  , link, "The specified file was not found.");
		else if (r == ERROR_PATH_NOT_FOUND  ) log_sys.error("Starting for [{}] returned: ERROR_PATH_NOT_FOUND - {}"  , link, "The specified path was not found.");
		else if (r == ERROR_BAD_FORMAT      ) log_sys.error("Starting for [{}] returned: ERROR_BAD_FORMAT - {}"      , link, "The .exe file is invalid (non-Win32 .exe or error in .exe image).");
		else if (r == SE_ERR_ACCESSDENIED   ) log_sys.error("Starting for [{}] returned: SE_ERR_ACCESSDENIED - {}"   , link, "The operating system denied access to the specified file.");
		else if (r == SE_ERR_ASSOCINCOMPLETE) log_sys.error("Starting for [{}] returned: SE_ERR_ASSOCINCOMPLETE - {}", link, "The file name association is incomplete or invalid.");
		else if (r == SE_ERR_DDEBUSY        ) log_sys.error("Starting for [{}] returned: SE_ERR_DDEBUSY - {}"        , link, "The DDE transaction could not be completed because other DDE transactions were being processed.");
		else if (r == SE_ERR_DDEFAIL        ) log_sys.error("Starting for [{}] returned: SE_ERR_DDEFAIL - {}"        , link, "The DDE transaction failed.");
		else if (r == SE_ERR_DDETIMEOUT     ) log_sys.error("Starting for [{}] returned: SE_ERR_DDETIMEOUT - {}"     , link, "The DDE transaction could not be completed because the request timed out.");
		else if (r == SE_ERR_DLLNOTFOUND    ) log_sys.error("Starting for [{}] returned: SE_ERR_DLLNOTFOUND - {}"    , link, "The specified DLL was not found.");
		// Duplicate branch warning on:
//		else if (r == SE_ERR_FNF            ) log_sys.error("Starting for [{}] returned: SE_ERR_FNF - {}"            , link, "The specified file was not found.");
		else if (r == SE_ERR_NOASSOC        ) log_sys.error("Starting for [{}] returned: SE_ERR_NOASSOC - {}"        , link, "There is no application associated with the given file name extension. This error will also be returned if you attempt to print a file that is not printable.");
		else if (r == SE_ERR_OOM            ) log_sys.error("Starting for [{}] returned: SE_ERR_OOM - {}"            , link, "There was not enough memory to complete the operation.");
		// Duplicate branch warning on:
//		else if (r == SE_ERR_PNF            ) log_sys.error("Starting for [{}] returned: SE_ERR_PNF - {}"            , link, "The specified path was not found.");
		else if (r == SE_ERR_SHARE          ) log_sys.error("Starting for [{}] returned: SE_ERR_SHARE - {}"          , link, "A sharing violation occurred.");
		else                                  log_sys.error("Starting for [{}] returned: {} - UNKNOWN ERROR"         , link, r);
	}

#else
	std::system(("xdg-open " + std::move(link)).c_str());
#endif
}

// -------------------------------------------------------------------------------------------------

} // namespace sys
} // namespace libv
