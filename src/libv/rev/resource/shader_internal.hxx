// Project: libv.rev, File: src/libv/rev/internal_shader.lpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <libv/rev/fwd.hpp>
// libv
#include <libv/fsw/token.hpp>
#include <libv/gl/enum.hpp>
#include <libv/glr/program.hpp>
#include <libv/utility/type_uid.hpp>
// std
#include <memory>
#include <string>
#include <vector>
// pro
#include <libv/rev/glsl/glsl_source_code.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

struct InternalIncludedSource {
	std::string path;
	libv::fsw::Token fsw_token;
};

struct InternalShaderStage {
	libv::gl::ShaderType type;
	std::string source_path;
	std::vector<std::string> defines;

	GLSLSourceCode source;
	std::vector<InternalIncludedSource> included_sources;
};

class InternalShader : public std::enable_shared_from_this<InternalShader> {
public:
	libv::glr::Program program;
	std::unique_ptr<detail::BaseUniformContainer> uniformContainer;

	int current_version = -1;
	int load_version = -1;
	std::string name_;
	std::weak_ptr<InternalShaderLoader> loader;

	libv::type_uid uniformTID;
	std::vector<InternalShaderStage> stages;

public:
	InternalShader(
			std::weak_ptr<InternalShaderLoader> loader,
			std::unique_ptr<detail::BaseUniformContainer>&& uniformContainer,
			libv::type_uid uniformTID);

	~InternalShader();

	[[nodiscard]] constexpr inline ShaderID id() const noexcept {
		return std::bit_cast<ShaderID>(this);
	}

public:
	void finish();

public:
	[[nodiscard]] int compare(libv::type_uid uniformTID, libv::gl::ShaderType t0, const std::string& p0, libv::gl::ShaderType t1, const std::string& p1) const noexcept;
	[[nodiscard]] int compare(libv::type_uid uniformTID, libv::gl::ShaderType t0, const std::string& p0, libv::gl::ShaderType t1, const std::string& p1, libv::gl::ShaderType t2, const std::string& p2) const noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
