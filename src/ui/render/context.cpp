// File: ui_render_vbo.cpp, Created on 2015. április 30. 0:09, Author: Vader

// hpp
#include <libv/ui/render/context.hpp>
// ext
//#include <boost/range/adaptor/reversed.hpp>
#include <range/v3/view/reverse.hpp>
// libv
#include <libv/read_file.hpp>
// std
#include <queue>
// pro
#include <libv/ui/log.hpp>
#include <libv/ui/resource/resource_provider.hpp>
#include <libv/ui/resource/resource_provider_impl.hpp>
//#include <libv/ui/resource/file.hpp>
//#include <libv/ui/resource/font.hpp>
//#include <libv/ui/resource/program.hpp>
//#include <libv/ui/resource/shader.hpp>
//#include <libv/ui/resource/texture.hpp>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

File_view ContextResource::file(const boost::filesystem::path& filePath) {
	LIBV_LOG_UI_RESOURCE_TRACE("Request file [%s]", filePath);
	for (const auto& provider : providers | ranges::view::reverse) {
		auto res = provider->file(filePath);
		if (res)
			return res;
	}
	LIBV_LOG_UI_RESOURCE_WARN("Failed to provide file for [%s]. Returning burnt in default.", filePath);
	assert(false);
	return nullptr; //<<< burnt in default here
}

UIFont2D_view ContextResource::font(const boost::filesystem::path& filePath) {
	LIBV_LOG_UI_RESOURCE_TRACE("Request font [%s]", filePath);
	for (const auto& provider : providers | ranges::view::reverse) {
		auto res = provider->font(filePath);
		if (res)
			return res;
	}
	LIBV_LOG_UI_RESOURCE_WARN("Failed to provide font for [%s]. Returning burnt in default.", filePath);
	assert(false);
	return nullptr; //<<< burnt in default here
}

UIProgram_view ContextResource::program(const std::string& name) {
	LIBV_LOG_UI_RESOURCE_TRACE("Request program [%s]", name);
	for (const auto& provider : providers | ranges::view::reverse) {
		auto res = provider->program(name);
		if (res)
			return res;
	}
	LIBV_LOG_UI_RESOURCE_WARN("Failed to provide program for [%s]. Returning burnt in default.", name);
	assert(false);
	return nullptr; //<<< burnt in default here
}

UIShader_view ContextResource::shader(const boost::filesystem::path& filePath) {
	LIBV_LOG_UI_RESOURCE_TRACE("Request shader [%s]", filePath);
	for (const auto& provider : providers | ranges::view::reverse) {
		auto res = provider->shader(filePath);
		if (res)
			return res;
	}
	LIBV_LOG_UI_RESOURCE_WARN("Failed to provide shader for [%s]. Returning burnt in default.", filePath);
	assert(false);
	return nullptr; //<<< burnt in default here
}

UITexture2D_view ContextResource::texture(const boost::filesystem::path& filePath) {
	LIBV_LOG_UI_RESOURCE_TRACE("Request texture [%s]", filePath);
	for (const auto& provider : providers | ranges::view::reverse) {
		auto res = provider->texture(filePath);
		if (res)
			return res;
	}
	LIBV_LOG_UI_RESOURCE_WARN("Failed to provide texture for [%s]. Returning burnt in default.", filePath);
	assert(false);
	return nullptr; //<<< burnt in default here
}

void ContextResource::load() {
	LIBV_LOG_UI_RESOURCE_TRACE("Load Context Resource");

	providers.emplace_back(std::make_shared<ResourceProviderImpl>(context));
}

void ContextResource::unload() {
	LIBV_LOG_UI_RESOURCE_TRACE("Unload Context Resource");

	providers.clear();
}

// -------------------------------------------------------------------------------------------------

ContextResource::ContextResource(observer_ptr<Context> context) : context(context) { }

ContextResource::~ContextResource() { }

// =================================================================================================

struct ContextExecutorImpl {
	std::mutex mutex;
	std::queue<std::pair<ThreadAffinity, std::function<void()>>> que;
};

// -------------------------------------------------------------------------------------------------

ContextExecutor::ContextExecutor() :
	pimpl(std::make_unique<ContextExecutorImpl>()){ }

ContextExecutor::~ContextExecutor() { }

void ContextExecutor::async(ThreadAffinity aff, std::function<void()> func) {
	std::lock_guard<std::mutex> lk(pimpl->mutex);
	pimpl->que.emplace(aff, std::move(func));
}

void ContextExecutor::process() {
	std::pair<ThreadAffinity, std::function<void()>> cmd;
	// TODO P3: ThreadAffinity ignored

	while (true) {
		{
			std::lock_guard<std::mutex> lk(pimpl->mutex);
			if (pimpl->que.empty())
				return;
			cmd = std::move(pimpl->que.front());
			pimpl->que.pop();
		}
		cmd.second();
	}
}

// -------------------------------------------------------------------------------------------------

Context::Context() : resource(make_observer(this)) { }

void Context::load() {
	resource.load();
}

void Context::refresh() {
	executor.process();
//	resource.refresh();
}

void Context::unload() {
	resource.unload();
}

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv
