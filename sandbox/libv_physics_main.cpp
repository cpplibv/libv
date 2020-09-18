// Project: libv, File: sandbox/libv_physics_main.cpp, Author: Császár Mátyás [Vader]

// ext
#include <rp3d/body/RigidBody.h>
#include <rp3d/collision/ContactManifold.h>
#include <rp3d/collision/ProxyShape.h>
#include <rp3d/collision/shapes/BoxShape.h>
#include <rp3d/collision/shapes/SphereShape.h>
#include <rp3d/constraint/ContactPoint.h>
#include <rp3d/engine/DynamicsWorld.h>
#include <rp3d/engine/EventListener.h>
#include <rp3d/engine/Material.h>
#include <rp3d/mathematics/Quaternion.h>
#include <rp3d/mathematics/Transform.h>
#include <rp3d/mathematics/Vector3.h>
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
	virtual void newContact(const rp3d::CollisionCallback::CollisionCallbackInfo& collisionInfo) override {
		std::cout << '\n';
		std::cout << "\tNew contact:"
				<< " body1: " << libv::bit_cast<size_t>(collisionInfo.body1)
				<< " body2: " << libv::bit_cast<size_t>(collisionInfo.body2)
				<< " shape1: " << libv::bit_cast<size_t>(collisionInfo.proxyShape1)
				<< " shape2: " << libv::bit_cast<size_t>(collisionInfo.proxyShape2)
				<< '\n';

		for (rp3d::ContactManifoldListElement* manifold = collisionInfo.contactManifoldElements; manifold; manifold = manifold->getNext()) {
			const rp3d::ContactManifold* contact = manifold->getContactManifold();

			std::cout << "\t\tcontact count     : " << int(contact->getNbContactPoints()) << '\n';
			for (rp3d::ContactPoint* point = contact->getContactPoints(); point; point = point->getNext()) {
				std::cout << '\n';
				std::cout << "\t\tnormal            : " << libv::vec3f(point->getNormal()) << '\n';
				std::cout << "\t\tlocalPointOnShape1: " << libv::vec3f(point->getLocalPointOnShape1()) << '\n';
				std::cout << "\t\tlocalPointOnShape2: " << libv::vec3f(point->getLocalPointOnShape2()) << '\n';
				std::cout << "\t\tpenetrationImpulse: " << point->getPenetrationImpulse() << '\n';
				std::cout << "\t\tisRestingContact  : " << point->getIsRestingContact() << '\n';
				std::cout << "\t\tpenetrationDepth  : " << point->getPenetrationDepth() << '\n';
			}
		}
		std::cout << '\n' << std::endl;
	}
};

int main() {
	// https://www.rp3d.com/usermanual.html

	rp3d::WorldSettings settings;
	settings.defaultPositionSolverNbIterations = 10;
	settings.defaultSleepAngularVelocity = 3.0f * libv::pi / 180.0f;
	settings.defaultSleepLinearVelocity = 0.02f;
	settings.defaultVelocitySolverNbIterations = 15;
	settings.isSleepingEnabled = true;

	rp3d::Vector3 gravity(0.0, 0.0, -9.81f);
	rp3d::DynamicsWorld world(gravity, settings);
	EventListener listener;
	world.setEventListener(&listener);

	const float mass = 1.0f;
	rp3d::Vector3 position(0.0f, 0.0f, 10.0f);
	rp3d::Quaternion orientation(0, 0, 0, 1);
	rp3d::Transform transform(position, orientation);
	rp3d::BoxShape shape({1, 1, 1});
//	rp3d::SphereShape shape(0.5f);
	rp3d::RigidBody* body = world.createRigidBody(transform);
	rp3d::ProxyShape* proxyShape = body->addCollisionShape(&shape, rp3d::Transform::identity(), mass);
	proxyShape->setCollisionCategoryBits(CATEGORY1 | CATEGORY2);
	proxyShape->setCollideWithMaskBits(CATEGORY1 | CATEGORY2);
	body->getMaterial().setBounciness(0.4f);
	body->getMaterial().setFrictionCoefficient(0.2f);

	rp3d::Vector3 floor_position(0.0f, 0.0f, 0.0f);
	rp3d::Quaternion floor_orientation(1, 0, 0, 0.1f);
	rp3d::Transform floor_transform(floor_position, floor_orientation);
	rp3d::BoxShape floorShape({100, 100, 1});
	rp3d::RigidBody* floor = world.createRigidBody(floor_transform);
	floor->setType(rp3d::BodyType::STATIC);
	rp3d::ProxyShape* floorProxyShape = floor->addCollisionShape(&floorShape, rp3d::Transform::identity(), 0);
	floorProxyShape->setCollisionCategoryBits(CATEGORY1 | CATEGORY2);
	floorProxyShape->setCollideWithMaskBits(CATEGORY1 | CATEGORY2);
	body->getMaterial().setBounciness(0.4f);
	body->getMaterial().setFrictionCoefficient(0.2f);

	const float timeStep = 1.0f / 60.0f;
	for (size_t i = 0; i < 60 * 4; ++i) {
		world.update(timeStep);
		std::cout
				<< "\nobject"
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

	world.destroyRigidBody(body);
	world.destroyRigidBody(floor);

	return 0;
}
