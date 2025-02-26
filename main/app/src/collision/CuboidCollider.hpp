#pragma once

#include <array>

#include "Collider.hpp"
#include "TerrainCollider.hpp"

class CuboidCollider : public Collider {

public:
	struct Side {
		std::array<glm::vec3, 4> points;
		glm::vec3 center;
		glm::vec3 normal;
	};

private:
	std::array<glm::vec3, 8> transPoints;
	std::array<Side, 6> transSides;

	std::array<glm::vec3, 8> basePoints;
	std::array<Side, 6> baseSides;

	std::vector<glm::vec3> baseSubdPoints;
	std::vector<glm::vec3> transSubdPoints;

private:
	float width;
	float length;
	float height;

private:
	void createCorners();
	void createSides();
	Side createSide(const std::array<unsigned int, 4>& indices) const;
	void createSubdivision(unsigned int subdivideInto);

public:
	static float vertexFaceThreshold;
	static float edgeEdgeThreshold;

private:
	std::vector<glm::vec3> getEdgesOf(const CuboidCollider& collider) const;
	Collider::ContactData* calculateEdgeIntersection(glm::vec3 pa, glm::vec3 va, float a, glm::vec3 pb, glm::vec3 vb, float b) const;
	std::vector<ContactData> edgeEdgeCollision(const CuboidCollider& collidee) const;

	bool calculateSideIntersection(glm::vec3 point, const Side& side) const;
	std::vector<ContactData> vertexFaceCollision(const CuboidCollider& collidee) const;

public:
	CuboidCollider(float w, float h, float l);
	CuboidCollider(const CuboidCollider& c);

	float getWidth() const { return width; }
	float getLength() const { return length; }
	float getHeight() const { return height; }

	const std::array<glm::vec3, 8>& getTransPoints() const { return transPoints; }
	const std::array<glm::vec3, 8>& getBasePoints() const { return basePoints; }
	const std::array<Side, 6>& getTransSides() const { return transSides; }
	const std::array<Side, 6>& getBaseSides() const { return baseSides; }
	const std::vector<glm::vec3>& getTransSubdPoints() const { return transSubdPoints; }

	std::vector<glm::vec3> getSideDrawData(unsigned int sideIndex);

	void updateTransformations() override;

	std::vector<ContactData> collidesWith(const CuboidCollider& collidee) const override;
	std::vector<ContactData> collidesWith(const SphereCollider& collidee) const override;
	std::vector<ContactData> collidesWith(const TerrainCollider& collidee) const override;

	virtual glm::vec3* CuboidDepth(const CuboidCollider& collidee, const ContactData& contact) override;
	virtual glm::vec3* SphereDepth(const SphereCollider& collidee, const ContactData& contact) override;
	virtual glm::vec3* TerrainDepth(const TerrainCollider& collidee, const ContactData& contact) override;
};

