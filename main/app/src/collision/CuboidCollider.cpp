#include "CuboidCollider.hpp"

float CuboidCollider::vertexFaceThreshold = 0.3f;

float CuboidCollider::edgeEdgeThreshold = 0.3f;

void CuboidCollider::createCorners() {
    float w = width / 2.0f;
    float h = height / 2.0f;
    float l = length / 2.0f;

    basePoints[0] = glm::vec3(-w, -h, l);
    basePoints[1] = glm::vec3(w, -h, l);
    basePoints[2] = glm::vec3(w, -h, -l);
    basePoints[3] = glm::vec3(-w, -h, -l);

    basePoints[4] = glm::vec3(-w, h, l);
    basePoints[5] = glm::vec3(w, h, l);
    basePoints[6] = glm::vec3(w, h, -l);
    basePoints[7] = glm::vec3(-w, h, -l);

    transPoints = basePoints;
}

CuboidCollider::Side CuboidCollider::createSide(const std::array<unsigned int, 4>& indices) const {
    std::array<glm::vec3, 4> points;
    glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < indices.size(); ++i) {
        points[i] = basePoints[indices[i]];
        center += points[i];
    }
    center /= static_cast<float>(indices.size());

    return { points, center, glm::normalize(center) };
}

void CuboidCollider::createSubdivision(unsigned int subdivideInto) {
     const float eps = 0.0001f;
     std::vector<glm::vec3> totalSubd;

     auto side = baseSides[0];
     glm::vec3 sa = side.points[1] - side.points[0];
     glm::vec3 sb = side.points[3] - side.points[0];
     float la = glm::length(sa);
     float lb = glm::length(sb);
     float da = la / (float)subdivideInto;
     float db = lb / (float)subdivideInto;
     sa = glm::normalize(sa);
     sb = glm::normalize(sb);
     std::vector<glm::vec3> sideSubd;
     for (float i = 0.0f; i < la + eps; i += da) {
         for (float j = 0.0f; j < lb + eps; j += db) {
             glm::vec3 p = side.points[0] + i * sa + j * sb;
             sideSubd.push_back(p);
         }
     }
     totalSubd.insert(totalSubd.begin(), sideSubd.begin(), sideSubd.end());
     for (auto& p : sideSubd) {
         p -= height * side.normal;
     }
     totalSubd.insert(totalSubd.begin(), sideSubd.begin(), sideSubd.end());

     auto p = side.points;
     std::vector<glm::vec3> edges;
     edges.push_back(p[0]); edges.push_back(p[1]);
     edges.push_back(p[1]); edges.push_back(p[2]);
     edges.push_back(p[2]); edges.push_back(p[3]);
     edges.push_back(p[3]); edges.push_back(p[0]);
     std::vector<glm::vec3> borderSubd;
     for (int i = 0; i < edges.size(); i += 2) {
         glm::vec3 ev = edges[i + 1] - edges[i];
         glm::vec3 en = glm::normalize(ev);
         float el = glm::length(ev);
         float ed = el / (float)subdivideInto;
         float l = el - ed + eps;
         for (float j = 0.0f; j < l; j += ed) {
             glm::vec3 p = edges[i] + j * en;
             borderSubd.push_back(p);
         }
     }

     float dh = height / (float)subdivideInto;
     for (float i = dh; i < height - dh + eps; i += dh) {
         for (const auto& b : borderSubd) {
             glm::vec3 p = b - i * side.normal;
             totalSubd.push_back(p);
         }
     }

     baseSubdPoints = totalSubd;
     transSubdPoints = totalSubd;
}

void CuboidCollider::createSides() {
    std::array<unsigned int, 4> sideIndices[6] =
    {
        {0, 1, 2, 3},
        {4, 5, 6, 7},

        {0, 3, 7, 4},
        {1, 2, 6, 5},

        {0, 1, 5, 4},
        {3, 2, 6, 7}
    };

    for (int i = 0; i < 6; ++i) {
        baseSides[i] = createSide(sideIndices[i]);
        transSides[i] = createSide(sideIndices[i]);
    }     
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

std::vector<glm::vec3> CuboidCollider::getEdgesOf(const CuboidCollider& collider) const {
    auto p = collider.transPoints;
    std::vector<glm::vec3> edges;

    edges.push_back(p[0]); edges.push_back(p[1]);
    edges.push_back(p[1]); edges.push_back(p[2]);
    edges.push_back(p[2]); edges.push_back(p[3]);
    edges.push_back(p[3]); edges.push_back(p[0]);

    edges.push_back(p[4]); edges.push_back(p[5]);
    edges.push_back(p[5]); edges.push_back(p[6]);
    edges.push_back(p[6]); edges.push_back(p[7]);
    edges.push_back(p[7]); edges.push_back(p[4]);

    edges.push_back(p[0]); edges.push_back(p[4]);
    edges.push_back(p[1]); edges.push_back(p[5]);
    edges.push_back(p[2]); edges.push_back(p[6]);
    edges.push_back(p[3]); edges.push_back(p[7]);

    return edges;
}

Collider::ContactData* CuboidCollider::calculateEdgeIntersection(glm::vec3 pa, glm::vec3 va, float a, glm::vec3 pb, glm::vec3 vb, float b) const {
    using namespace glm;
    float areParallel = dot(va, vb);
    const float eps = glm::pow(10.0f, -3);

    if (areParallel + eps >= 1.0f && areParallel - eps <= 1.0f) {
        return nullptr;
    }
    if (areParallel + eps >= -1.0f && areParallel - eps <= -1.0f) {
        return nullptr;
    }

    if (va == vec3(0.0f, 0.0f, 0.0f) || vb == vec3(0.0f, 0.0f, 0.0f)) {
        std::cout << "Edge direction vector is zero" << std::endl;
        return nullptr;
    }

    vec3 c = cross(va, vb);
    if (c == vec3(0.0f, 0.0f, 0.0f)) {
        return nullptr;
    }

    float dist = glm::length(dot(pb - pa, c)) / glm::length(c);
    if (dist > edgeEdgeThreshold) {
        return nullptr;
    }
    else {
        float PA = dot(pb, va) - dot(pa, va);
        float PB = dot(pa, vb) - dot(pb, vb);
        float VBA = dot(vb, va);
        float VBB = dot(vb, vb);
        float VAA = dot(va, va);
        float beta = (-PB - (PA * VBA) / VAA) / (VBA * VBA / VAA - VBB);
        float alpha = (beta * VBA + PA) / VAA;
        
        if (!(0 <= alpha && alpha <= a)) {
            return nullptr;
        }
        if (!(0 <= beta && beta <= b)) {
            return nullptr;
        }

        vec3 ra = pa + alpha * va;
        vec3 rb = pb + beta * vb;
        return new ContactData {
            glm::vec3((ra + rb) / 2.0f),
            glm::normalize(glm::cross(vb, va)),
            {ra, va},
            {rb, vb},
            false
        };
    }
}

std::vector<Collider::ContactData> CuboidCollider::edgeEdgeCollision(const CuboidCollider& collidee) const {
    std::vector<Collider::ContactData> result;
    std::vector<glm::vec3> edgesA = getEdgesOf(*this);
    std::vector<glm::vec3> edgesB = getEdgesOf(collidee);

    for (int i = 0; i < edgesA.size(); i += 2) {
        glm::vec3 vecA = edgesA[i + 1] - edgesA[i];
        float a = glm::length(vecA); 

        for (int j = 0; j < edgesB.size(); j += 2) {
            glm::vec3 vecB = edgesB[j + 1] - edgesB[j];
            float b = glm::length(vecB);

            vecA = glm::normalize(vecA);
            vecB = glm::normalize(vecB);

            auto* contact = calculateEdgeIntersection(edgesA[i], vecA, a, edgesB[j], vecB, b);
            if (contact != nullptr) {   
                result.push_back(*contact);
                delete contact;
            }
        }
    }

    return result;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

bool CuboidCollider::calculateSideIntersection(glm::vec3 point, const Side& side) const {
    struct edge {
        glm::vec3 edgePoint;
        glm::vec3 edgeVector;
    };

    float arePerpendicular = glm::dot(side.normal, point - side.points[0]);
    if (glm::abs(arePerpendicular) > vertexFaceThreshold) {
        return false;
    }
    else {
        std::vector<edge> edges;
        edges.push_back({ side.points[0], side.points[1] - side.points[0] });
        edges.push_back({ side.points[1], side.points[2] - side.points[1] });
        edges.push_back({ side.points[2], side.points[3] - side.points[2] });
        edges.push_back({ side.points[3], side.points[0] - side.points[3] });

        for (const auto& e : edges) {
            glm::vec3 ev = glm::normalize(e.edgeVector);
            glm::vec3 pv = glm::normalize(point - e.edgePoint);
            float d = glm::dot(ev, pv);
            if (!(0.0f <= d && d <= 1.0f)) {
                return false;
            }
        }
        return true;
    }
}

std::vector<Collider::ContactData> CuboidCollider::vertexFaceCollision(const CuboidCollider& collidee) const {
    std::vector<Collider::ContactData> result;

    for (const Side& side : collidee.getTransSides()) {
        for (const glm::vec3& point : transPoints) {
            if (calculateSideIntersection(point, side)) {
                result.push_back({
                    point,
                    side.normal,
                    {},
                    {},
                    true
                });
            }
        }
    }

    return result;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

CuboidCollider::CuboidCollider(float w, float h, float l) {
    width = w;
    height = h;
    length = l;

    type = Type::C_CUBOID;

    createCorners();
    createSides();
    createSubdivision(2);
}

CuboidCollider::CuboidCollider(const CuboidCollider& c) {
    type = c.type;

    width = c.width;
    height = c.height;
    length = c.length;

    basePoints = c.basePoints;
    baseSides = c.baseSides;

    transPoints = c.transPoints;
    transSides = c.transSides;

    transformation = c.transformation;
}

std::vector<glm::vec3> CuboidCollider::getSideDrawData(unsigned int sideIndex)
{
    std::vector<glm::vec3> result;
    auto points = transSides[sideIndex].points;
    glm::vec3 center = transSides[sideIndex].center;
    glm::vec3 normal = transSides[sideIndex].normal;

    for (auto& p : points) {
        p += 0.1f * normal;
    }

    result.push_back(center); result.push_back(center + 1.5f * normal);

    result.push_back(points[0]); result.push_back(points[1]);
    result.push_back(points[1]); result.push_back(points[2]);
    result.push_back(points[2]); result.push_back(points[3]);
    result.push_back(points[3]); result.push_back(points[0]);
    
    return result;
}

void CuboidCollider::updateTransformations() {
    glm::mat4 M = transformation->makeModelMatrix();
    glm::mat4 MI = transformation->makeModelInverseMatrix();

    for (int i = 0; i < transPoints.size(); ++i) {
        transPoints[i] = glm::vec3(glm::vec4(basePoints[i], 1.0f) * M);
    }

    for (int i = 0; i < transSubdPoints.size(); ++i) {
        transSubdPoints[i] = glm::vec3(glm::vec4(baseSubdPoints[i], 1.0f) * M);
    }

    for (int i = 0; i < transSides.size(); ++i) {
        for (int j = 0; j < transSides[i].points.size(); ++j) {
            glm::vec3 newPoint = glm::vec3(glm::vec4(baseSides[i].points[j], 1.0f) * M);
            transSides[i].points[j] = newPoint;
        }
        transSides[i].center = glm::vec3(glm::vec4(baseSides[i].center, 1.0f) * M);
        transSides[i].normal = glm::vec3(MI * glm::vec4(baseSides[i].normal, 0.0f));
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

std::vector<Collider::ContactData> CuboidCollider::collidesWith(const CuboidCollider& collidee) const {
    auto edgeEdge = edgeEdgeCollision(collidee);
    auto vertexFace = vertexFaceCollision(collidee);
    edgeEdge.insert(edgeEdge.end(), vertexFace.begin(), vertexFace.end());
    return edgeEdge;
}

std::vector<Collider::ContactData> CuboidCollider::collidesWith(const SphereCollider& collidee) const {
    return std::vector<ContactData>();
}

std::vector<Collider::ContactData> CuboidCollider::collidesWith(const TerrainCollider& collidee) const {
    std::vector<ContactData> result;
    auto MI = collidee.getTransformation()->makeModelInverseMatrix();
    auto M = collidee.getTransformation()->makeModelMatrix();
    for (const auto& p : transSubdPoints) {
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

glm::vec3* CuboidCollider::CuboidDepth(const CuboidCollider& collidee, const ContactData& contact) {
    if (contact.vertexFace) {
        const CuboidCollider::Side* contactSide = nullptr;
        const float eps = 0.0001f;
        for (const auto& side : collidee.getTransSides()) {
            if (glm::abs(contact.normal.x - side.normal.x) < eps &&
                glm::abs(contact.normal.y - side.normal.y) < eps &&
                glm::abs(contact.normal.z - side.normal.z) < eps) {
                contactSide = &side;
                break;
            }
        }

        if (contactSide == nullptr) {
            std::cout << "Incorrect contact calculation!" << std::endl;
            return nullptr;
        }

        if (glm::dot(contactSide->normal, contact.point - contactSide->center) >= 0.0f) {
            return nullptr;
        }
        else {
            float projectionVectorLength = glm::dot(contactSide->center - contact.point, contact.normal);
            glm::vec3 projectionVector = projectionVectorLength * glm::normalize(contact.normal);
            return new glm::vec3(projectionVector);
        }

        return nullptr;
    }
    else {
        glm::mat4 MI = collidee.getTransformation()->makeModelInverseMatrix();
        glm::vec3 edgePointA = contact.edgeA[0];
        edgePointA = glm::vec3(glm::vec4(edgePointA, 1.0f) * MI);

        const float eps = 0.0001f;
        if (glm::abs(edgePointA.x) < ((collidee.getWidth() / 2.0f) + eps) &&
            glm::abs(edgePointA.y) < ((collidee.getHeight() / 2.0f) + eps) &&
            glm::abs(edgePointA.z) < ((collidee.getLength() / 2.0f) + eps)) {

            return new glm::vec3(contact.edgeB[0] - contact.edgeA[0]);
        }
        else {
            return nullptr;
        }
    }
}

glm::vec3* CuboidCollider::SphereDepth(const SphereCollider& collidee, const ContactData& contact) {
    return nullptr;
}

glm::vec3* CuboidCollider::TerrainDepth(const TerrainCollider& collidee, const ContactData& contact) {
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
