// Project: libv.rev, File: src/libv/rev/effect/particle.cpp

// hpp
// #include <libv/rev/effect/particle.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------


// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv

// =================================================================================================
// =================================================================================================
// =================================================================================================
// =================================================================================================
// =================================================================================================
//
// struct MinMaxCurve {
//	// constant - Set the constant value.
//	// constantMax - Set a constant for the upper bound.
//	// constantMin - Set a constant for the lower bound.
//	// curve - Set the curve.
//	// curveMax - Set a curve for the upper bound.
//	// curveMin - Set a curve for the lower bound.
//	// curveMultiplier - Set a multiplier to apply to the curves.
//	// mode - Set the mode that the min-max curve uses to evaluate values.
//
//	// Evaluate - Manually query the curve to calculate values based on what mode it is in.
// };
//
// struct MinMaxGradient {
//	// color - Set a constant color.
//	// colorMax - Set a constant color for the upper bound.
//	// colorMin - Set a constant color for the lower bound.
//	// gradient - Set the gradient.
//	// gradientMax - Set a gradient for the upper bound.
//	// gradientMin - Set a gradient for the lower bound.
//	// mode - Set the mode that the Min-Max Gradient uses to evaluate colors.
//
//	// Evaluate - Manually query the gradient to calculate colors based on what mode it is in.
// };
//
// // ---
//
// struct Modifiers {
//	// Either
//	//	Constant;
//	//	Curve;
//	//	Random Between Two Constants;
//	//	Random Between Two Curves;
//	//	Random Color;
// };
//
// // ---
//
// struct Particle {
//	// angularVelocity - The angular velocity of the particle.
//	// angularVelocity3D - The 3D angular velocity of the particle.
//	// animatedVelocity - The animated velocity of the particle.
//	// axisOfRotation - Mesh particles rotate around this axis.
//	// position - The position of the particle.
//	// randomSeed - The random seed of the particle.
//	// remainingLifetime - The remaining lifetime of the particle.
//	// rotation - The rotation of the particle.
//	// rotation3D - The 3D rotation of the particle.
//	// startColor - The initial color of the particle. The current color of the particle is calculated procedurally based on this value and the active color modules.
//	// startLifetime - The starting lifetime of the particle.
//	// startSize - The initial size of the particle. The current size of the particle is calculated procedurally based on this value and the active size modules.
//	// startSize3D - The initial 3D size of the particle. The current size of the particle is calculated procedurally based on this value and the active size modules.
//	// totalVelocity - The total velocity of the particle.
//	// velocity - The velocity of the particle, measured in units per second.
//
//	// GetCurrentColor - Calculate the current color of the particle by applying the relevant curves to its startColor property.
//	// GetCurrentSize - Calculate the current size of the particle by applying the relevant curves to its startSize property.
//	// GetCurrentSize3D - Calculate the current 3D size of the particle by applying the relevant curves to its startSize3D property.
//	// GetMeshIndex - Calculate the Mesh index of the particle, used for choosing which Mesh a particle is rendered with.
//	// SetMeshIndex - Sets the Mesh index of the particle, used for choosing which Mesh a particle is rendered with.
// };
//
// struct ParticleSystem {
//	// collision - Script interface for the CollisionModule of a Particle System.
//	// colorBySpeed - Script interface for the ColorByLifetimeModule of a Particle System.
//	// colorOverLifetime - Script interface for the ColorOverLifetimeModule of a Particle System.
//	// customData - Script interface for the CustomDataModule of a Particle System.
//	// emission - Script interface for the EmissionModule of a Particle System.
//	// externalForces - Script interface for the ExternalForcesModule of a Particle System.
//	// forceOverLifetime - Script interface for the ForceOverLifetimeModule of a Particle System.
//	// inheritVelocity - Script interface for the InheritVelocityModule of a Particle System.
//	// isEmitting - Determines whether the Particle System is emitting particles. A Particle System may stop emitting when its emission module has finished, it has been paused or if the system has been stopped using Stop with the StopEmitting flag. Resume emitting by calling Play.
//	// isPaused - Determines whether the Particle System is paused.
//	// isPlaying - Determines whether the Particle System is playing.
//	// isStopped - Determines whether the Particle System is in the stopped state.
//	// lifetimeByEmitterSpeed - Script interface for the Particle System Lifetime By Emitter Speed module.
//	// lights - Script interface for the LightsModule of a Particle System.
//	// limitVelocityOverLifetime - Script interface for the LimitVelocityOverLifetimeModule of a Particle System. .
//	// main - Access the main Particle System settings.
//	// noise - Script interface for the NoiseModule of a Particle System.
//	// particleCount - The current number of particles (Read Only). The number doesn't include particles of child Particle Systems
//	// proceduralSimulationSupported - Does this system support Procedural Simulation?
//	// randomSeed - Override the random seed used for the Particle System emission.
//	// rotationBySpeed - Script interface for the RotationBySpeedModule of a Particle System.
//	// rotationOverLifetime - Script interface for the RotationOverLifetimeModule of a Particle System.
//	// shape - Script interface for the ShapeModule of a Particle System.
//	// sizeBySpeed - Script interface for the SizeBySpeedModule of a Particle System.
//	// sizeOverLifetime - Script interface for the SizeOverLifetimeModule of a Particle System.
//	// subEmitters - Script interface for the SubEmittersModule of a Particle System.
//	// textureSheetAnimation - Script interface for the TextureSheetAnimationModule of a Particle System.
//	// time - Playback position in seconds.
//	// trails - Script interface for the TrailsModule of a Particle System.
//	// trigger - Script interface for the TriggerModule of a Particle System.
//	// useAutoRandomSeed - Controls whether the Particle System uses an automatically-generated random number to seed the random number generator.
//	// velocityOverLifetime - Script interface for the VelocityOverLifetimeModule of a Particle System.
//
//	// AllocateAxisOfRotationAttribute - Ensures that the axisOfRotations particle attribute array is allocated.
//	// AllocateCustomDataAttribute - Ensures that the customData1 and customData2 particle attribute arrays are allocated.
//	// AllocateMeshIndexAttribute - Ensures that the meshIndices particle attribute array is allocated.
//	// Clear - Remove all particles in the Particle System.
//	// Emit - Emit count particles immediately.
//	// GetCustomParticleData - Get a stream of custom per-particle data.
//	// GetParticles - Gets the particles of this Particle System.
//	// GetPlaybackState - Returns all the data that relates to the current internal state of the Particle System.
//	// GetTrails - Returns all the data relating to the current internal state of the Particle System Trails.
//	// IsAlive - Does the Particle System contain any live particles, or will it produce more?
//	// Pause - Pauses the system so no new particles are emitted and the existing particles are not updated.
//	// Play - Starts the Particle System.
//	// SetCustomParticleData - Set a stream of custom per-particle data.
//	// SetParticles - Sets the particles of this Particle System.
//	// SetPlaybackState - Use this method with the results of an earlier call to ParticleSystem.GetPlaybackState, in order to restore the Particle System to the state stored in the playbackState object.
//	// SetTrails - Use this method with the results of an earlier call to ParticleSystem.GetTrails, in order to restore the Particle System to the state stored in the Trails object.
//	// Simulate - Fast-forwards the Particle System by simulating particles over the given period of time, then pauses it.
//	// Stop - Stops playing the Particle System using the supplied stop behaviour.
//	// TriggerSubEmitter - Triggers the specified sub emitter on all particles of the Particle System.
// };
//
// // ---
//
// struct MainModule {
//	// cullingMode - Configure whether the Particle System will still be simulated each frame, when it is offscreen.
//	// customSimulationSpace - Simulate particles relative to a custom transform component.
//	// duration - The duration of the Particle System in seconds.
//	// emitterVelocity - The current Particle System velocity.
//	// emitterVelocityMode - Control how the Particle System calculates its velocity, when moving in the world.
//	// flipRotation - Makes some particles spin in the opposite direction.
//	// gravityModifier - A scale that this Particle System applies to gravity, defined by Physics.gravity.
//	// gravityModifierMultiplier - Change the gravity multiplier.
//	// loop - Specifies whether the Particle System is looping.
//	// maxParticles - The maximum number of particles to emit.
//	// playOnAwake - If set to true, the Particle System automatically begins to play on startup.
//	// prewarm - If ParticleSystem.MainModule.loop is true, when you enable this property, the Particle System looks like it has already simulated for one loop when first becoming visible.
//	// ringBufferLoopRange - When ParticleSystem.MainModule.ringBufferMode is set to loop, this value defines the proportion of the particle life that loops.
//	// ringBufferMode - Configure the Particle System to not kill its particles when their lifetimes are exceeded.
//	// scalingMode - Control how the Particle System applies its Transform component to the particles it emits.
//	// simulationSpace - This selects the space in which to simulate particles. It can be either world or local space.
//	// simulationSpeed - Override the default playback speed of the Particle System.
//	// startColor - The initial color of particles when the Particle System first spawns them.
//	// startDelay - Start delay in seconds.
//	// startDelayMultiplier - A multiplier for ParticleSystem.MainModule.startDelay in seconds.
//	// startLifetime - The total lifetime in seconds that each new particle has.
//	// startLifetimeMultiplier - A multiplier for ParticleSystem.MainModule.startLifetime.
//	// startRotation - The initial rotation of particles when the Particle System first spawns them.
//	// startRotation3D - A flag to enable 3D particle rotation.
//	// startRotationMultiplier - A multiplier for ParticleSystem.MainModule.startRotation.
//	// startRotationX - The initial rotation of particles around the x-axis when emitted.
//	// startRotationXMultiplier - The initial rotation multiplier of particles around the x-axis when the Particle System first spawns them.
//	// startRotationY - The initial rotation of particles around the y-axis when the Particle System first spawns them.
//	// startRotationYMultiplier - The initial rotation multiplier of particles around the y-axis when the Particle System first spawns them..
//	// startRotationZ - The initial rotation of particles around the z-axis when the Particle System first spawns them
//	// startRotationZMultiplier - The initial rotation multiplier of particles around the z-axis when the Particle System first spawns them.
//	// startSize - The initial size of particles when the Particle System first spawns them.
//	// startSize3D - A flag to enable specifying particle size individually for each axis.
//	// startSizeMultiplier - A multiplier for the initial size of particles when the Particle System first spawns them.
//	// startSizeX - The initial size of particles along the x-axis when the Particle System first spawns them.
//	// startSizeXMultiplier - A multiplier for ParticleSystem.MainModule.startSizeX.
//	// startSizeY - The initial size of particles along the y-axis when the Particle System first spawns them.
//	// startSizeYMultiplier - A multiplier for ParticleSystem.MainModule.startSizeY.
//	// startSizeZ - The initial size of particles along the z-axis when the Particle System first spawns them.
//	// startSizeZMultiplier - A multiplier for ParticleSystem.MainModule.startSizeZ.
//	// startSpeed - The initial speed of particles when the Particle System first spawns them.
//	// startSpeedMultiplier - A multiplier for ParticleSystem.MainModule.startSpeed.
//	// stopAction - Select whether to Disable or Destroy the GameObject, or to call the MonoBehaviour.OnParticleSystemStopped script Callback, when the Particle System stops and all particles have died.
//	// useUnscaledTime - When true, use the unscaled delta time to simulate the Particle System. Otherwise, use the scaled delta time.
// };
//
// struct CollisionModule {
//	// bounce - How much force is applied to each particle after a collision.
//	// bounceMultiplier - A multiplier for ParticleSystem.CollisionModule.bounce.
//	// colliderForce - How much force is applied to a Collider when hit by particles from this Particle System.
//	// collidesWith - Control which Layers this Particle System collides with.
//	// dampen - How much speed does each particle lose after a collision.
//	// dampenMultiplier - Change the dampen multiplier.
//	// enabled - Specifies whether the CollisionModule is enabled or disabled.
//	// enableDynamicColliders - Allow particles to collide with dynamic colliders when using world collision mode.
//	// lifetimeLoss - How much a collision reduces a particle's lifetime.
//	// lifetimeLossMultiplier - Change the lifetime loss multiplier.
//	// maxCollisionShapes - The maximum number of collision shapes Unity considers for particle collisions. It ignores excess shapes. Terrains take priority.
//	// maxKillSpeed - Kill particles whose speed goes above this threshold, after a collision.
//	// minKillSpeed - Kill particles whose speed falls below this threshold, after a collision.
//	// mode - Choose between 2D and 3D world collisions.
//	// multiplyColliderForceByCollisionAngle - Specifies whether the physics system considers the collision angle when it applies forces from particles to Colliders.
//	// multiplyColliderForceByParticleSize - Specifies whether the physics system considers particle sizes when it applies forces to Colliders.
//	// multiplyColliderForceByParticleSpeed - Specifies whether the physics system considers particle speeds when it applies forces to Colliders.
//	// planeCount - Shows the number of planes currently set as Colliders.
//	// quality - Specifies the accuracy of particle collisions against colliders in the Scene.
//	// radiusScale - A multiplier that Unity applies to the size of each particle before collisions are processed.
//	// sendCollisionMessages - Send collision callback messages.
//	// type - The type of particle collision to perform.
//	// voxelSize - Size of voxels in the collision cache.
//
//	// AddPlane - Adds a collision plane to use with this Particle System.
//	// GetPlane - Get a collision plane associated with this Particle System.
//	// RemovePlane - Removes a collision plane associated with this Particle System.
//	// SetPlane - Set a collision plane to use with this Particle System.
// };
//
// struct ColorBySpeedModule {
//	// color - The gradient that controls the particle colors.
//	// enabled - Specifies whether the ColorBySpeedModule is enabled or disabled.
//	// range - Apply the color gradient between these minimum and maximum speeds.
// };
//
// struct ColorOverLifetimeModule {
//	// color - The gradient that controls the particle colors.
//	// enabled - Specifies whether the ColorOverLifetimeModule is enabled or disabled.
// };
//
// struct EmissionModule {
//	// burstCount - The current number of bursts.
//	// enabled - Specifies whether the EmissionModule is enabled or disabled.
//	// rateOverDistance - The rate at which the emitter spawns new particles over distance.
//	// rateOverDistanceMultiplier - Change the rate over distance multiplier.
//	// rateOverTime - The rate at which the emitter spawns new particles over time.
//	// rateOverTimeMultiplier - Change the rate over time multiplier.
//
//	// GetBurst - Gets a single burst from the array of bursts.
//	// GetBursts - Gets the burst array.
//	// SetBurst - Sets a single burst in the array of bursts.
//	// SetBursts - Sets the burst array.
//
//	struct EmitParams {
//	 - // angularVelocity - Override the angular velocity of particles this system emits.
//	 - // angularVelocity3D - Override the 3D angular velocity of particles this system emits.
//	 - // applyShapeToPosition - When overriding the position of particles, setting this flag to true allows you to retain the influence of the shape module.
//	 - // axisOfRotation - Override the axis of rotation of particles this system emits.
//	 - // meshIndex - Set the index that specifies which Mesh to emit.
//	 - // particle - Override all the properties of particles this system emits.
//	 - // position - Override the position of particles this system emits.
//	 - // randomSeed - Override the random seed of particles this system emits.
//	 - // rotation - Override the rotation of particles this system emits.
//	 - // rotation3D - Override the 3D rotation of particles this system emits.
//	 - // startColor - Override the initial color of particles this system emits.
//	 - // startLifetime - Override the lifetime of particles this system emits.
//	 - // startSize - Override the initial size of particles this system emits.
//	 - // startSize3D - Override the initial 3D size of particles this system emits.
//	 - // velocity - Override the velocity of particles this system emits.
//
//	 - // GetBurst - Gets a single burst from the array of bursts.
//	 - // GetBursts - Gets the burst array.
//	 - // SetBurst - Sets a single burst in the array of bursts.
//	 - // SetBursts - Sets the burst array.
//	};
//
//	struct Burst {
//	 - // count - Specify the number of particles to emit.
//	 - // cycleCount - Specifies how many times the system should play the burst. Set this to 0 to make it play indefinitely.
//	 - // maxCount - The maximum number of particles to emit.
//	 - // minCount - The minimum number of particles to emit.
//	 - // probability - The probability that the system triggers a burst.
//	 - // repeatInterval - How often to repeat the burst, in seconds.
//	 - // time - The time that each burst occurs.
//	};
// };
//
// struct ExternalForcesModule {
//	// enabled - Specifies whether the ExternalForcesModule is enabled or disabled.
//	// influenceCount - The number of Force Fields explicitly provided to the influencers list.
//	// influenceFilter - Apply all Force Fields belonging to a matching Layer to this Particle System.
//	// influenceMask - Particle System Force Field Components with a matching Layer affect this Particle System.
//	// multiplier - Multiplies the magnitude of external forces affecting the particles.
//	// multiplierCurve - Multiplies the magnitude of applied external forces.
//
//	// AddInfluence - Adds a ParticleSystemForceField to the influencers list.
//	// GetInfluence - Gets the ParticleSystemForceField at the given index in the influencers list.
//	// IsAffectedBy - Determines whether any particles are inside the influence of a Force Field.
//	// RemoveAllInfluences - Removes every Force Field from the influencers list.
//	// RemoveInfluence - Removes the Force Field from the influencers list at the given index.
//	// SetInfluence - Assigns the Force Field at the given index in the influencers list.
// };
//
// struct ForceOverLifetimeModule {
//	// enabled - Specifies whether the ForceOverLifetimeModule is enabled or disabled.
//	// randomized - When randomly selecting values between two curves or constants, this flag causes the system to choose a new random force on each frame.
//	// space - Specifies whether the modules applies the forces in local or world space.
//	// x - The curve that defines particle forces in the x-axis.
//	// xMultiplier - Defines the x-axis multiplier.
//	// y - The curve defining particle forces in the y-axis.
//	// yMultiplier - Defines the y-axis multiplier.
//	// z - The curve defining particle forces in the z-axis.
//	// zMultiplier - Defines the z-axis multiplier.
// };
//
// struct InheritVelocityModule {
//	// curve - Curve to define how much of the emitter velocity the system applies during the lifetime of a particle.
//	// curveMultiplier - Change the curve multiplier.
//	// enabled - Specifies whether the InheritVelocityModule is enabled or disabled.
//	// mode - Specifies how to apply emitter velocity to particles.
// };
//
// struct LifetimeByEmitterSpeedModule {
//	// curve - Use this curve to define which value to multiply the start lifetime of a particle with, based on the speed of the emitter when the particle is spawned.
//	// curveMultiplier - Use this property to change the curve multiplier.
//	// enabled - Use this property to enable or disable the LifetimeByEmitterSpeed module.
//	// range - Control the start lifetime multiplier between these minimum and maximum speeds of the emitter.
// };
//
// struct LightsModule {
//	// alphaAffectsIntensity - Toggle whether the system multiplies the particle alpha by the light intensity when it computes the final light intensity.
//	// enabled - Specifies whether the LightsModule is enabled or disabled.
//	// intensity - Define a curve to apply custom intensity scaling to particle Lights.
//	// intensityMultiplier - Intensity multiplier.
//	// light - Select what Light Prefab you want to base your particle lights on.
//	// maxLights - Set a limit on how many Lights this Module can create.
//	// range - Define a curve to apply custom range scaling to particle Lights.
//	// rangeMultiplier - Range multiplier.
//	// ratio - Choose what proportion of particles receive a dynamic light.
//	// sizeAffectsRange - Toggle whether the system multiplies the particle size by the light range to determine the final light range.
//	// useParticleColor - Toggle whether the particle lights multiply their color by the particle color.
//	// useRandomDistribution - Randomly assign Lights to new particles based on ParticleSystem.LightsModule.ratio.
// };
//
// struct LimitVelocityOverLifetimeModule {
//	// dampen - Controls how much this module dampens particle velocities that exceed the velocity limit.
//	// drag - Controls the amount of drag that this modules applies to the particle velocities.
//	// dragMultiplier - Specifies the drag multiplier.
//	// enabled - Specifies whether the LimitForceOverLifetimeModule is enabled or disabled.
//	// limit - Maximum velocity curve, when not using one curve per axis.
//	// limitMultiplier - Change the limit multiplier.
//	// limitX - Maximum velocity curve for the x-axis.
//	// limitXMultiplier - Change the limit multiplier on the x-axis.
//	// limitY - Maximum velocity curve for the y-axis.
//	// limitYMultiplier - Change the limit multiplier on the y-axis.
//	// limitZ - Maximum velocity curve for the z-axis.
//	// limitZMultiplier - Change the limit multiplier on the z-axis.
//	// multiplyDragByParticleSize - Adjust the amount of drag this module applies to particles, based on their sizes.
//	// multiplyDragByParticleVelocity - Adjust the amount of drag this module applies to particles, based on their speeds.
//	// separateAxes - Set the velocity limit on each axis separately. This module uses ParticleSystem.LimitVelocityOverLifetimeModule.drag to dampen a particle's velocity if the velocity exceeds this value.
//	// space - Specifies if the velocity limits are in local space (rotated with the transform) or world space.
// };
//
// struct NoiseModule {
//	// damping - Higher frequency noise reduces the strength by a proportional amount, if enabled.
//	// enabled - Specifies whether the the NoiseModule is enabled or disabled.
//	// frequency - Low values create soft, smooth noise, and high values create rapidly changing noise.
//	// octaveCount - Layers of noise that combine to produce final noise.
//	// octaveMultiplier - When combining each octave, scale the intensity by this amount.
//	// octaveScale - When combining each octave, zoom in by this amount.
//	// positionAmount - How much the noise affects the particle positions.
//	// quality - Generate 1D, 2D or 3D noise.
//	// remap - Define how the noise values are remapped.
//	// remapEnabled - Enable remapping of the final noise values, allowing for noise values to be translated into different values.
//	// remapMultiplier - Remap multiplier.
//	// remapX - Define how the noise values are remapped on the x-axis, when using the ParticleSystem.NoiseModule.separateAxes option.
//	// remapXMultiplier - x-axis remap multiplier.
//	// remapY - Define how the noise values are remapped on the y-axis, when using the ParticleSystem.NoiseModule.separateAxes option.
//	// remapYMultiplier - y-axis remap multiplier.
//	// remapZ - Define how the noise values are remapped on the z-axis, when using the ParticleSystem.NoiseModule.separateAxes option.
//	// remapZMultiplier - z-axis remap multiplier.
//	// rotationAmount - How much the noise affects the particle rotation, in degrees per second.
//	// scrollSpeed - Scroll the noise map over the Particle System.
//	// scrollSpeedMultiplier - Scroll speed multiplier.
//	// separateAxes - Control the noise separately for each axis.
//	// sizeAmount - How much the noise affects the particle sizes, applied as a multiplier on the size of each particle.
//	// strength - How strong the overall noise effect is.
//	// strengthMultiplier - Strength multiplier.
//	// strengthX - Define the strength of the effect on the x-axis, when using the ParticleSystem.NoiseModule.separateAxes option.
//	// strengthXMultiplier - x-axis strength multiplier.
//	// strengthY - Define the strength of the effect on the y-axis, when using the ParticleSystem.NoiseModule.separateAxes option.
//	// strengthYMultiplier - y-axis strength multiplier.
//	// strengthZ - Define the strength of the effect on the z-axis, when using the ParticleSystem.NoiseModule.separateAxes option.
//	// strengthZMultiplier - z-axis strength multiplier.
// };
//
// struct RotationBySpeedModule {
//	// enabled - ESpecifies whether the RotationBySpeedModule is enabled or disabled.
//	// range - Set the minimum and maximum speeds that this module applies the rotation curve between.
//	// separateAxes - Set the rotation by speed on each axis separately.
//	// x - Rotation by speed curve for the x-axis.
//	// xMultiplier - Speed multiplier along the x-axis.
//	// y - Rotation by speed curve for the y-axis.
//	// yMultiplier - Speed multiplier along the y-axis.
//	// z - Rotation by speed curve for the z-axis.
//	// zMultiplier - Speed multiplier along the z-axis.
// };
//
// struct RotationOverLifetimeModule {
//	// enabled - Specifies whether the RotationOverLifetimeModule is enabled or disabled.
//	// separateAxes - Set the rotation over lifetime on each axis separately.
//	// x - Rotation over lifetime curve for the x-axis.
//	// xMultiplier - Rotation multiplier around the x-axis.
//	// y - Rotation over lifetime curve for the y-axis.
//	// yMultiplier - Rotation multiplier around the y-axis.
//	// z - Rotation over lifetime curve for the z-axis.
//	// zMultiplier - Rotation multiplier around the z-axis.
// };
//
// struct ShapeModule {
//	// alignToDirection - Align particles based on their initial direction of travel.
//	// angle - Angle of the cone to emit particles from.
//	// arc - Angle of the circle arc to emit particles from.
//	// arcMode - The mode that Unity uses to generate particles around the arc.
//	// arcSpeed - In animated modes, this determines how quickly the particle emission position moves around the arc.
//	// arcSpeedMultiplier - A multiplier of the arc speed of the particle emission shape.
//	// arcSpread - Control the gap between particle emission points around the arc.
//	// boxThickness - Thickness of the box to emit particles from.
//	// donutRadius - The thickness of the Donut shape to emit particles from.
//	// enabled - Specifies whether the ShapeModule is enabled or disabled.
//	// length - Length of the cone to emit particles from.
//	// mesh - Mesh to emit particles from.
//	// meshMaterialIndex - Emit particles from a single Material of a Mesh.
//	// meshRenderer - MeshRenderer to emit particles from.
//	// meshShapeType - Where on the Mesh to emit particles from.
//	// meshSpawnMode - The mode to use to generate particles on a Mesh.
//	// meshSpawnSpeed - In animated modes, this determines how quickly the particle emission position moves across the Mesh.
//	// meshSpawnSpeedMultiplier - A multiplier of the Mesh spawn speed.
//	// meshSpawnSpread - Control the gap between particle emission points across the Mesh.
//	// normalOffset - Move particles away from the surface of the source Mesh.
//	// position - Apply an offset to the position from which the system emits particles.
//	// radius - Radius of the shape to emit particles from.
//	// radiusMode - The mode to use to generate particles along the radius.
//	// radiusSpeed - In animated modes, this determines how quickly the particle emission position moves along the radius.
//	// radiusSpeedMultiplier - A multiplier of the radius speed of the particle emission shape.
//	// radiusSpread - Control the gap between particle emission points along the radius.
//	// radiusThickness - Radius thickness of the shape's edge from which to emit particles.
//	// randomDirectionAmount - Randomizes the starting direction of particles.
//	// randomPositionAmount - Randomizes the starting position of particles.
//	// rotation - Apply a rotation to the shape from which the system emits particles.
//	// scale - Apply scale to the shape from which the system emits particles.
//	// shapeType - The type of shape to emit particles from.
//	// skinnedMeshRenderer - SkinnedMeshRenderer to emit particles from.
//	// sphericalDirectionAmount - Makes particles move in a spherical direction from their starting point.
//	// sprite - Sprite to emit particles from.
//	// spriteRenderer - SpriteRenderer to emit particles from.
//	// texture - Specifies a Texture to tint the particle's start colors.
//	// textureAlphaAffectsParticles - When enabled, the system applies the alpha channel of the Texture to the particle alpha when the particle spawns.
//	// textureBilinearFiltering - When enabled, the system takes four neighboring samples from the Texture then combines them to give the final particle value.
//	// textureClipChannel - Selects which channel of the Texture to use for discarding particles.
//	// textureClipThreshold - Discards particles when they spawn on an area of the Texture with a value lower than this threshold.
//	// textureColorAffectsParticles - When enabled, the system applies the RGB channels of the Texture to the particle color when the particle spawns.
//	// textureUVChannel - When using a Mesh as a source shape type, this option controls which UV channel on the Mesh to use for reading the source Texture.
//	// useMeshColors - Modulate the particle colors with the vertex colors, or the Material color if no vertex colors exist.
//	// useMeshMaterialIndex - Emit particles from a single Material, or the whole Mesh.
// };
//
// struct SizeBySpeedModule {
//	// enabled - Specifies whether the SizeBySpeedModule is enabled or disabled.
//	// range - Set the minimum and maximum speed that this modules applies the size curve between.
//	// separateAxes - Set the size by speed on each axis separately.
//	// size - Curve to control particle size based on speed.
//	// sizeMultiplier - A multiplier for ParticleSystem.SizeBySpeedModule.size.
//	// x - Size by speed curve for the x-axis.
//	// xMultiplier - Size multiplier along the x-axis.
//	// y - Size by speed curve for the y-axis.
//	// yMultiplier - Size multiplier along the y-axis.
//	// z - Size by speed curve for the z-axis.
//	// zMultiplier - Size multiplier along the z-axis.
// };
//
// struct SizeOverLifetimeModule {
//	// enabled - Specifies whether the SizeOverLifetimeModule is enabled or disabled.
//	// separateAxes - Set the size over lifetime on each axis separately.
//	// size - Curve to control particle size based on lifetime.
//	// sizeMultiplier - A multiplier for ParticleSystem.SizeOverLifetimeModule.size.
//	// x - Size over lifetime curve for the x-axis.
//	// xMultiplier - Size multiplier along the x-axis.
//	// y - Size over lifetime curve for the y-axis.
//	// yMultiplier - Size multiplier along the y-axis.
//	// z - Size over lifetime curve for the z-axis.
//	// zMultiplier - Size multiplier along the z-axis.
// };
//
// struct SubEmittersModule {
//	// enabled - Specifies whether the SubEmittersModule is enabled or disabled.
//	// subEmittersCount - The total number of sub-emitters.
//
//	// AddSubEmitter - Add a new sub-emitter.
//	// GetSubEmitterEmitProbability - Gets the probability that the sub-emitter emits particles.
//	// GetSubEmitterProperties - Gets the properties of the sub-emitter at the given index.
//	// GetSubEmitterSystem - Gets the sub-emitter Particle System at the given index.
//	// GetSubEmitterType - Gets the type of the sub-emitter at the given index.
//	// RemoveSubEmitter - Removes a sub-emitter from the given index in the array.
//	// SetSubEmitterEmitProbability - Sets the probability that the sub-emitter emits particles.
//	// SetSubEmitterProperties - Sets the properties of the sub-emitter at the given index.
//	// SetSubEmitterSystem - Sets the Particle System to use as the sub-emitter at the given index.
//	// SetSubEmitterType - Sets the type of the sub-emitter at the given index.
// };
//
// struct TextureSheetAnimationModule {
//	// animation - Specifies the animation type.
//	// cycleCount - Specifies how many times the animation loops during the lifetime of the particle.
//	// enabled - Specifies whether the TextureSheetAnimationModule is enabled or disabled.
//	// fps - Control how quickly the animation plays.
//	// frameOverTime - A curve to control which frame of the Texture sheet animation to play.
//	// frameOverTimeMultiplier - The frame over time mutiplier.
//	// mode - Select whether the animated Texture information comes from a grid of frames on a single Texture, or from a list of Sprite objects.
//	// numTilesX - Defines the tiling of the Texture in the x-axis.
//	// numTilesY - Defines the tiling of the texture in the y-axis.
//	// rowIndex - Explicitly select which row of the Texture sheet to use. The system uses this property when ParticleSystem.TextureSheetAnimationModule.rowMode is set to Custom.
//	// rowMode - Select how particles choose which row of a Texture Sheet Animation to use.
//	// speedRange - Specify how particle speeds are mapped to the animation frames.
//	// spriteCount - The total number of sprites.
//	// startFrame - Define a random starting frame for the Texture sheet animation.
//	// startFrameMultiplier - The starting frame multiplier.
//	// timeMode - Select whether the system bases the playback on mapping a curve to the lifetime of each particle, by using the particle speeds, or if playback simply uses a constant frames per second.
//	// uvChannelMask - Choose which UV channels receive Texture animation.
//
//	// AddSprite - Add a new Sprite.
//	// GetSprite - Get the Sprite at the given index.
//	// RemoveSprite - Remove a Sprite from the given index in the array.
//	// SetSprite - Set the Sprite at the given index.
// };
//
// struct TrailModule {
//	// attachRibbonsToTransform - Adds an extra position to each ribbon, connecting it to the location of the Transform Component.
//	// colorOverLifetime - The gradient that controls the trail colors during the lifetime of the attached particle.
//	// colorOverTrail - The gradient that controls the trail colors over the length of the trail.
//	// dieWithParticles - Specifies whether trails disappear immediately when their owning particle dies. When false, each trail persists until all its points have naturally expired, based on its lifetime.
//	// enabled - Specifies whether the TrailModule is enabled or disabled.
//	// generateLightingData - Configures the trails to generate Normals and Tangents. With this data, Scene lighting can affect the trails via Normal Maps and the Unity Standard Shader, or your own custom-built Shaders.
//	// inheritParticleColor - Toggle whether the trail inherits the particle color as its starting color.
//	// lifetime - The curve describing the trail lifetime, throughout the lifetime of the particle.
//	// lifetimeMultiplier - A multiplier for ParticleSystem.TrailModule.lifetime.
//	// minVertexDistance - Set the minimum distance each trail can travel before the system adds a new vertex to it.
//	// mode - Choose how the system generates the particle trails.
//	// ratio - Choose what proportion of particles receive a trail.
//	// ribbonCount - Select how many lines to create through the Particle System.
//	// shadowBias - Apply a shadow bias to prevent self-shadowing artifacts. The specified value is the proportion of the trail width at each segment.
//	// sizeAffectsLifetime - Set whether the particle size acts as a multiplier on top of the trail lifetime.
//	// sizeAffectsWidth - Set whether the particle size acts as a multiplier on top of the trail width.
//	// splitSubEmitterRibbons - Specifies whether, if you use this system as a sub-emitter, ribbons connect particles from each parent particle independently.
//	// textureMode - Choose whether the U coordinate of the trail Texture is tiled or stretched.
//	// widthOverTrail - The curve describing the width of each trail point.
//	// widthOverTrailMultiplier - A multiplier for ParticleSystem.TrailModule.widthOverTrail.
//	// worldSpace - Drop new trail points in world space, regardless of Particle System Simulation Space.
//
//	struct Trails {
//	 - // capacity - Reserve memory for the particle trail data.
//	};
// };
//
// struct TriggerModule {
//	// colliderCount - Indicates the number of collision shapes attached to this Particle System trigger.
//	// colliderQueryMode - Determines whether collider information is available when calling [[ParticleSystem::GetTriggerParticles]].
//	// enabled - Specifies whether the TriggerModule is enabled or disabled.
//	// enter - Choose what action to perform when particles enter the trigger volume.
//	// exit - Choose what action to perform when particles leave the trigger volume.
//	// inside - Choose what action to perform when particles are inside the trigger volume.
//	// outside - Choose what action to perform when particles are outside the trigger volume.
//	// radiusScale - A multiplier Unity applies to the size of each particle before it processes overlaps.
//
//	// AddCollider - Adds a Collision shape associated with this Particle System trigger.
//	// GetCollider - Gets a collision shape associated with this Particle System trigger.
//	// RemoveCollider - Removes a collision shape associated with this Particle System trigger.
//	// SetCollider - Sets a Collision shape associated with this Particle System trigger.
// };
//
// struct VelocityOverLifetimeModule {
//	// enabled - Specifies whether the VelocityOverLifetimeModule is enabled or disabled.
//	// orbitalOffsetX - Specify a custom center of rotation for the orbital and radial velocities.
//	// orbitalOffsetXMultiplier - A multiplier for _orbitalOffsetX.
//	// orbitalOffsetY - Specify a custom center of rotation for the orbital and radial velocities.
//	// orbitalOffsetYMultiplier - A multiplier for _orbitalOffsetY.
//	// orbitalOffsetZ - Specify a custom center of rotation for the orbital and radial velocities.
//	// orbitalOffsetZMultiplier - A multiplier for _orbitalOffsetY.
//	// orbitalX - Curve to control particle speed based on lifetime, around the x-axis.
//	// orbitalXMultiplier - Speed multiplier along the x-axis.
//	// orbitalY - Curve to control particle speed based on lifetime, around the y-axis.
//	// orbitalYMultiplier - Speed multiplier along the y-axis.
//	// orbitalZ - Curve to control particle speed based on lifetime, around the z-axis.
//	// orbitalZMultiplier - Speed multiplier along the z-axis.
//	// radial - Curve to control particle speed based on lifetime, away from a center position.
//	// radialMultiplier - A multiplier for ParticleSystem.VelocityOverLifetimeModule.radial.
//	// space - Specifies if the velocities are in local space (rotated with the transform) or world space.
//	// speedModifier - Curve to control particle speed based on lifetime, without affecting the direction of the particles.
//	// speedModifierMultiplier - A multiplier for ParticleSystem.VelocityOverLifetimeModule.speedModifier.
//	// x - Curve to control particle speed based on lifetime, on the x-axis.
//	// xMultiplier - A multiplier for ParticleSystem.VelocityOverLifetimeModule.x
//	// y - Curve to control particle speed based on lifetime, on the y-axis.
//	// yMultiplier - A multiplier for ParticleSystem.VelocityOverLifetimeModule.y.
//	// z - Curve to control particle speed based on lifetime, on the z-axis.
//	// zMultiplier - A multiplier for ParticleSystem.VelocityOverLifetimeModule.z.
// };
//
// // ---
//
// struct ParticleSystemForceField {
//	// directionX - Apply a linear force along the local X axis to particles within the volume of the Force Field.
//	// directionY - Apply a linear force along the local Y axis to particles within the volume of the Force Field.
//	// directionZ - Apply a linear force along the local Z axis to particles within the volume of the Force Field.
//	// drag - Apply drag to particles within the volume of the Force Field.
//	// endRange - Determines the size of the shape used for influencing particles.
//	// gravity - Apply gravity to particles within the volume of the Force Field.
//	// gravityFocus - When using the gravity force, set this value between 0 and 1 to control the focal point of the gravity effect.
//	// length - Describes the length of the Cylinder when using the Cylinder Force Field shape to influence particles.
//	// multiplyDragByParticleSize - When using Drag, the drag strength will be multiplied by the size of the particles if this toggle is enabled.
//	// multiplyDragByParticleVelocity - When using Drag, the drag strength will be multiplied by the speed of the particles if this toggle is enabled.
//	// rotationAttraction - Controls how strongly particles are dragged into the vortex motion.
//	// rotationRandomness - Apply randomness to the Force Field axis that particles will travel around.
//	// rotationSpeed - The speed at which particles are propelled around a vortex.
//	// shape - Selects the type of shape used for influencing particles.
//	// startRange - Setting a value greater than 0 creates a hollow Force Field shape. This will cause particles to not be affected by the Force Field when closer to the center of the volume than the startRange property.
//	// vectorField - Apply forces to particles within the volume of the Force Field, by using a 3D texture containing vector field data.
//	// vectorFieldAttraction - Controls how strongly particles are dragged into the vector field motion.
//	// vectorFieldSpeed - The speed at which particles are propelled through the vector field.
// };
//
// // ---
//
// struct ParticleSystemRenderer {
//	// activeVertexStreamsCount - The number of currently active custom vertex streams.
//	// alignment - Control the direction that particles face.
//	// allowRoll - Allow billboard particles to roll around their z-axis.
//	// cameraVelocityScale - How much do the particles stretch depending on the Camera's speed.
//	// enableGPUInstancing - Enables GPU Instancing on platforms that support it.
//	// flip - Flip a percentage of the particles, along each axis.
//	// freeformStretching - Enables freeform stretching behavior.
//	// lengthScale - How much are the particles stretched in their direction of motion, defined as the length of the particle compared to its width.
//	// maskInteraction - Specifies how the Particle System Renderer interacts with SpriteMask.
//	// maxParticleSize - Clamp the maximum particle size.
//	// mesh - The Mesh that the particle uses instead of a billboarded Texture.
//	// meshCount - The number of Meshes the system uses for particle rendering.
//	// meshDistribution - Specifies how the system randomly assigns meshes to particles.
//	// minParticleSize - Clamp the minimum particle size.
//	// normalDirection - Specifies how much a billboard particle orients its normals towards the Camera.
//	// pivot - Modify the pivot point used for rotating particles.
//	// renderMode - Specifies how the system draws particles.
//	// rotateWithStretchDirection - Rotate the particles based on the direction they are stretched in. This is added on top of other particle rotation.
//	// shadowBias - Apply a shadow bias to prevent self-shadowing artifacts. The specified value is the proportion of the particle size.
//	// sortingFudge - Biases Particle System sorting amongst other transparencies.
//	// sortMode - Specifies how to sort particles within a system.
//	// supportsMeshInstancing - Determines whether the Particle System can be rendered using GPU Instancing.
//	// trailMaterial - Set the Material that the TrailModule uses to attach trails to particles.
//	// velocityScale - Specifies how much particles stretch depending on their velocity.
//
//	// BakeMesh - Creates a snapshot of ParticleSystemRenderer and stores it in mesh.
//	// BakeTrailsMesh - Creates a snapshot of ParticleSystem Trails and stores them in mesh.
//	// GetActiveVertexStreams - Queries which Vertex Shader streams are enabled on the ParticleSystemRenderer.
//	// GetMeshes - Gets the array of Meshes to use when selecting particle meshes.
//	// GetMeshWeightings - Gets the array of Mesh weightings to use when randomly selecting particle meshes.
//	// SetActiveVertexStreams - Enables a set of vertex Shader streams on the ParticleSystemRenderer.
//	// SetMeshes - Sets the Meshes that the ParticleSystemRenderer uses to display particles when the ParticleSystemRenderer.renderMode is set to ParticleSystemRenderMode.Mesh.
//	// SetMeshWeightings - Sets the weights that the ParticleSystemRenderer uses to assign Meshes to particles.
// };
