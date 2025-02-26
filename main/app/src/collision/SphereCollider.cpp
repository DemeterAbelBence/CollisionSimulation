#include "SphereCollider.hpp"
#include "TerrainCollider.hpp"

SphereCollider::SphereCollider(float _radius, unsigned int n) {
	type = C_SPHERE;
	radius = _radius;

	baseCenter = glm::vec3(0.0f, 0.0f, 0.0f);
	transCenter = glm::vec3(0.0f, 0.0f, 0.0f);;

	std::vector<glm::vec3> points;
	points.reserve(n);

	const float phi = 3.14159f * (3.0 - std::sqrt(5.0));

	for (int i = 0; i < n; ++i) {
		float y = 1 - (i / static_cast<float>(n - 1)) * 2;
		float radiusAtY = std::sqrt(1 - y * y);

		float theta = phi * i;
		float x = std::cos(theta) * radiusAtY;
		float z = std::sin(theta) * radiusAtY;

		glm::vec3 point = baseCenter + glm::vec3(x, y, z) * radius;
		points.push_back(point);
	}

	basePoints = points;
	transPoints = points;
}

void SphereCollider::updateTransformations() {
	glm::mat4 M = transformation->makeModelMatrix();
	glm::mat4 MI = transformation->makeModelInverseMatrix();

	transCenter = glm::vec3(glm::vec4(baseCenter, 1.0f) * M);

	for (int i = 0; i < basePoints.size(); ++i) {
		transPoints[i] = glm::vec3(glm::vec4(basePoints[i], 1.0f) * M);
	}
}

std::vector<Collider::ContactData> SphereCollider::collidesWith(const CuboidCollider& collidee) const
{
	return std::vector<ContactData>();
}

std::vector<Collider::ContactData> SphereCollider::collidesWith(const SphereCollider& collidee) const {
	glm::vec3 v = collidee.transCenter - transCenter;
	if (glm::length(v) < radius + collidee.radius) {
		v = glm::normalize(v);
		glm::vec3 p = transCenter + v * radius;
		return { { p, v, {}, {}, true } };
	}
}

std::vector<Collider::ContactData> SphereCollider::collidesWith(const TerrainCollider& collidee) const {
	std::vector<ContactData> result;
	auto MI = collidee.getTransformation()->makeModelInverseMatrix();
	auto M = collidee.getTransformation()->makeModelMatrix();
	for (const auto& p : transPoints) {
		glm::vec3 tp = glm::vec3(glm::vec4(p, 1.0f) * MI);
		auto e = collidee.evaluateTerrainFormula(tp.x, tp.z);

		glm::vec3 ep = std::get<0>(e);
		glm::vec3 en = std::get<1>(e);
		glm::vec3 tep = glm::vec3(glm::vec4(ep, 1.0f) * M);
		glm::vec3 ten = glm::vec3(MI * glm::vec4(en, 0.0f));

		if (tep.y > p.y) {
			result.push_back({
				p,
				glm::normalize(ten),
				{},
				{},
				true
				});
		}
	}
	return result;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

glm::vec3* SphereCollider::CuboidDepth(const CuboidCollider& collidee, const ContactData& contact)
{
	return nullptr;
}

glm::vec3* SphereCollider::SphereDepth(const SphereCollider& collidee, const ContactData& contact) {
	glm::vec3 v = collidee.transCenter - transCenter;
	float dist = glm::length(v);
	float max_dist = radius + collidee.radius;

	if (dist < max_dist) {
		float l = max_dist - dist;
		return new glm::vec3(l * glm::normalize(v));
	}
	else {
		return nullptr;
	}
	
}

glm::vec3* SphereCollider::TerrainDepth(const TerrainCollider& collidee, const ContactData& contact) {
	auto MI = collidee.getTransformation()->makeModelInverseMatrix();
	auto M = collidee.getTransformation()->makeModelMatrix();

	glm::vec3 tp = glm::vec3(glm::vec4(contact.point, 1.0f) * MI);
	auto e = collidee.evaluateTerrainFormula(tp.x, tp.z);

	glm::vec3 ep = std::get<0>(e);
	glm::vec3 tep = glm::vec3(glm::vec4(ep, 1.0f) * M);

	if (tep.y < contact.point.y) {
		return nullptr;
	}
	else {
		return new glm::vec3(tep - contact.point);
	}
}
