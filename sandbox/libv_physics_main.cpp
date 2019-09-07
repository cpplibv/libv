// File: Main.cpp, Created on 2014.04.25. at 21:23, Author: Vader

// ext
#include <reactphysics3d/body/RigidBody.h>
#include <reactphysics3d/collision/ProxyShape.h>
#include <reactphysics3d/collision/shapes/BoxShape.h>
#include <reactphysics3d/collision/shapes/SphereShape.h>
#include <reactphysics3d/engine/DynamicsWorld.h>
#include <reactphysics3d/engine/Material.h>
#include <reactphysics3d/mathematics/Transform.h>
#include <reactphysics3d/mathematics/Vector3.h>
// libv
#include <libv/math/vec.hpp>
// std
#include <iostream>


// -------------------------------------------------------------------------------------------------

enum Category {
	CATEGORY1 = 0x0001,
	CATEGORY2 = 0x0002,
	CATEGORY3 = 0x0004,
};

int main() {
	// https://www.reactphysics3d.com/usermanual.html

	reactphysics3d::WorldSettings settings;
	settings.defaultPositionSolverNbIterations = 10;
	settings.defaultSleepAngularVelocity = 0.001f;
	settings.defaultSleepLinearVelocity = 0.001f;
	settings.defaultVelocitySolverNbIterations = 15;
	settings.isSleepingEnabled = true;

	reactphysics3d::Vector3 gravity(0.0, 0.0, -9.81f);
	reactphysics3d::DynamicsWorld world(gravity, settings);

	const float radius = 0.5f;
	const float mass = 1.0f;
	reactphysics3d::Vector3 position(0.0f, 0.0f, 10.0f);
	reactphysics3d::Quaternion orientation(1, 0, 0, 0);
	reactphysics3d::Transform transform(position, orientation);
	reactphysics3d::SphereShape shape(radius);
	reactphysics3d::RigidBody* body = world.createRigidBody(transform);
	reactphysics3d::ProxyShape* proxyShape = body->addCollisionShape(&shape, reactphysics3d::Transform::identity(), mass);
	proxyShape->setCollisionCategoryBits(CATEGORY1 | CATEGORY2);
	proxyShape->setCollideWithMaskBits(CATEGORY1 | CATEGORY2);
	body->getMaterial().setBounciness(0.4f);
	body->getMaterial().setFrictionCoefficient(0.2f);

	reactphysics3d::Vector3 floor_position(0.0f, 0.0f, 0.0f);
	reactphysics3d::Quaternion floor_orientation(1, 0, 0, 0.1f);
	reactphysics3d::Transform floor_transform(floor_position, floor_orientation);
	reactphysics3d::BoxShape floorShape({100, 100, 1});
	reactphysics3d::RigidBody* floor = world.createRigidBody(floor_transform);
	floor->setType(reactphysics3d::BodyType::STATIC);
	reactphysics3d::ProxyShape* floorProxyShape = floor->addCollisionShape(&floorShape, reactphysics3d::Transform::identity(), 0);
	floorProxyShape->setCollisionCategoryBits(CATEGORY1 | CATEGORY2);
	floorProxyShape->setCollideWithMaskBits(CATEGORY1 | CATEGORY2);
	body->getMaterial().setBounciness(0.4f);
	body->getMaterial().setFrictionCoefficient(0.2f);

	const float timeStep = 1.0f / 60.0f;
	for (size_t i = 0; i < 60 * 5; ++i) {
		world.update(timeStep);
		std::cout << libv::vec3f(floor->getTransform().getPosition()) << " / " << libv::vec3f(body->getTransform().getPosition()) << std::endl;
	}

	world.destroyRigidBody(body);
	world.destroyRigidBody(floor);

	return 0;
}
