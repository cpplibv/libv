// File: resource_provider.cpp Author: Vader Created on 2016. september 07., 13:19

// hpp
#include <libv/ui/resource/resource_provider_impl.hpp>
// std
#include <type_traits>
// pro
#include <libv/ui/log.hpp>
#include <libv/ui/render/context.hpp>
#include <libv/ui/resource/file.hpp>
#include <libv/ui/resource/font.hpp>
#include <libv/ui/resource/program.hpp>
#include <libv/ui/resource/resource_fwd.hpp>
#include <libv/ui/resource/shader.hpp>
#include <libv/ui/resource/texture.hpp>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

template <typename T>
struct ResourceWrapper {
	std::atomic<int64_t> ref_count{0};
	T resource;

	template <typename... Args>
	inline explicit ResourceWrapper(Args&&... args) :
		resource(std::forward<Args>(args)...) { }

	inline ResourceView<T> view() {
		return ResourceView<T>(&ref_count, &resource);
	}

	ResourceWrapper(ResourceWrapper<T>&& other) = default;
	ResourceWrapper<T>& operator=(ResourceWrapper<T>&& right) = default;
};

// -------------------------------------------------------------------------------------------------

namespace {

template <typename T>
void loadSequence(observer_ptr<Context> context, const ResourceView<T>& res_view, size_t i = 0) {
	constexpr auto max_step = std::extent<decltype(T::ResourceLoadSequence)>::value - 1;

//	context->executor.async(T::ResourceLoadSequence[i].affinity, [context, res_view, i] {
		auto result = ((*res_view).*T::ResourceLoadSequence[i].func)();
		LIBV_LOG_UI_RESOURCE_TRACE("Loading %s step %d/%d resulted [%d]", T::ResourceLabel, i + 1, max_step + 1, to_value(result));

		//if (i < std::size(T::ResourceLoadSequence) // C++17
		if (i < max_step)
			loadSequence(context, res_view, i + 1);
//		});
}

template <typename T>
void unloadSequence(observer_ptr<Context> context, const ResourceView<T>& res_view, size_t i = 0) {
	constexpr auto max_step = std::extent<decltype(T::ResourceUnloadSequence)>::value - 1;

//	context->executor.async(T::ResourceLoadSequence[i].affinity, [context, res_view, i] {
		auto result = ((*res_view).*T::ResourceUnloadSequence[i].func)();
		LIBV_LOG_UI_RESOURCE_TRACE("Unloading %s step %d/%d resulted [%d]", T::ResourceLabel, i + 1, max_step + 1, to_value(result));

		//if (i < std::size(T::ResourceLoadSequence) // C++17
		if (i < max_step)
			unloadSequence(context, res_view, i + 1);
//		});
}

template <typename T, typename Cache, typename Key>
ResourceView<T> cacheLookup(Cache& cache, const Key& key) {
	auto it = cache.find(key);
	if (it != cache.end()) {
		LIBV_LOG_UI_RESOURCE_TRACE("Cache hit on %s [%s]", T::ResourceLabel, key);

		auto& wrapper = *it->second;
		return wrapper.view();
	}
	LIBV_LOG_UI_RESOURCE_TRACE("Cache miss on %s [%s]", T::ResourceLabel, key);

	return ResourceView<T>();
}

template <typename T, typename Cache, typename Key, typename... Args>
ResourceView<T> cacheStore(Cache& cache, const Key& key, Args&&... args) {
	auto wrapper_up = std::make_unique<ResourceWrapper < T >> (std::forward<Args>(args)...);
	auto res_view = wrapper_up->view();
	cache.emplace(key, std::move(wrapper_up));

	return res_view;
}

} // namespace

// -------------------------------------------------------------------------------------------------

ResourceProviderImpl::ResourceProviderImpl(observer_ptr<Context> context) : context(context) { }

ResourceProviderImpl::~ResourceProviderImpl() { }

File_view ResourceProviderImpl::file(const boost::filesystem::path& filePath) {
	std::lock_guard<std::recursive_mutex> lock(mutex);

	if (auto res_view = cacheLookup<File>(cacheFile, filePath))
		return res_view;

	auto res_view = cacheStore<File>(cacheFile, filePath, filePath);
	loadSequence(context, res_view);

	return res_view;
}

UIFont2D_view ResourceProviderImpl::font(const boost::filesystem::path& filePath) {
	boost::filesystem::path resolvedFilePath = filePath;
	if (resolvedFilePath == "default")
		resolvedFilePath = DEFAULT_FONT_TTF_FILEPATH;

	std::lock_guard<std::recursive_mutex> lock(mutex);

	LIBV_LOG_UI_RESOURCE_TRACE("Resolved font [%s] to [%s]", filePath, resolvedFilePath);

	if (auto res_view = cacheLookup<UIFont2D>(cacheFont, resolvedFilePath))
		return res_view;

	auto file = context->resource.file(resolvedFilePath);

	auto res_view = cacheStore<UIFont2D>(cacheFont, resolvedFilePath, file);
	loadSequence(context, res_view);

	return res_view;
}

UIShader_view ResourceProviderImpl::shader(const boost::filesystem::path& filePath) {
	std::lock_guard<std::recursive_mutex> lock(mutex);

	if (auto res_view = cacheLookup<UIShader>(cacheShader, filePath))
		return res_view;

	auto file = context->resource.file(filePath);

	auto res_view = cacheStore<UIShader>(cacheShader, filePath, file);
	loadSequence(context, res_view);

	return res_view;
}

UIProgram_view ResourceProviderImpl::program(const std::string& name) {
	auto resolvedFilePath = name;
	if (resolvedFilePath == "default")
		resolvedFilePath = DEFAULT_PROGRAM_FILEPATH;

	std::lock_guard<std::recursive_mutex> lock(mutex);

	LIBV_LOG_UI_RESOURCE_TRACE("Resolved program [%s] to [%s]", name, resolvedFilePath);

	if (auto res_view = cacheLookup<UIProgram>(cacheProgram, resolvedFilePath))
		return res_view;

	auto fs = context->resource.shader(resolvedFilePath + ".fs");
	auto vs = context->resource.shader(resolvedFilePath + ".vs");

	auto res_view = cacheStore<UIProgram>(cacheProgram, resolvedFilePath, name, fs, vs);
	loadSequence(context, res_view);
	return res_view;
}

UITexture2D_view ResourceProviderImpl::texture(const boost::filesystem::path& filePath) {
	std::lock_guard<std::recursive_mutex> lock(mutex);

	if (auto res_view = cacheLookup<UITexture2D>(cacheTexture, filePath))
		return res_view;

	auto file = context->resource.file(filePath);

	auto res_view = cacheStore<UITexture2D>(cacheTexture, filePath, file);
	loadSequence(context, res_view);

	return res_view;
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv