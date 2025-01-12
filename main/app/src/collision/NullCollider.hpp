#pragma once

#include "Collider.hpp"
#include "CuboidCollider.hpp"

class NullCollider : public Collider {
public:
	NullCollider();

	std::vector<ContactData> collidesWith(const CuboidCollider& collidee) const override;
	std::vector<ContactData> collidesWith(const SphereCollider& collidee) const override;
	std::vector<ContactData> collidesWith(const TerrainCollider& collidee) const override;

	glm::vec3* CuboidDepth(const CuboidCollider& collidee, const ContactData& contact) override;
	glm::vec3* SphereDepth(const SphereCollider& collidee, const ContactData& contact) override;
	glm::vec3* TerrainDepth(const TerrainCollider& collidee, const ContactData& contact) override;
};