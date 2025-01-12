#pragma once

#include <glm.hpp>
#include <iostream>
#include <vector>
#include <array>

#include "../util/Transformation.hpp"

class CuboidCollider;
class TerrainCollider;
class SphereCollider;

class Collider {
public:
	enum Type {
		C_CUBOID,
		C_SPHERE,
		C_TERRAIN,
		C_NULL
	};
	Type type = C_NULL;

public:
	struct ContactData {
		glm::vec3 point;
		glm::vec3 normal;
		std::array<glm::vec3, 2> edgeA;
		std::array<glm::vec3, 2> edgeB;

		bool vertexFace;
	};

protected:
	Transformation* transformation;

protected:
	virtual std::vector<ContactData> collidesWith(const CuboidCollider& collider) const = 0;
	virtual std::vector<ContactData> collidesWith(const TerrainCollider& collider) const = 0;
	virtual std::vector<ContactData> collidesWith(const SphereCollider& collider) const = 0;

	virtual glm::vec3* CuboidDepth(const CuboidCollider& collidee, const ContactData& contact);
	virtual glm::vec3* SphereDepth(const SphereCollider& collidee, const ContactData& contact);
	virtual glm::vec3* TerrainDepth(const TerrainCollider& collidee, const ContactData& contact);

public:
	inline void setTransformation(Transformation* value) { transformation = value; }
	inline const Transformation* getTransformation() const { return transformation; }
	inline Collider::Type getColliderType() const { return type; }

	std::vector<ContactData> collidesWith(const Collider& collidee) const;
	glm::vec3* calculateContactDepthWith(const Collider& collidee, const ContactData& contact);

	virtual void updateTransformations();
};