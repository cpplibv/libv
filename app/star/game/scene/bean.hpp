// Project: libv, File: app/star/game/scene/utility.hpp

#pragma once

// libv
#include <libv/utility/concat.hpp>
#include <libv/utility/nexus.hpp>
// std
#include <exception>
#include <string_view>


namespace star {

// -------------------------------------------------------------------------------------------------

template <typename T>
inline T& requireBean(libv::Nexus& nexus, std::string_view sceneName, std::string_view beanName) {
	const auto bean = nexus.object_view_get<T>();
	if (!bean)
		throw std::runtime_error(libv::concat(sceneName, " scene creation failed: Missing bean: ", beanName));

	return *bean;
}

// -------------------------------------------------------------------------------------------------

} // namespace star
