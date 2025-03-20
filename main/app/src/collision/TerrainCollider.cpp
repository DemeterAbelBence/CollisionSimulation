#include "TerrainCollider.hpp"

TerrainCollider::TerrainCollider(TerrainData* _terrainData) : terrainData(_terrainData) {
	type = C_TERRAIN;
}

std::tuple<glm::vec3, glm::vec3> TerrainCollider::evaluateTerrainFormula(float x, float z) const {
    int itr = terrainData->iterations;
    float fre = terrainData->frequency;

    float nx = 0;
    float nz = 0;
    float height = 0;

    for (int f1 = 0; f1 < itr; f1++) {
        for (int f2 = 0; f2 < itr; f2++) {
            float amp = calculate_amplitude(f1, f2, this->rand(f1, f2));
            float pha = calculate_phase(f1, f2, this->rand(f1, f2));
            float cos_param = fre * (f1 * x + f2 * z) + pha;
            height += amp * glm::cos(cos_param);

            float sin_param = fre * (f1 * x + f2 * z) + pha;
            nx += amp * fre * f1 * glm::sin(sin_param);
            nz += amp * fre * f2 * glm::sin(sin_param);
        }
    }

    glm::vec3 p = glm::vec3(x, height, z);
    glm::vec3 n = glm::vec3(nx, 1.0f, nz);

    return std::make_tuple(p, n);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

std::vector<Collider::ContactData> TerrainCollider::collidesWith(const CuboidCollider& collidee) const {
	return std::vector<ContactData>();
}

std::vector<Collider::ContactData> TerrainCollider::collidesWith(const SphereCollider& collidee) const {
	return std::vector<ContactData>();
}

std::vector<Collider::ContactData> TerrainCollider::collidesWith(const TerrainCollider& collidee) const {
	return std::vector<ContactData>();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

glm::vec3* TerrainCollider::CuboidDepth(const CuboidCollider& collidee, const ContactData& contact)
{
    return nullptr;
}

glm::vec3* TerrainCollider::SphereDepth(const SphereCollider& collidee, const ContactData& contact)
{
    return nullptr;
}

glm::vec3* TerrainCollider::TerrainDepth(const TerrainCollider& collidee, const ContactData& contact)
{
    return nullptr;
}
