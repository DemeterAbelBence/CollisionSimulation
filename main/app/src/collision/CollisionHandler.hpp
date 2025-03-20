#pragma once

#include "../objects/SceneObject.hpp"
#include "../collision/CuboidCollider.hpp"
#include "../collision/TerrainCollider.hpp"
#include "../collision/SphereCollider.hpp"
#include "../collision/NullCollider.hpp"

#include <iostream>
#include <algorithm>
#include <vector>

class CollisionHandler {
public:
    static bool apply_impulse;
    static bool apply_displacement;
    static bool enable_debug;
    static bool enable_interactions;
    static float elasticity;
    static float slipperiness;

private:
    using CONTACT = std::tuple<SceneObject*, SceneObject*, std::vector<Collider::ContactData>>;

private:
    static void debugContact(const CONTACT& interaction, const Camera& camera);
    static void drawCollidersOf(const std::vector<SceneObject*>& sceneObjects, const Camera& camera);

    static bool isCollidingContact(float relativeVelocity);
    static std::vector<CONTACT> calculateInteractions(const std::vector<SceneObject*>& sceneObjects);
    static void pushObjectsApart(const CONTACT& interaction);
    static void applyImpulse(const CONTACT& interaction);

public:
    static void handleCollisions(const std::vector<SceneObject*>& sceneObjects, const Camera& camera);
};
