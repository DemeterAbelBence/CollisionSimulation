#pragma once

#include "Collider.hpp"
#include "../util/datatypes/TerrainData.hpp"

#include <tuple>

class TerrainCollider : public Collider {
private:
	TerrainData* terrainData;

private:
	float rand(float x, float z) const {
		return glm::abs(glm::cos(glm::length(glm::vec2(x, z))));
	}

	float calculate_amplitude(float f1, float f2, float A0) const {
		float A;
		float s = glm::sqrt(f1 * f1 + f2 * f2);

		if (s > 0) A = A0 / s;
		else A = A0;
		return terrainData->amplitude * A;
	}

	float calculate_phase(float x, float z, float r) const {
		return terrainData->phase * r;
	}

public:
	TerrainCollider(TerrainData* _terrainData);

	inline void setTerrainData(TerrainData* value) { terrainData = value; }
	inline TerrainData* getTerrainData() const { return terrainData; }

	std::tuple<glm::vec3, glm::vec3> evaluateTerrainFormula(float x, float z) const;

	std::vector<ContactData> collidesWith(const CuboidCollider& collidee) const override;
	std::vector<ContactData> collidesWith(const SphereCollider& collider) const override;
	std::vector<ContactData> collidesWith(const TerrainCollider& collidee) const override;

	glm::vec3* CuboidDepth(const CuboidCollider& collidee, const ContactData& contact) override;
	glm::vec3* SphereDepth(const SphereCollider& collidee, const ContactData& contact) override;
	glm::vec3* TerrainDepth(const TerrainCollider& collidee, const ContactData& contact) override;
};