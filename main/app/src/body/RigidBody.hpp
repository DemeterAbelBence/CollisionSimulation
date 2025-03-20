#pragma once

#include <glm.hpp>
#include <vector>

namespace RigidBody {
	struct BodyData {
		float invMass = 0.0f;
        glm::mat3 Ibody = glm::mat3(1.0f);
        glm::mat3 Ibodyinv = glm::mat3(1.0f);

        glm::vec3 X = glm::vec3(0.0f);
        glm::mat3 R = glm::mat3(1.0f);
        glm::vec3 P = glm::vec3(0.0f);
        glm::vec3 L = glm::vec3(0.0f);

        glm::mat3 Iinv = glm::mat3(1.0f);
        glm::vec3 vel = glm::vec3(0.0f);
        glm::vec3 omega = glm::vec3(0.0f);

        glm::vec3 force = glm::vec3(0.0f);
        glm::vec3 torque = glm::vec3(0.0f);
	};

    static glm::mat3 star(glm::vec3 a) {
        return glm::mat3(
            glm::vec3(0.0f, -a[2], a[1]),
            glm::vec3(a[2], 0.0f, -a[0]),
            glm::vec3(-a[1], a[0], 0.0f)
        );
    }

    static glm::mat3 orthonormalize(const glm::mat3& R) {
        glm::vec3 cx = glm::vec3(R[0][0], R[1][0], R[2][0]);
        glm::vec3 cy = glm::vec3(R[0][1], R[1][1], R[2][1]);
        glm::vec3 cz = glm::vec3(R[0][2], R[1][2], R[2][2]);

        glm::vec3 rx = glm::normalize(cx);
        glm::vec3 ry = glm::normalize(glm::cross(cz, rx));
        glm::vec3 rz = glm::normalize(glm::cross(rx, ry));

        return glm::mat3(
            glm::vec3(rx.x, ry.x, rz.x),
            glm::vec3(rx.y, ry.y, rz.y),
            glm::vec3(rx.z, ry.z, rz.z)
        );
    }

    class BodyStateSolver {

    public:
        BodyData Body;
        BodyData Initial;
        BodyData Previous;

    public:
        BodyStateSolver(BodyData _Body) : Body(_Body) {}
        void rollbackToInitial() { Body = Initial; }
        void rollbackToPrevious() { Body = Previous; }
        void makeStateInitial() { Initial = Body; }

        void computeTotalTorque(const std::vector<glm::vec3>& particles) {
            Body.torque =  glm::cross(particles[0] - Body.X, Body.force);
        }

        void updateAngularComponents(float t) {     
            Body.Iinv = Body.R * Body.Ibodyinv * glm::transpose(Body.R);
            Body.omega = Body.Iinv * Body.L;

            Body.L += Body.torque * t;
            Body.R += star(Body.omega) * Body.R * t;
            Body.R = orthonormalize(Body.R);
        }

        void updateLinearComponents(float t) {
            Body.vel = Body.invMass * Body.P;

            Body.P += Body.force * t;
            Body.X += Body.vel * t;
        }

        void updateState(float t) {
            Previous = Body;
            updateAngularComponents(t);
            updateLinearComponents(t);
        }
    };
}
