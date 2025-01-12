#include "CollisionHandler.hpp"

bool CollisionHandler::apply_impulse = false;

bool CollisionHandler::apply_displacement = false;

bool CollisionHandler::enable_debug = true;

bool CollisionHandler::enable_interactions = true;

float CollisionHandler::elasticity = 0.5f;

float CollisionHandler::slipperiness = 0.5f;

bool CollisionHandler::isCollidingContact(float relativeVelocity) {
    static const float threshold = 0.00001f;

    if (relativeVelocity > threshold) {
        return false;
    }
    if (relativeVelocity > -threshold) {
        return false;
    }
    else {
        return true;
    }
}

std::vector<CollisionHandler::CONTACT> CollisionHandler::calculateInteractions(const std::vector<SceneObject*>& sceneObjects) {
    std::vector<CONTACT> interactions;
    auto invalidPair = [&interactions](SceneObject* s1, SceneObject* s2) {
        if (s1 == s2) {
            return true;
        }

        for (const auto& i : interactions) {
            SceneObject* _s1 = std::get<0>(i);
            SceneObject* _s2 = std::get<1>(i);

            if (s1 == _s1 && s2 == _s2) {
                return true;
            }

            if (s1 == _s2 && s2 == _s1) {
                return true;
            }
        }
        return false;
    };
    auto optimizeCuboids = [](SceneObject* s1, SceneObject* s2) {
        CuboidCollider* c1 = dynamic_cast<CuboidCollider*>(s1->getCollider());
        CuboidCollider* c2 = dynamic_cast<CuboidCollider*>(s2->getCollider());

        if (c1 != nullptr && c2 != nullptr) {
            std::vector<float> dim1 = { c1->getWidth(), c1->getHeight(), c1->getLength() };
            float r1 = 0;
            for (float d : dim1) {
                if (d > r1) {
                    r1 = d;
                }
            }

            std::vector<float> dim2 = { c2->getWidth(), c2->getHeight(), c2->getLength() };
            float r2 = 0;
            for (float d : dim2) {
                if (d > r2) {
                    r2 = d;
                }
            }

            auto sc1 = SphereCollider(r1, 0);
            sc1.setTransformation(s1->getTransformation());
            sc1.updateTransformations();

            auto sc2 = SphereCollider(r2, 0);
            sc2.setTransformation(s2->getTransformation());
            sc2.updateTransformations();

            auto coll1 = sc1.collidesWith(sc2);
            auto coll2 = sc2.collidesWith(sc1);
            if (coll1.size() > 0 || coll2.size() > 0) {
                return false;
            }
            else {
                return true;
            }

            sc1.setTransformation(nullptr);
            sc2.setTransformation(nullptr);
        }
        else { 
            return false; 
        }
    };

    for (SceneObject* s1 : sceneObjects) {
        for (SceneObject* s2 : sceneObjects) {
            if (!invalidPair(s1, s2)) {
                auto s1_collider = s1->getCollider();
                auto s2_collider = s2->getCollider();

                if (optimizeCuboids(s1, s2)) {
                    continue;
                }

                auto contact1 = s1_collider->collidesWith(*s2_collider);
                auto contact2 = s2_collider->collidesWith(*s1_collider);

                SceneObject* colliderObject;
                SceneObject* collideeObject;
                std::vector<Collider::ContactData>* contactData = nullptr;

                for (const auto& c : contact1) {
                    if (c.vertexFace) {
                        colliderObject = s1;
                        collideeObject = s2;
                        contactData = &contact1;
                        break;
                    }
                }
                
                for (const auto& c : contact2) {
                    if (!c.vertexFace) {
                        colliderObject = s2;
                        collideeObject = s1;
                        contactData = &contact2;
                        break;
                    }
                }

                if (contactData == nullptr) {
                    if (contact1.size() > 0) {
                        interactions.push_back(std::make_tuple(s1, s2, contact1));
                    }
                }
                else {
                    auto interaction = std::make_tuple(colliderObject, collideeObject, *contactData);
                    interactions.push_back(interaction);
                }
            }
        }
    }

    return interactions;
}

void CollisionHandler::debugContact(const CONTACT& interaction, const Camera& camera) {
    auto colliderObject = std::get<0>(interaction);
    auto collideeObject = std::get<1>(interaction);
    auto contactData = std::get<2>(interaction);

    RigidBody::BodyData* A = &colliderObject->getBodyStateSolver()->Body;
    RigidBody::BodyData* B = &collideeObject->getBodyStateSolver()->Body;

    std::vector<glm::vec3> contactPoints;
    for (const auto& c : contactData) {
        contactPoints.push_back(c.point);

        glm::vec3 ra = c.point - A->X;
        glm::vec3 rb = c.point - B->X;
        glm::vec3 velpa = A->vel + glm::cross(A->omega, ra);
        glm::vec3 velpb = B->vel + glm::cross(B->omega, rb);
        glm::vec3 vrel = velpa - velpb;
        glm::vec3 vrelt = vrel - glm::dot(vrel, c.normal) * c.normal;

        DebugDrawer::setOverrideZ(1);
        DebugDrawer::setMode(GL_LINES);

       /* DebugDrawer::setVertexData({ c.point, c.point + 10.0f * velpa });
        DebugDrawer::draw(camera, glm::vec3(1.0f, 0.0f, 0.0f));

        DebugDrawer::setVertexData({ c.point, c.point + 10.0f * velpb });
        DebugDrawer::draw(camera, glm::vec3(0.0f, 0.0f, 1.0f));

        DebugDrawer::setVertexData({ c.point, c.point + 10.0f * vrel });
        DebugDrawer::draw(camera, glm::vec3(1.0f, 0.0f, 1.0f));*/

        /*DebugDrawer::setVertexData({c.point, c.point + 10.0f * vrelt});
        DebugDrawer::draw(camera, glm::vec3(0.0f, 0.0f, 0.0f));*/

        DebugDrawer::setVertexData({ c.point, c.point + 4.0f * c.normal });
        DebugDrawer::draw(camera, glm::vec3(0.5f, 0.5f, 0.0f));

        glm::vec3 rA = c.edgeA[0];
        glm::vec3 xA = colliderObject->getBodyStateSolver()->Body.X;
        glm::vec3 rB = c.edgeB[0];
        glm::vec3 xB = collideeObject->getBodyStateSolver()->Body.X;
        if (!c.vertexFace) {
           /* DebugDrawer::setMode(GL_POINTS);
            DebugDrawer::setOverrideZ(1);

            DebugDrawer::setVertexData({ rA, xA });
            DebugDrawer::draw(camera, glm::vec3(0.0f, 0.0f, 0.0f));

            DebugDrawer::setVertexData({ rB, xB });
            DebugDrawer::draw(camera, glm::vec3(1.0f, 1.0f, 1.0f));*/
        }

        Collider* collider = colliderObject->getCollider();
        Collider* collidee = collideeObject->getCollider();
        glm::vec3* distanceVector = collider->calculateContactDepthWith(*collidee, c);
        if (distanceVector != nullptr) {
            DebugDrawer::setMode(GL_LINES);
            DebugDrawer::setOverrideZ(1);
            if (c.vertexFace) {
                DebugDrawer::setVertexData({ c.point, c.point + *distanceVector });
            }
            else {
                DebugDrawer::setVertexData({ rA, rA + *distanceVector });
            }
            DebugDrawer::draw(camera, glm::vec3(0.0f, 0.0f, 0.0f));
            delete distanceVector;
        }
    }

    DebugDrawer::setMode(GL_POINTS);
    DebugDrawer::setOverrideZ(1);
    DebugDrawer::setVertexData(contactPoints);
    DebugDrawer::draw(camera, glm::vec3(0.0f, 1.0f, 0.0f));
}

void CollisionHandler::drawCollidersOf(const std::vector<SceneObject*>& sceneObjects, const Camera& camera) {
    for (SceneObject* sceneObject : sceneObjects) {
        auto cuboidCollider = dynamic_cast<CuboidCollider*>(sceneObject->getCollider());
        if (cuboidCollider != nullptr) {
            auto p = cuboidCollider->getTransSubdPoints();
            auto points = std::vector<glm::vec3>(p.begin(), p.end());
            DebugDrawer::setMode(GL_POINTS);
            DebugDrawer::setVertexData(points);
            DebugDrawer::setOverrideZ(0);
            DebugDrawer::draw(camera, glm::vec3(1.0f, 0.0f, 0.0f));

            std::vector<std::vector<glm::vec3>> sideData;
            sideData.push_back(cuboidCollider->getSideDrawData(0));
            sideData.push_back(cuboidCollider->getSideDrawData(1));
            sideData.push_back(cuboidCollider->getSideDrawData(2));
            sideData.push_back(cuboidCollider->getSideDrawData(3));
            sideData.push_back(cuboidCollider->getSideDrawData(4));
            sideData.push_back(cuboidCollider->getSideDrawData(5));

            for (const auto& data : sideData) {
                DebugDrawer::setMode(GL_LINES);
                DebugDrawer::setOverrideZ(0);
                DebugDrawer::setVertexData(data);
                DebugDrawer::draw(camera, glm::vec3(0.0f, 0.4f, 0.4f));
            }
        }

        auto sphereCollider = dynamic_cast<SphereCollider*>(sceneObject->getCollider());
        if (sphereCollider != nullptr) {
            auto p = sphereCollider->getTransPoints();
            auto points = std::vector<glm::vec3>(p.begin(), p.end());
            DebugDrawer::setMode(GL_POINTS);
            DebugDrawer::setVertexData(points);
            DebugDrawer::setOverrideZ(0);
            DebugDrawer::draw(camera, glm::vec3(1.0f, 0.0f, 0.0f));
        }
    }
}

void CollisionHandler::pushObjectsApart(const CONTACT& interaction) {
    auto colliderObject = std::get<0>(interaction);
    auto collideeObject = std::get<1>(interaction);
    auto contactData = std::get<2>(interaction);

    Collider* collider = colliderObject->getCollider();
    Collider* collidee = collideeObject->getCollider();
    glm::vec3 displacementVector = glm::vec3(0.0f, 0.0f, 0.0f);
    float max_dist = 0.0f;
    for (const auto& c : contactData) {
        glm::vec3* d = collider->calculateContactDepthWith(*collidee, c);;
        if (d != nullptr) {
            float ld = glm::length(*d);
            if (ld > max_dist) {
                max_dist = ld;
                displacementVector = *d;
            }
            delete d;
        }
    }

    if (max_dist == 0.0f) {
        return;
    }

    auto pushApart = [&colliderObject, &collideeObject](glm::vec3 dis, glm::vec3 dir) {
        glm::vec3 colliderPos = colliderObject->getBodyStateSolver()->Body.X;
        glm::vec3 collideePos = collideeObject->getBodyStateSolver()->Body.X;
        SceneObject* objectToDisplace;

        if (glm::dot(dir, dis) < 0.0f) {
            dis = -dis;
        }
        if (glm::dot(colliderPos, dir) > glm::dot(collideePos, dir)) {
            float invMass = colliderObject->getBodyStateSolver()->Body.invMass;
            objectToDisplace = invMass != 0.0f ? colliderObject : collideeObject;
        }
        else {
            float invMass = collideeObject->getBodyStateSolver()->Body.invMass;
            objectToDisplace = invMass != 0.0f ? collideeObject : colliderObject;
        }

        objectToDisplace->getBodyStateSolver()->Body.X += dis;
        objectToDisplace->updateTransformations();
    };

    std::array<glm::vec3, 3> directions = { glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) };
    for (const glm::vec3& d : directions) {
        glm::vec3 projection = d * glm::dot(displacementVector, d);
        if (glm::length(projection) > 0.01f) {
            pushApart(projection, d);
        }
    }

}

void CollisionHandler::applyImpulse(const CONTACT& interaction) {
    RigidBody::BodyData* A = &(std::get<0>(interaction)->getBodyStateSolver()->Body);
    RigidBody::BodyData* B = &(std::get<1>(interaction)->getBodyStateSolver()->Body);
    auto contactData = std::get<2>(interaction);

    for (const auto& c : contactData) {
        glm::vec3 ra = c.point - A->X;
        glm::vec3 rb = c.point - B->X;
        glm::vec3 velpa = A->vel + glm::cross(A->omega, ra);
        glm::vec3 velpb = B->vel + glm::cross(B->omega, rb);
        float lvreln = glm::dot(c.normal, velpa - velpb);

        if (isCollidingContact(lvreln)) {
            glm::vec3 term1an = A->Iinv * glm::cross(ra, c.normal); 
            glm::vec3 term1bn = B->Iinv * glm::cross(rb, c.normal);
            float term2an = glm::dot(c.normal, glm::cross(term1an, ra));
            float term2bn = glm::dot(c.normal, glm::cross(term1bn, rb));

            float numerator = -(1.0f + elasticity) * lvreln;
            float denominator = A->invMass + B->invMass + term2an + term2bn;
            float j = numerator / denominator;

            glm::vec3 impForce = j * c.normal;
            glm::vec3 impTorqueA = glm::cross(ra, impForce);
            glm::vec3 impTorqueB = glm::cross(rb, impForce);

            A->P += impForce;
            A->vel = A->P * A->invMass;
            A->L += impTorqueA;
            A->omega = A->Iinv * A->L;

            B->P -= impForce;
            B->vel = B->P * B->invMass;
            B->L -= impTorqueB;
            B->omega = B->Iinv * B->L;

            glm::vec3 vrel = velpa - velpb;
            glm::vec3 vrelt = vrel - glm::dot(vrel, c.normal) * c.normal;
            glm::vec3 tangent = glm::normalize(vrelt);
            
            if (glm::length(vrelt) > 0.0f) {
                glm::vec3 term1at = A->Iinv * glm::cross(ra, tangent);
                glm::vec3 term1bt = B->Iinv * glm::cross(rb, tangent);
                float term2at = glm::dot(tangent, glm::cross(term1at, ra));
                float term2bt = glm::dot(tangent, glm::cross(term1bt, rb));

                float jtmax = (1.0f - slipperiness) * j;
                float denominator = A->invMass + B->invMass + term2at + term2bt;
                float jt = glm::length(vrelt) / denominator;
                jt = glm::clamp(jt, -jtmax, jtmax);

                glm::vec3 fricImpForce = -jt * tangent;
                glm::vec3 fricImpTorqueA = glm::cross(ra, fricImpForce);
                glm::vec3 fricImpTorqueB = glm::cross(rb, fricImpForce);

                A->P += fricImpForce;
                A->vel = A->P * A->invMass;
                A->L += fricImpTorqueA;
                A->omega = A->Iinv * A->L;

                B->P -= fricImpForce;
                B->vel = B->P * B->invMass;
                B->L -= fricImpTorqueB;
                B->omega = B->Iinv * B->L;
            }
        }
    }
}

void CollisionHandler::handleCollisions(const std::vector<SceneObject*>& sceneObjects, const Camera& camera) {
    if (enable_debug) {
        drawCollidersOf(sceneObjects, camera);
    }

    auto interactions = calculateInteractions(sceneObjects);
    for (const auto& interaction : interactions) {
        if (enable_debug) {
            debugContact(interaction, camera);
        }
        if (enable_interactions) {
            applyImpulse(interaction);
            pushObjectsApart(interaction);
        }
        
        if (apply_displacement) {
            pushObjectsApart(interaction);
            apply_displacement = false;
        }

        if (apply_impulse) {
            applyImpulse(interaction);
            apply_impulse = false;
        }
    }
}