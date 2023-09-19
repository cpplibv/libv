// Project: libv.rev, File: src/libv/rev/_idea/particle_static.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/algo/erase_if_unstable.hpp>
#include <libv/utility/random/xoroshiro128.hpp>
// #include <libv/math/quat.hpp>
// std
// #include <variant>
#include <memory>
#include <vector>
#include <span>
#include <random>
#include <tuple>


#include <fmt/printf.h>












// =================================================================================================
//
// NOTE:
// This is a rough prototype to check on the performance penalty of dynamic module composition
// Most important improvement could be that the ParticleSystem could be a fix non template type, with using
// one level of indirection of the statically composed modules
//
// The overall speedup with this kind of static particle system is about: 25%
// Given the convenience and flexibility of the dynamic system I will go with that variant for now
//
// 82492, 0Dyn     1200.000us
// 82492, 320kDyn  693.292us
// 82492, 16kDyn   487.280us
// 82459, Static   369.649us
//
// If necessary for ironed out effects a static composition could be implemented for that 25% gain
// There is also a possibility of a static composition module in the dynamic system
//
// =================================================================================================















namespace idea {

// -------------------------------------------------------------------------------------------------

using Duration = float;

// -------------------------------------------------------------------------------------------------

struct Particle {
	libv::vec3f position;
	libv::vec3f velocity;

	// IDEA: The less memory we access, the better: AOS -> SOA
	// IDEA: The less memory we write, the better: birthTime/deathTime instead of age/lifetime
	// float birthTime = 0;
	// float deathTime = 0;
	float age = 0;
	float lifetime = 0;

	libv::vec4f color;
	float size = 1.f;
};

template <typename... Args>
struct ParticleSystemStatic;

struct Module {
	template <typename System>
	void onModuleUpdate(System& system, Duration delta_time) { }

	template <typename System>
	void onSpawn(System& system, Particle& particles) { }
	template <typename System>
	void onUpdate(System& system, Duration delta_time, Particle& particles) { }
	template <typename System>
	void onDeath(System& system, Duration delta_time, Particle& particles) { }
};

template <typename... Args>
struct ParticleSystemStatic {
private:
	static constexpr int32_t PARTICLE_BATCH_MEMORY_SIZE = 16 * 1024; // 16 KB, L1 cache is 20-80 KB / core
	static constexpr int32_t PARTICLE_BATCH_SIZE = PARTICLE_BATCH_MEMORY_SIZE / sizeof(Particle);

private:
	// bool looping = true;
	// bool prewarm = true;
	// Provider<float> lifetime;

	// uint32_t particleLimit = 100'000;

private:
	std::vector<Particle> particles;
	std::tuple<Args...> modules;
	// std::vector<std::unique_ptr<Module>> modules;
	// std::vector<small_unique_ptr<Module, 32>> modules;
	// libv::poly_lin_list<Module> modules;

	int32_t numNewSpawn = 0;

	libv::vec3f boundsMin;
	libv::vec3f boundsMax;

private:
	libv::xoroshiro128 rng_{0x5EED};

	libv::vec3f position_;

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
	explicit ParticleSystemStatic(Args&&... modules) : modules(std::forward<Args>(modules)...) {}

public:
	[[nodiscard]] constexpr inline int32_t numParticle() const noexcept {
		return particles.size();
	}

	[[nodiscard]] constexpr inline libv::vec3f position() const noexcept {
		return position_;
	}

	[[nodiscard]] inline libv::xoroshiro128& rng() noexcept {
		return rng_;
	}

	void addModule(std::unique_ptr<Module>&& mod) {
		modules.emplace_back(std::move(mod));
	}

	void spawn(int32_t count) {
		numNewSpawn += count;
	}

	void update(Duration delta_time) {
		// static constexpr int32_t ASSUMED_L1_CACHE_SIZE = 16 * 1024;

		std::apply([&](auto&... mod) {
			(mod.onModuleUpdate(*this, delta_time), ...);
		}, modules);

		if (numNewSpawn != 0) {
			auto start = static_cast<int32_t>(particles.size());
			particles.resize(particles.size() + numNewSpawn);

			for (auto it = particles.begin() + start; it != particles.end(); ++it) {
				std::apply([&](auto&... mod) {
					(mod.onSpawn(*this, *it), ...);
				}, modules);
			}
			numNewSpawn = 0;
		}

		// TODO P1: Make update and unstable remove if in one pass (with batching also included)
		for (auto& particle : particles) {
			particle.age += delta_time;
			particle.position += particle.velocity * delta_time;

			std::apply([&](auto&... mod) {
				(mod.onUpdate(*this, delta_time, particle), ...);
			}, modules);
		}

		const auto deathIt = libv::remove_if_unstable(particles, [](const Particle& particle) {
			return particle.age >= particle.lifetime;
		});

		for (auto it = deathIt; it != particles.end(); ++it) {
			std::apply([&](auto&... mod) {
				(mod.onDeath(*this, delta_time, *it), ...);
			}, modules);
		}

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
	}

	// void update_offscreen(Duration delta_time) {
	//
	// }
};

struct ModuleEmitter : Module {
	bool enabled = true;

	libv::vec3f offset;
	libv::vec3f velocity;
	// libv::quatf direction;
	// libv::quatf angular_velocity;

	float spawnRate = 10.f;
	float spawnCredit = 0.f;

public:
	ModuleEmitter(bool enabled, const libv::vec3f offset, const libv::vec3f velocity, float spawnRate) :
			enabled(enabled), offset(offset), velocity(velocity), spawnRate(spawnRate) {}

	template <typename System>
	void onModuleUpdate(System& system, Duration delta_time) {
		if (!enabled)
			return;

		spawnCredit += spawnRate * delta_time;

		float spawnCount;
		spawnCredit = std::modf(spawnCredit, &spawnCount);
		system.spawn(static_cast<int32_t>(spawnCount));
	}

	template <typename System>
	void onSpawn(System& system, Particle& particle) {
		std::uniform_real_distribution distLifetime{5.f, 10.f};

			particle.position += offset;
			particle.velocity += velocity;
			particle.lifetime = distLifetime(system.rng());
	}
};

struct ModuleGravity : Module {
	libv::vec3f force{0.f, 0.f, -9.81f};

	ModuleGravity() {}
	explicit ModuleGravity(libv::vec3f force) : force(force) {}

public:
// protected:
	template <typename System>
	void onUpdate(System& system, Duration delta_time, Particle& particle) {
			particle.velocity += force * delta_time;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace idea
