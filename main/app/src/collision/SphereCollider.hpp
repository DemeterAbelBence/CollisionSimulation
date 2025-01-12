#pragma once

#include "Collider.hpp"

class SphereCollider : public Collider {
private:
	glm::vec3 transCenter;
	glm::vec3 baseCenter;
	float radius;

private:
	std::vector<glm::vec3> basePoints;
	std::vector<glm::vec3> transPoints;

public:
	SphereCollider(float _radius, unsigned int n = 100);

	inline std::vector<glm::vec3> getTransPoints() const { return transPoints; }
	inline glm::vec3 getTransCenter() const { return transCenter; }

	void updateTransformations() override;

	std::vector<ContactData> collidesWith(const CuboidCollider& collidee) const override;
	std::vector<ContactData> collidesWith(const SphereCollider& collidee) const override;
	std::vector<ContactData> collidesWith(const TerrainCollider& collidee) const override;

	glm::vec3* CuboidDepth(const CuboidCollider& collidee, const ContactData& contact) override;
	glm::vec3* SphereDepth(const SphereCollider& collidee, const ContactData& contact) override;
	glm::vec3* TerrainDepth(const TerrainCollider& collidee, const ContactData& contact) override;
};