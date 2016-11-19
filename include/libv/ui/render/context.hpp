// File: UIRenderVBO.hpp, Created on 2015. április 30. 0:01, Author: Vader

#pragma once

// cfg
#include <libv/ui/config.hpp>
// libv
#include <libv/gl/gl.hpp>
#include <libv/memory.hpp>
//#include <libv/scoped_cache.hpp>
#include <libv/vec.hpp>
// std
//#include <memory>
#include <vector>
// pro
#include <libv/ui/resource/resource_fwd.hpp>

namespace boost {
namespace filesystem {
class path;
} //namespace filesystem
} //namespace boost

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ResourceProvider;
class Context;

//template <typename T> class ResourceHandler;

//enum class LifeTime : uint8_t {
//	Frame = 0,
//	//	Frame64 = 1,
//	//	Window = 2,
//	Program = 3,
//};

// -------------------------------------------------------------------------------------------------

class ContextResource {
private:
	observer_ptr<Context> context;
//	std::unique_ptr<Impl> pimpl;

//	ResourceHandler<File>

//	std::unique_ptr<ResourceHandler<File>> handlerFile;
//	std::vector<std::shared_ptr<ProviderIOResource>> providersIO;
	std::vector<std::shared_ptr<ResourceProvider>> providers;

public:
	// TODO P4: Extra param for resource lifetime hint.
	//	File_view file(LifeTime lifeTimeHint, const boost::filesystem::path& filePath);
	//	UIFont2D_view font(LifeTime lifeTimeHint, const boost::filesystem::path& filePath);
	//	UIProgram_view program(LifeTime lifeTimeHint, const std::string& name);
	//	UIShader_view shader(LifeTime lifeTimeHint, const boost::filesystem::path& filePath);
	//	UITexture2D_view texture(LifeTime lifeTimeHint, const boost::filesystem::path& filePath);

	File_view file(const boost::filesystem::path& filePath);
	UIFont2D_view font(const boost::filesystem::path& filePath);
	UIProgram_view program(const std::string& name);
	UIShader_view shader(const boost::filesystem::path& filePath);
	UITexture2D_view texture(const boost::filesystem::path& filePath);

public:
	void load();
	void unload();
	ContextResource(observer_ptr<Context> context);
	~ContextResource();
};

struct ContextConfig {
	fvec4 defaultFontColor{1, 1, 1, 1};
	uint32_t defaultFontSize{12};
};

class ContextExecutorImpl;

class ContextExecutor {
private:
	std::unique_ptr<ContextExecutorImpl> pimpl;
public:
	ContextExecutor();
	~ContextExecutor();

	void async(ThreadAffinity aff, std::function<void() > func);
	void process();
	//	void sync(ThreadAffinity aff, std::function<void()> func);
};

struct Context {
	gl::GL gl;
	ContextResource resource;
	ContextConfig config;
	ContextExecutor executor;

	Context();

	void load();
	void refresh();
	void unload();
};

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv
