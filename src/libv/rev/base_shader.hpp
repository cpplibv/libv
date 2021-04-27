// Project: libv, File: app/space/shader.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <libv/rev/fwd.hpp>
// libv
#include <libv/glr/fwd.hpp>
// std
#include <memory>
#include <string>
#include <typeindex>


namespace libv {
namespace rev {
namespace detail {

// -------------------------------------------------------------------------------------------------

class BaseUniformContainer {
public:
	virtual void bind_assign(libv::glr::Program& program) = 0;
	virtual ~BaseUniformContainer() = default;
};

} // namespace detail ------------------------------------------------------------------------------

class BaseShader {
	using ucc_type = std::unique_ptr<detail::BaseUniformContainer>(*)(); /// Uniform container creator function

private:
	std::shared_ptr<InternalShader> internal_;

public:
	explicit BaseShader(std::shared_ptr<InternalShader>&& internal_);
	explicit BaseShader(const std::shared_ptr<InternalShader>& internal_);

	inline BaseShader(const BaseShader&) noexcept = default;
	inline BaseShader& operator=(const BaseShader&) & noexcept = default;
	inline BaseShader(BaseShader&&) noexcept = default;
	inline BaseShader& operator=(BaseShader&&) & noexcept = default;

	BaseShader(ShaderLoader& loader, std::type_index uniformTID, ucc_type ucc, libv::gl::ShaderType type0, std::string path0, libv::gl::ShaderType type1, std::string path1);
	BaseShader(ShaderLoader& loader, std::type_index uniformTID, ucc_type ucc, libv::gl::ShaderType type0, std::string path0, libv::gl::ShaderType type1, std::string path1, libv::gl::ShaderType type2, std::string path2);

	~BaseShader();

public:
	// introspection stuff comes here
	// .
	// .
	// .

	// span<stage> stages();
	[[nodiscard]] ShaderID id() const; /// Not OpenGL ID, it is C++ Unique ID
	[[nodiscard]] const std::string& name() const;

	[[nodiscard]] libv::glr::Program& program();
	[[nodiscard]] const libv::glr::Program& program() const;

public:
	[[nodiscard]] friend inline bool operator==(const BaseShader& lhs, const BaseShader& rhs) noexcept {
		return lhs.internal_ == rhs.internal_;
	}
	[[nodiscard]] friend inline bool operator!=(const BaseShader& lhs, const BaseShader& rhs) noexcept {
		return lhs.internal_ != rhs.internal_;
	}
	[[nodiscard]] friend inline bool operator<(const BaseShader& lhs, const BaseShader& rhs) noexcept {
		return lhs.internal_ < rhs.internal_;
	}
	[[nodiscard]] friend inline bool operator<=(const BaseShader& lhs, const BaseShader& rhs) noexcept {
		return lhs.internal_ <= rhs.internal_;
	}
	[[nodiscard]] friend inline bool operator>(const BaseShader& lhs, const BaseShader& rhs) noexcept {
		return lhs.internal_ > rhs.internal_;
	}
	[[nodiscard]] friend inline bool operator>=(const BaseShader& lhs, const BaseShader& rhs) noexcept {
		return lhs.internal_ >= rhs.internal_;
	}

protected:
	[[nodiscard]] detail::BaseUniformContainer& base_uniform();
	[[nodiscard]] const detail::BaseUniformContainer& base_uniform() const;
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
