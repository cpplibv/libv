// Project: libv, File: sandbox/libv_physics_main.cpp, Author: Császár Mátyás [Vader]

// ext
#include <rp3d/body/RigidBody.h>
#include <rp3d/collision/Collider.h>
#include <rp3d/collision/CollisionCallback.h>
#include <rp3d/collision/ContactManifold.h>
#include <rp3d/collision/shapes/BoxShape.h>
#include <rp3d/collision/shapes/SphereShape.h>
#include <rp3d/constraint/ContactPoint.h>
#include <rp3d/engine/EventListener.h>
#include <rp3d/engine/Material.h>
#include <rp3d/engine/PhysicsWorld.h>
#include <rp3d/mathematics/Quaternion.h>
#include <rp3d/mathematics/Transform.h>
#include <rp3d/mathematics/Vector3.h>
#include <rp3d/rp3d.h>
#include <rp3d/systems/DynamicsSystem.h>
// libv
#include <libv/math/angle.hpp>
#include <libv/math/constants.hpp>
#include <libv/math/quaternion.hpp>
#include <libv/math/vec.hpp>
#include <libv/utility/bit_cast.hpp>
// std
#include <iostream>


// -------------------------------------------------------------------------------------------------

enum Category {
	CATEGORY1 = 0x0001,
	CATEGORY2 = 0x0002,
	CATEGORY3 = 0x0004,
};

struct EventListener : rp3d::EventListener {
	virtual void onContact(const rp3d::CollisionCallback::CallbackData& info) override {
		std::cout << "--- onContact ---\n";

		// For each contact pair
		for (uint32_t p = 0; p < info.getNbContactPairs(); p++) {
			CollisionCallback::ContactPair contact = info.getContactPair(p);

			std::cout << '\n';
			std::cout << "\tNew contact:"
					<< " body1: " << libv::bit_cast<size_t>(contact.getBody1())
					<< " body2: " << libv::bit_cast<size_t>(contact.getBody2())
					<< " collider1: " << libv::bit_cast<size_t>(contact.getCollider1())
					<< " collider2: " << libv::bit_cast<size_t>(contact.getCollider2())
					<< '\n';

			std::cout << "\t\tcontact count     : " << contact.getNbContactPoints() << '\n';

			// For each contact point of the contact pair
			for (uint32_t c = 0; c < contact.getNbContactPoints(); c++) {
				// Get the contact point
				CollisionCallback::ContactPoint contactPoint = contact.getContactPoint(c);

				// Get the contact point on the first collider and convert it in world-space
				rp3d::Vector3 worldPoint = contact.getCollider1()->getLocalToWorldTransform() * contactPoint.getLocalPointOnCollider1();

				std::cout << '\n';
				std::cout << "\t\tworldPoint        : " << libv::vec3f(worldPoint) << '\n';
				std::cout << "\t\tworldNormal       : " << libv::vec3f(contactPoint.getWorldNormal()) << '\n';
				std::cout << "\t\tlocalPointOnShape1: " << libv::vec3f(contactPoint.getLocalPointOnCollider1()) << '\n';
				std::cout << "\t\tlocalPointOnShape2: " << libv::vec3f(contactPoint.getLocalPointOnCollider2()) << '\n';
//				std::cout << "\t\tpenetrationImpulse: " << point->getPenetrationImpulse() << '\n';
//				std::cout << "\t\tisRestingContact  : " << point->getIsRestingContact() << '\n';
				std::cout << "\t\tpenetrationDepth  : " << contactPoint.getPenetrationDepth() << '\n';
			}
			std::cout << '\n' << std::endl;
		}
	}
};

// https://www.reactphysics3d.com/usermanual.html

int main() {
	// First you need to create the PhysicsCommon object.
	rp3d::PhysicsCommon physicsCommon;

	// Create the world settings
	rp3d::PhysicsWorld::WorldSettings settings;
	settings.defaultPositionSolverNbIterations = 10;
//	settings.defaultSleepAngularVelocity = 3.0f * libv::pi / 180.0f;
//	settings.defaultSleepLinearVelocity = 0.02f;
	settings.defaultVelocitySolverNbIterations = 20;
	settings.isSleepingEnabled = true;
	settings.gravity = rp3d::Vector3(0.0f, 0.0f, -9.81f);

	// Create the physics world with your settings
	rp3d::PhysicsWorld* world = physicsCommon.createPhysicsWorld(settings);

	EventListener listener;
	world->setEventListener(&listener);

	const float mass = 1.0f;
	rp3d::Vector3 position(0.0f, 0.0f, 10.0f);
	rp3d::Quaternion orientation(0, 0, 0, 1);
	rp3d::Transform transform(position, orientation);
	rp3d::BoxShape* box_shape = physicsCommon.createBoxShape({1, 1, 1});
//	rp3d::SphereShape shape(0.5f);
	rp3d::RigidBody* body = world->createRigidBody(transform);
	body->setMass(mass);
	rp3d::Collider* collider = body->addCollider(box_shape, rp3d::Transform::identity());
	collider->setCollisionCategoryBits(CATEGORY1 | CATEGORY2);
	collider->setCollideWithMaskBits(CATEGORY1 | CATEGORY2);
	rp3d::Material& material = collider->getMaterial();
	material.setBounciness(0.4f);
	material.setFrictionCoefficient(0.2f);

	rp3d::Vector3 floor_position(0.0f, 0.0f, 0.0f);
	rp3d::Quaternion floor_orientation(1, 0, 0, 0.1f);
	rp3d::Transform floor_transform(floor_position, floor_orientation);
	rp3d::BoxShape* floor_shape = physicsCommon.createBoxShape({1000, 1000, 1});
	rp3d::RigidBody* floor = world->createRigidBody(floor_transform);
	floor->setType(rp3d::BodyType::STATIC);
	rp3d::Collider* floor_collider = floor->addCollider(floor_shape, rp3d::Transform::identity());
	floor_collider->setCollisionCategoryBits(CATEGORY1 | CATEGORY2);
	floor_collider->setCollideWithMaskBits(CATEGORY1 | CATEGORY2);
	rp3d::Material& floor_material = floor_collider->getMaterial();
	floor_material.setBounciness(0.2f);
	floor_material.setFrictionCoefficient(0.6f);

	const float timeStep = 1.0f / 60.0f;
	for (size_t i = 0; i < 60 * 10; ++i) {
		world->update(timeStep);
		std::cout
				<< "\nobject         : Floor"
				<< "\nsleep          : " << floor->isSleeping();

		std::cout
				<< "\nobject         : Box"
				<< "\nsleep          : " << body->isSleeping()
				<< "\nposition       : " << libv::vec3f(body->getTransform().getPosition())
				<< "\norientation    : " << libv::vec4f(body->getTransform().getOrientation())
				<< "\nangles         : "
				<< libv::rad_to_deg(libv::math::toEulerAngles(body->getTransform().getOrientation()).x) << " "
				<< libv::rad_to_deg(libv::math::toEulerAngles(body->getTransform().getOrientation()).y) << " "
				<< libv::rad_to_deg(libv::math::toEulerAngles(body->getTransform().getOrientation()).z)
				<< "\nlinearVelocity : " << libv::vec3f(body->getLinearVelocity())
				<< "\nangularVelocity: " << libv::vec3f(body->getAngularVelocity()) << std::endl;
	}

	world->destroyRigidBody(body);
	world->destroyRigidBody(floor);

	physicsCommon.destroyPhysicsWorld(world);

	return 0;
}
