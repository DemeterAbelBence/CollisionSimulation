#include "Collider.hpp"
#include "CuboidCollider.hpp"
#include "TerrainCollider.hpp"
#include "SphereCollider.hpp"

glm::vec3* Collider::CuboidDepth(const CuboidCollider& collidee, const ContactData& contact) {
    return nullptr;
}

glm::vec3* Collider::SphereDepth(const SphereCollider& collidee, const ContactData& contact) {
    return nullptr;
}

glm::vec3* Collider::TerrainDepth(const TerrainCollider& collidee, const ContactData& contact) {
    return nullptr;
}

std::vector<Collider::ContactData> Collider::collidesWith(const Collider& collidee) const {
    if (collidee.type == Type::C_CUBOID) {
        auto c = dynamic_cast<const CuboidCollider&>(collidee);
        return collidesWith(c);
    }
    if (collidee.type == Type::C_SPHERE) {
        auto s = dynamic_cast<const SphereCollider&>(collidee);
        return collidesWith(s);
    }
    if (collidee.type == Type::C_TERRAIN) {
        auto t = dynamic_cast<const TerrainCollider&>(collidee);
        return collidesWith(t);
    }

    return std::vector<Collider::ContactData>();
}

glm::vec3* Collider::calculateContactDepthWith(const Collider& collidee, const ContactData& contact) {
    if (collidee.type == C_CUBOID) {
        auto c = dynamic_cast<const CuboidCollider&>(collidee);
        return CuboidDepth(c, contact);
    }
    if (collidee.type == C_SPHERE) {
        auto s = dynamic_cast<const SphereCollider&>(collidee);
        return SphereDepth(s, contact);
    }
    if (collidee.type == C_TERRAIN) {
        auto t = dynamic_cast<const TerrainCollider&>(collidee);
        return TerrainDepth(t, contact);
    }
    return nullptr;
}

void Collider::updateTransformations() {}