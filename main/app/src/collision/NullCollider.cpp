#include "NullCollider.hpp"

NullCollider::NullCollider() {
    type = C_NULL;
}

std::vector<Collider::ContactData> NullCollider::collidesWith(const CuboidCollider& collidee) const {
    return std::vector<Collider::ContactData>();
}

std::vector<Collider::ContactData> NullCollider::collidesWith(const SphereCollider& collidee) const {
    return std::vector<Collider::ContactData>();
}

std::vector<Collider::ContactData> NullCollider::collidesWith(const TerrainCollider& collidee) const {
    return std::vector<Collider::ContactData>();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

glm::vec3* NullCollider::CuboidDepth(const CuboidCollider& collidee, const ContactData& contact)
{
    return nullptr;
}

glm::vec3* NullCollider::SphereDepth(const SphereCollider& collidee, const ContactData& contact)
{
    return nullptr;
}

glm::vec3* NullCollider::TerrainDepth(const TerrainCollider& collidee, const ContactData& contact)
{
    return nullptr;
}
