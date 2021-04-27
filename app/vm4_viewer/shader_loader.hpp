// Project: libv, File: app/vm4_viewer/shader_loader.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/gl/gl_fwd.hpp>
#include <libv/utility/function_ref.hpp>
#include <libv/utility/observer_ref.hpp>
// std
#include <memory>


namespace app {

// -------------------------------------------------------------------------------------------------

struct BaseShader;

struct ShaderReportSuccess {
	BaseShader& shader;
};

struct ShaderReportFailure {
	BaseShader& shader;
	std::string reason;
};

// -------------------------------------------------------------------------------------------------

class ShaderLoader {
public:
	using callback_success_type = libv::function_ref<void(const app::ShaderReportSuccess&)>;
	using callback_failure_type = libv::function_ref<void(const app::ShaderReportFailure&)>;

private:
	std::unique_ptr<class InternalShaderLoader> self;

public:
	ShaderLoader();
	~ShaderLoader();

public:
	void subscribe(const libv::observer_ref<BaseShader> shader);
	void unsubscribe(const libv::observer_ref<BaseShader> shader);

public:
	// TODO P5: This could be a C++20 co-routine
	void update(libv::gl::GL& gl, callback_success_type success_cb, callback_failure_type failure_cb);
};

// -------------------------------------------------------------------------------------------------

} // namespace app
