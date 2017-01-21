// File: resource_provider.hpp Author: Vader Created on 2016. augusztus 25., 5:22

#pragma once

// ext
#include <boost/filesystem/path.hpp>
#include <boost/container/flat_map.hpp>
// libv
#include <libv/memory.hpp>
//#include <libv/worker_thread.hpp>
// std
#include <vector>
#include <memory>
#include <mutex>
// pro
#include <libv/ui/resource/resource_fwd.hpp>
#include <libv/ui/resource/resource_provider.hpp>
//#include <libv/ui/resource/program.hpp>
//#include <libv/ui/resource/shader.hpp>

namespace libv {
namespace ui {

class Context;

template <typename T>
struct ResourceWrapper;

//template <typename T>
//struct ResourceWrapper {
//	std::atomic<int64_t> ref_count{0};
//	T resource;
//
//	template <typename... Args>
//	inline explicit ResourceWrapper(Args&&... args) :
//		resource(std::forward<Args>(args)...) { }
//
//	inline ResourceView<T> view() {
//		return ResourceView<T>(&ref_count, &resource);
//	}
//
//	ResourceWrapper(ResourceWrapper<T>&& other) = default;
//	ResourceWrapper<T>& operator=(ResourceWrapper<T>&& right) = default;
//};

// -------------------------------------------------------------------------------------------------

struct ResourceProviderImpl : ResourceProvider {
public:
	ResourceProviderImpl(observer_ptr<Context> context);

	virtual ~ResourceProviderImpl();

private:
	//	std::vector<ProgramArgs> loadQueProgram;
	//	std::vector<UIShader_view> loadQueShader;
	//	std::vector<UITexture2D_view> loadQueTexture;
	//	std::vector<UIProgram_view> unloadQueProgram;
	//	std::vector<UIShader_view> unloadQueShader;
	//	std::vector<UITexture2D_view> unloadQueTexture;
	std::recursive_mutex mutex;

	observer_ptr<Context> context;
	std::vector<std::function<void()>> tasks;

	boost::container::flat_map<boost::filesystem::path, std::unique_ptr<ResourceWrapper<File>>> cacheFile;
	boost::container::flat_map<boost::filesystem::path, std::unique_ptr<ResourceWrapper<UIFont2D>>> cacheFont;
	boost::container::flat_map<std::string, std::unique_ptr<ResourceWrapper<UIProgram>>> cacheProgram;
	boost::container::flat_map<boost::filesystem::path, std::unique_ptr<ResourceWrapper<UIShader>>> cacheShader;
	boost::container::flat_map<boost::filesystem::path, std::unique_ptr<ResourceWrapper<UITexture2D>>> cacheTexture;

public:
	virtual File_view file(const boost::filesystem::path& filePath) override;
	virtual UIFont2D_view font(const boost::filesystem::path& filePath) override;
	virtual UIProgram_view program(const std::string& name) override;
	virtual UIShader_view shader(const boost::filesystem::path& filePath) override;
	virtual UITexture2D_view texture(const boost::filesystem::path& filePath) override;
};

// -------------------------------------------------------------------------------------------------

//class ProgramDescription;
//
//struct ProviderProgramDescription {
//	ProgramDescription programDescription(const std::string& name);
//	virtual ~ProviderProgramDescription();
//
//private:
//	virtual ProgramDescription doProgramDescription(const std::string& name) = 0;
//};
//
//struct ProviderProgramDescriptionDefault : ProviderProgramDescription {
//private:
//	boost::container::flat_map<std::string, ProgramDescription> cacheProgram;
//public:
//	virtual ~ProviderResourceImpl();
//	void loadProgram(const std::string& name, const boost::filesystem::path& filePathA, const boost::filesystem::path& filePathB);
//	void loadProgram(const std::string& name, const boost::filesystem::path& filePathA, const boost::filesystem::path& filePathB, const boost::filesystem::path& filePathC);
//private:
//	ProgramDescription doProgramDescription(const std::string& name) override;
//};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv