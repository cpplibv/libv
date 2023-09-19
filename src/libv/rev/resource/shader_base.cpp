// Project: libv.rev, File: src/libv/rev/resource/shader_base.cpp

// hpp
#include <libv/rev/resource/shader.hpp>
// pro
#include <libv/rev/resource/shader_internal.hxx>
#include <libv/rev/resource/shader_loader.hpp>
#include <libv/rev/resource/shader_loader_internal.hxx>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

BaseShader::BaseShader(std::shared_ptr<InternalShader>&& internal_) :
	internal_(std::move(internal_)) {
}

BaseShader::BaseShader(const std::shared_ptr<InternalShader>& internal_) :
	internal_(internal_) {
}

BaseShader::BaseShader(ShaderLoader& loader, libv::type_uid uniformTID, BaseShader::ucc_type ucc, libv::gl::ShaderType type0, std::string path0) :
	internal_(loader.self->internal_load(uniformTID, ucc, type0, std::move(path0))) {
}

BaseShader::BaseShader(ShaderLoader& loader, libv::type_uid uniformTID, BaseShader::ucc_type ucc, libv::gl::ShaderType type0, std::string path0, libv::gl::ShaderType type1, std::string path1) :
	internal_(loader.self->internal_load(uniformTID, ucc, type0, std::move(path0), type1, std::move(path1))) {
}

BaseShader::BaseShader(ShaderLoader& loader, libv::type_uid uniformTID, BaseShader::ucc_type ucc, libv::gl::ShaderType type0, std::string path0, libv::gl::ShaderType type1, std::string path1, libv::gl::ShaderType type2, std::string path2) :
	internal_(loader.self->internal_load(uniformTID, ucc, type0, std::move(path0), type1, std::move(path1), type2, std::move(path2))) {
}

BaseShader::~BaseShader() {
	// For the sake of forward declared types
}

ShaderID BaseShader::id() const noexcept {
	return internal_->id();
}

const std::string& BaseShader::name() const noexcept {
	return internal_->name_;
}

int BaseShader::current_version() const noexcept {
	return internal_->current_version;
}

int BaseShader::load_version() const noexcept {
	return internal_->load_version;
}

libv::glr::Program& BaseShader::program() {
	return internal_->program;
}

const libv::glr::Program& BaseShader::program() const {
	return internal_->program;
}

detail::BaseUniformContainer& BaseShader::base_uniform() {
	return *internal_->uniformContainer;
}

const detail::BaseUniformContainer& BaseShader::base_uniform() const {
	return *internal_->uniformContainer;
}
// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
