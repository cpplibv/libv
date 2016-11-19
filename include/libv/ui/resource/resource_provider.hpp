// File: resource_provider.hpp Author: Vader Created on 2016. augusztus 25., 5:22

#pragma once

// std
#include <string>
// pro
#include <libv/ui/resource/resource_fwd.hpp>

// -------------------------------------------------------------------------------------------------

namespace boost {
namespace filesystem {
class path;
} //namespace filesystem
} //namespace boost

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

//template <typename T, typename... Args>
//struct ResourceResolver {
//	virtual bool operator()(T::Arguments& arg, const Args&... args) = 0;
//};
//
//template <typename T>
//struct ResourceCache {
//	virtual bool operator()(ResourceView<T>& res, const T::Arguments& arg) = 0;
//};
//
//template <typename T>
//struct ResourceProvider {
//	virtual bool operator()(ResourceView<T>& res, const T::Arguments& arg) = 0;
//};
//
//// -------------------------------------------------------------------------------------------------
//
//template <typename T>
//struct ResourceHandler {
//	std::vector<std::shared_ptr<ResourceResolver<T>>> resolvers;
//	std::vector<std::shared_ptr<ResourceCache<T>>> caches;
//	std::vector<std::shared_ptr<ResourceProvider<T>>> providers;
//};

// -------------------------------------------------------------------------------------------------

struct ResourceProvider {
	virtual File_view file(const boost::filesystem::path& filePath) = 0;
	virtual UIFont2D_view font(const boost::filesystem::path& filePath) = 0;
	virtual UIProgram_view program(const std::string& name) = 0;
	virtual UIShader_view shader(const boost::filesystem::path& filePath) = 0;
	virtual UITexture2D_view texture(const boost::filesystem::path& filePath) = 0;
	virtual ~ResourceProvider() = default;
};

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv