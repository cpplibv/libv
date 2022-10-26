// Project: libv.sys, File: src/libv/sys/os_version.cpp

// hpp
#include <libv/sys/os_version.hpp>
// ext
#include <fmt/format.h>
// os
#ifdef _WIN32
#	include <sysinfoapi.h>
#else
#	include <sys/utsname.h>
#endif


namespace libv {
namespace sys {

// -------------------------------------------------------------------------------------------------

std::string os_version() noexcept {

#ifdef _WIN32

	OSVERSIONINFO info;
	ZeroMemory(&info, sizeof(OSVERSIONINFO));
	info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	if (GetVersionEx(&info) == 0)
		return "Unknown (Failed to determine Windows version)";

	return fmt::format("{} {}.{} {}",
			[&] {
				switch (info.dwPlatformId) {
				case VER_PLATFORM_WIN32s:
					return "Windows 3.x";
				case VER_PLATFORM_WIN32_WINDOWS:
					return info.dwMinorVersion == 0 ? "Windows 95" : "Windows 98";
				case VER_PLATFORM_WIN32_NT:
					return "Windows NT";
				default:
					return "Unknown";
				}
			}(),
			info.dwMajorVersion,
			info.dwMinorVersion,
			info.dwBuildNumber);

#else

	struct utsname uts;
	uname(&uts);
	return fmt::format("{} {} {}", uts.sysname, uts.release, uts.version);

#endif

}

// -------------------------------------------------------------------------------------------------

} // namespace sys
} // namespace libv
