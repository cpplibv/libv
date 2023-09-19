// Project: libv.rev, File: src/libv/rev/effect/particle.hpp

#pragma once

// libv
#include <libv/algo/erase_if_unstable.hpp>
#include <libv/algo/sort.hpp>
#include <libv/math/angle.hpp>
#include <libv/math/constants.hpp>
#include <libv/math/vec.hpp>
#include <libv/noise/noise.hpp>
#include <libv/utility/chrono.hpp>
#include <libv/utility/random/xoroshiro128.hpp>
// #include <libv/math/quat.hpp>
// std
// #include <variant>
#include <memory>
#include <vector>
#include <span>
#include <random>
// pro
// #include <libv/rev/engine/render_pass.hpp>

// #include <fmt/printf.h>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

template <typename T>
struct Provider {
// 	struct Constant {
// 		T value;
// 	};
// 	struct UniformRange {
// 		T low;
// 		T high;
// 	};
// 	struct Curve {
// 		// ...
// 	};
// 	struct Gradient {
// 		// ...
// 	};
//
// 	std::variant<Constant, UniformRange> recipe;
};

// -------------------------------------------------------------------------------------------------

struct ParticleGPUData {
	libv::vec4f color;
	libv::vec2f packSizeRotation;
	// There is space for 2 additional float
};

struct Particle {
	libv::vec3f position;
	libv::vec3f velocity;
	float rotation = 0.f;
	float angular_velocity = 0.f;
// 	libv::quatf direction;
// 	libv::quatf angular_velocity;

	// IDEA: The less memory we access, the better: AOS -> SOA
	// IDEA: The less memory we write, the better: birthTime/deathTime instead of age/lifetime
	// float birthTime = 0;
	// float deathTime = 0;
	float age = 0;
	float lifetime = 0;

	libv::vec4f color = {1, 1, 1, 1};
	float size = 5.f;
};

class ParticleEffect;

struct ParticleModule {
	virtual void onParticleModuleUpdate(ParticleEffect& system, libv::time_duration_f delta_time) {
		(void) system;
		(void) delta_time;
	}
	virtual void onSpawn(ParticleEffect& system, std::span<Particle> particles) {
		(void) system;
		(void) particles;
	}
	virtual void onUpdate(ParticleEffect& system, libv::time_duration_f delta_time, std::span<Particle> particles) {
		(void) system;
		(void) delta_time;
		(void) particles;
	}
	virtual void onDeath(ParticleEffect& system, libv::time_duration_f delta_time, std::span<Particle> particles) {
		(void) system;
		(void) delta_time;
		(void) particles;
	}
	virtual ~ParticleModule() = default;
};

class ParticleEffect {
private:
	static constexpr int32_t PARTICLE_BATCH_MEMORY_SIZE = 16 * 1024; // 16 KB, L1 cache is 20-80 KB / core
	static constexpr int32_t PARTICLE_BATCH_SIZE = PARTICLE_BATCH_MEMORY_SIZE / sizeof(Particle);

private:
	// libv::vec3f boundsMin;
	// libv::vec3f boundsMax;

private:
	std::vector<Particle> particles;
	std::vector<std::unique_ptr<ParticleModule>> modules;
	// libv::vec3f baseColor = {1.f};
	// float intensity = 1.f;

private:
	enum class SimulationSpace {
		world,
		local,
	};

	libv::vec3f position_;
	// libv::vec3f velocity_;
	// SimulationSpace simulationSpace = SimulationSpace::world;
	// int32_t particleLimit = 1000;
	int32_t numNewSpawn = 0;
	// bool looping = true;
	// bool prewarm = true;
	// bool inherit_velocity = true;
	libv::xoroshiro128 rng_{0x5EED};

private:
	template <typename Func>
	LIBV_FORCE_INLINE void foreach_batch(int32_t start, int32_t count, Func&& func) {
		auto* it = particles.data() + start;

		while (count > PARTICLE_BATCH_SIZE) {
			func(std::span<Particle>(it, PARTICLE_BATCH_SIZE));
			count -= PARTICLE_BATCH_SIZE;
			it += PARTICLE_BATCH_SIZE;
		}

		func(std::span<Particle>(it, count));
	}

public:
	// !!! Test Hack
	mutable std::vector<libv::vec3f> positionCache;
	[[nodiscard]] void* positionData() const {
		positionCache.clear();
		for (const auto& particle : particles)
			positionCache.emplace_back(particle.position);

		return positionCache.data();
	}

	// !!! Test Hack
	mutable std::vector<ParticleGPUData> propertiesCache;
	[[nodiscard]] void* propertiesData() const {
		propertiesCache.clear();
		for (const auto& particle : particles)
			propertiesCache.emplace_back(particle.color, libv::vec2f{particle.size, particle.rotation});
		return propertiesCache.data();
	}

public:
	[[nodiscard]] constexpr inline int32_t numParticle() const noexcept {
		return static_cast<int32_t>(particles.size());
	}

	[[nodiscard]] constexpr inline libv::vec3f position() const noexcept {
		return position_;
	}

	[[nodiscard]] inline libv::xoroshiro128& rng() noexcept {
		return rng_;
	}

	void addModule(std::unique_ptr<ParticleModule>&& mod) {
		modules.emplace_back(std::move(mod));
	}

	void spawn(int32_t count) {
		numNewSpawn += count;
	}

	void sort(libv::vec3f eye) {
		libv::sort_unstable(particles, std::greater<>{}, [&](const auto& p) {
			return (eye - p.position).length_sq();
		});
	}

	// void update(libv::time_duration_f delta_time, libv::vec3f eye) {
	void update(libv::time_duration_f delta_time) {
		for (const auto& mod : modules)
			mod->onParticleModuleUpdate(*this, delta_time);

		if (numNewSpawn != 0) {
			auto start = static_cast<int32_t>(particles.size());
			particles.resize(particles.size() + numNewSpawn);

			foreach_batch(start, numNewSpawn, [&](auto batch) {
				for (const auto& mod : modules)
					mod->onSpawn(*this, batch);
			});
			numNewSpawn = 0;
		}

		// switch (simulationSpace) {
		// case SimulationSpace::world:
		// 	break;
		// case SimulationSpace::local:
		// 	break;
		// }

		// TODO P1: Make update and unstable remove if in one pass (with batching also included)
		foreach_batch(0, static_cast<int32_t>(particles.size()), [&](auto batch) {
			for (auto& particle : batch) {
				particle.age += delta_time.count();
				particle.position += particle.velocity * delta_time.count();
				particle.rotation += particle.angular_velocity * delta_time.count();
			}

			for (const auto& mod : modules)
				mod->onUpdate(*this, delta_time, batch);
		});

		const auto deathIt = libv::remove_if_unstable(particles, [](const Particle& particle) {
			return particle.age >= particle.lifetime;
		});

		foreach_batch(
				static_cast<int32_t>(std::distance(particles.begin(), deathIt)),
				static_cast<int32_t>(std::distance(deathIt, particles.end())),
				[&](auto batch) {

			for (const auto& mod : modules)
				mod->onDeath(*this, delta_time, batch);
		});

		particles.erase(deathIt, particles.end());

		// // Batching API
		// remove_if_unstable(system.particles,
		// 	if_lifetime_expired();
		//
		// 	Nentry = 16 * 1024 / sizeof(Particle);
		// 	if (number_kept_alive == Nentry)
		// 		module_chain.execute(system, span(last_Nentry));
		// );
		// module_chain.execute_update(system, span(leftover_last_Nentry));
		// for (16k batches)
		// 	module_chain.execute_death(system, span(block_of_16k_of_every_one_who_died));

		// for (auto& particle : system.particles) {
		// 	particle.position += particle.velocity * delta_time;
		// 	particle.velocity += gravity * delta_time;
		// }

		// sort(eye);
	}

	// void render(libv::rev::RenderPass& rev) {
	//
	// 	// rev.viewFrustum.aabb_in_frustum()
	// 	// if (!rev.viewFrustum.point_in_frustum())
	// 	//	return;
	//
	// 	// sort(rev.eye());
	// 	// rev.rendererParticle.render(*this);
	// }

	// void update_offscreen(libv::time_duration_f delta_time) {
	//
	// }
};

// -------------------------------------------------------------------------------------------------

struct ParticleModuleEmitter : ParticleModule {
	bool enabled = true;

	libv::vec3f offset;
	libv::vec3f velocity;
	// libv::quatf direction;
	// libv::quatf angular_velocity;

	float spawnRate = 10.f;
	float spawnCredit = 0.f;

	// Provider<float> spawnRate;
	// Provider<float> lifetime;

public:
	ParticleModuleEmitter(bool enabled, const libv::vec3f offset, const libv::vec3f velocity, float spawnRate) :
			enabled(enabled), offset(offset), velocity(velocity), spawnRate(spawnRate) {}

protected:
	virtual void onParticleModuleUpdate(ParticleEffect& system, libv::time_duration_f delta_time) override {
		if (!enabled)
			return;

		spawnCredit += spawnRate * delta_time.count();

		float spawnCount;
		spawnCredit = std::modf(spawnCredit, &spawnCount);
		system.spawn(static_cast<int32_t>(spawnCount));
	}

	virtual void onSpawn(ParticleEffect& system, std::span<Particle> particles) override {
		std::uniform_real_distribution distLifetime{5.f, 20.f};
		std::uniform_real_distribution distPosition{-0.1f, 0.1f};
		std::uniform_real_distribution distVelocityX{-10.f, 10.f};
		std::uniform_real_distribution distVelocityY{-10.f, 10.f};
		// std::uniform_real_distribution distVelocityX{0.f, 10.f};
		// std::uniform_real_distribution distVelocityY{0.f, 10.f};
		std::uniform_real_distribution distColor{0.f, 1.0f};
		std::uniform_real_distribution distRotation{0.f, libv::tau};
		std::uniform_real_distribution distAngularVelocity{libv::deg_to_rad(-90.f), libv::deg_to_rad(+90.f)};

		for (auto& particle : particles) {
			particle.position += offset;
			particle.position.x += distPosition(system.rng());
			particle.position.y += distPosition(system.rng());
			particle.position.z += distPosition(system.rng());
			particle.velocity += velocity;
			particle.rotation += distRotation(system.rng());
			particle.angular_velocity = distAngularVelocity(system.rng());
			particle.lifetime = distLifetime(system.rng());
			particle.velocity.x += distVelocityX(system.rng());
			particle.velocity.y += distVelocityY(system.rng());
			particle.color.x = distColor(system.rng());
			particle.color.y = distColor(system.rng());
			particle.color.z = distColor(system.rng());
		}
	}

	// virtual void onUpdate(ParticleEffect& system, libv::time_duration_f delta_time, std::span<Particle> particles) override {
	// }
};

struct ParticleModuleGravity : ParticleModule {
	libv::vec3f force{0.f, 0.f, -9.81f};

	ParticleModuleGravity() {}
	explicit ParticleModuleGravity(libv::vec3f force) : force(force) {}

protected:
	virtual void onUpdate(ParticleEffect& system, libv::time_duration_f delta_time, std::span<Particle> particles) override {
		(void) system;
		for (auto& particle : particles)
			particle.velocity += force * delta_time.count();
	}
};

struct ParticleModuleVortex : ParticleModule {
	libv::vec3f up = {0, 0, 1};
	float force = 1.0f;

protected:
	// virtual void onUpdate(ParticleEffect& system, libv::time_duration_f delta_time, std::span<Particle> particles) override {
	// }
};

struct ParticleModuleTurbulence : ParticleModule {
	float force = 1.0f;
	// float frequency = 1.0f;
	libv::time_duration_f moduleAge{};

public:
	explicit ParticleModuleTurbulence(float force) : force(force) {}

protected:
	virtual void onParticleModuleUpdate(ParticleEffect& system, libv::time_duration_f delta_time) override {
		(void) system;
		// !!! Need access to global time?
		moduleAge += delta_time;
	}
	virtual void onUpdate(ParticleEffect& system, libv::time_duration_f delta_time, std::span<Particle> particles) override {
		(void) system;
		(void) delta_time;
		// !!! This is not a real turbulence implementation, only a test dummy
		for (auto& particle : particles) {
			// const auto x = libv::noise_simplex_glsl(particle.position.x, particle.position.y);
			// const auto y = libv::noise_simplex_glsl(particle.position.y, particle.position.z);
			// const auto z = libv::noise_simplex_glsl(particle.position.z, particle.position.x);
			const auto x = libv::noise_simplex_glsl(-20.f + particle.position.x * 0.5f, 20.f + moduleAge.count() * 0.2f);
			const auto y = libv::noise_simplex_glsl(50.f + particle.position.y * 0.5f, 50.f + moduleAge.count() * 0.2f);
			const auto z = libv::noise_simplex_glsl(-50.f + particle.position.z * 0.5f, -50.f + moduleAge.count() * 0.2f);
			// const auto x = libv::noise_simplex(0x5EED, particle.position.x * 0.5f, moduleAge.count() * 0.2f);
			// const auto y = libv::noise_simplex(0x5EED, 50.f + particle.position.y * 0.5f, 50.f + moduleAge.count() * 0.2f);
			// const auto z = libv::noise_simplex(0x5EED, -50.f + particle.position.z * 0.5f, -50.f + moduleAge.count() * 0.2f);
			particle.velocity += libv::vec3f(x, y, z) * force;
		}
	}
};

struct ParticleModuleNoise : ParticleModule {

protected:
	// virtual void onUpdate(ParticleEffect& system, libv::time_duration_f delta_time, std::span<Particle> particles) override {
	// }
};

struct ParticleModuleDrag : ParticleModule {

protected:
	// virtual void onUpdate(ParticleEffect& system, libv::time_duration_f delta_time, std::span<Particle> particles) override {
	// }
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
