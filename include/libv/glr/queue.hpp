// File: queue.hpp - Created on 2017.11.25. 05:06 - Author: Vader

#pragma once

// libv
#include <libv/gl/matrix_stack.hpp>
#include <libv/math/mat.hpp>
#include <libv/math/vec.hpp>
#include <libv/utility/guard.hpp>
// std
#include <bitset>
#include <functional>
// pro
#include <libv/glr/mesh.hpp>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

struct Queue {
public:
	gl::MatrixStack<libv::mat4f> model;
	gl::MatrixStack<libv::mat4f> view;
	gl::MatrixStack<libv::mat4f> projection;


	enum {
		DepthTest = 1,
		FrontFaceCCW = 2, // true = CCW, false = CW
	};

	using State = std::bitset<64>;
	State state = DepthTest | FrontFaceCCW;

private:
//	struct Task {
//		State state;
//
//		size_t first_texture;
//		size_t first_uniform;
//		uint8_t num_texture;
//		uint16_t num_uniform;
//	};
//
//	std::vector<Task> queue;
//	std::vector<std::pair<gl::TextureChannel, Texture>> textures;
//	std::vector<std::pair<gl::TextureChannel, gl::Texture2D>> textures;

	std::vector<std::function<void()>> dummy; // !!! State and mesh, not just a function dropped here

public:
	inline const libv::mat4f mvp() const {
		return projection * view * model;
	}
	inline const libv::vec3f eye() const {
		return libv::vec::xyz(view.top().inverse_copy()[3]);
	}
	inline const libv::mat4f getMatrixTexture() const {
		return libv::mat4f{
				0.5f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.5f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.5f, 0.0f,
				0.5f, 0.5f, 0.5f, 1.0f};
	}

private:
	[[nodiscard]] inline auto changeState(size_t bit, bool value) noexcept {
		auto past = state.test(bit);
		state.set(bit, value);
		return libv::Guard([this, bit, past] { state.set(bit, past); });
	}

public:
	[[nodiscard]] inline auto enableDepthTest() noexcept {
		return changeState(DepthTest, true);
	}
	[[nodiscard]] inline auto disableDepthTest() noexcept {
		return changeState(DepthTest, false);
	}
	[[nodiscard]] inline auto setFrontFaceCCW() noexcept {
		return changeState(FrontFaceCCW, true);
	}
	[[nodiscard]] inline auto setFrontFaceCW() noexcept {
		return changeState(FrontFaceCCW, false);
	}

public:
//	[[nodiscard]] inline auto bindTexture(gl::TextureChannel channel, const gl::Texture2DGuard& texture) noexcept {
////		return changeState(FrontFace, false);
//		(void) channel;
//		(void) texture;
//		return 0;
//	}
//	[[nodiscard]] inline auto bindTexture(gl::TextureChannel channel, const gl::TextureCubeGuard& texture) noexcept {
////		return changeState(FrontFace, false);
//		(void) channel;
//		(void) texture;
//		return 0;
//	}

public:
	void render(const Mesh& mesh) {
		dummy.emplace_back([mesh_ = mesh] () mutable {
			mesh_.render();
		});
	}

	void execute() { // !!! Not public
		for (const auto& f : dummy){
			f();
		}
		dummy.clear();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
