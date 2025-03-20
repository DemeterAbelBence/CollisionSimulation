#include "Gui.hpp"

Scene* Gui::scene = nullptr;
ImGuiIO* Gui::io = nullptr;

void Gui::renderRectangle(glm::vec3 color) {
    static Rect rectangle(glm::vec2(-1.0f, -1.0f), glm::vec2(1.0f, 1.0f), color);
    rectangle.draw();
}

void Gui::renderObjectPicker() {
    static float move_speed = 8.0f;
    static float rotate_speed = 1.0f;
    static float object_speed = 0.05f;

    ImGui::Begin("Select object");

    ImGui::SliderFloat("Camera move speed", &move_speed, 0.01f, 50.0f, "%.2f", 0);
    ImGui::SliderFloat("Camera rotate speed", &rotate_speed, 0.001f, 5.0f, "%.3f", 0);
    
    if(ImGui::SliderFloat("Object move speed", &object_speed, 0.01f, 2.0f, "%.2f", 0)) {
        scene->resetObjectEvents(object_speed);
    }

    bool move_camera = scene->getMoveCamera();
    const char* camera_text = move_camera ? "Move objects" : "Move camera";
    if (ImGui::Button(camera_text)) {
        scene->setMoveCamera(!move_camera);
    }

    if (ImGui::Button("Next object")) {
        scene->getNextObject();
    }

    if (scene->getMoveCamera()) {
        ImGui::Text("Currently moving camera");
    }
    else {
        ImGui::Text("Currently moving object");
    }
    ImGui::Text("W-up, S-down, A-left, D-right, LSHIFT-forward");
    ImGui::Text("LCTRL-backward, Q-rotateLeft, E-rotateRight");

    ImGui::End();

    scene->getCamera()->setMoveSpeed(move_speed);
    scene->getCamera()->setRotateSpeed(rotate_speed);
}

void Gui::renderPrimitivePicker() {
    const char* items[] = { "triangle", "line" };
    static int currentItem;

    ImGui::Begin("Select primitive");
    ImGui::ListBox("primitives", &currentItem, items, IM_ARRAYSIZE(items));
    ImGui::End();

    currentItem == 0 ? scene->setPrimitiveType(GL_TRIANGLES) :
        scene->setPrimitiveType(GL_LINE);
}

void Gui::renderFrameRateBox() {
    ImGui::Begin("Performance:");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);
    ImGui::End();
}

void Gui::renderProjectionSettings() {
    static float s_fov = 0.5f, s_near = 0.1f, s_far = 100.0f, s_top = 1.0f,
        s_bottom = -1.0f, s_right = 1.0f, s_left = -1.0f;
    static const float clip_limit = 5.0f;
    static const char* fov_precision = "%.2f";
    static const char* clip_precision = "%.1f";

    ImGui::Begin("Projection settings");
    ImGui::SliderFloat("fov", &s_fov, 0.0f, 2.0f, fov_precision, 0);
    ImGui::SliderFloat("near", &s_near, -clip_limit, clip_limit, clip_precision, 0);
    ImGui::SliderFloat("far", &s_far, -clip_limit, 20 * clip_limit, clip_precision, 0);
    ImGui::SliderFloat("top", &s_top, -clip_limit, clip_limit, clip_precision, 0);
    ImGui::SliderFloat("bottom", &s_bottom, -clip_limit, clip_limit, clip_precision, 0);
    ImGui::SliderFloat("right", &s_right, -clip_limit, clip_limit, clip_precision, 0);
    ImGui::SliderFloat("left", &s_left, -clip_limit, clip_limit, clip_precision, 0);
    if (ImGui::Button("Reset to default")) {
        s_fov = 0.5f;
        s_near = 1.0f; s_far = 100.0f; s_top = 1.0f;
        s_bottom = -1.0f; s_right = 1.0f; s_left = -1.0f;
    }
    ImGui::End();

    Camera* camera = scene->getCamera();
    camera->setFieldOfView(s_fov);
    camera->setNear(s_near);
    camera->setFar(s_far);
    camera->setTop(s_top);
    camera->setBottom(s_bottom);
    camera->setLeft(s_left);
    camera->setRight(s_right);
    camera->resetProjection();
}

void Gui::renderViewSettings() {
    static float eye[3] = { 0.0f, 0.0f, 1.0f };
    static float target[3] = { 0.0f, 0.0f, 0.0f };
    static float up[3] = { 0.0f, 1.0f, 0.0f };
    static float limit = 10.0f;
    static const char* precision = "%.1f";

    ImGui::Begin("Camera settings");
    ImGui::SliderFloat3("eye", &eye[0], -limit, limit, precision, 0);
    ImGui::SliderFloat3("target", &target[0], -limit, limit, precision, 0);
    ImGui::SliderFloat3("up", &up[0], -limit, limit, precision, 0);
    if (ImGui::Button("Reset to default")) {
        eye[0] = 0.0f; eye[1] = 0.0f; eye[2] = 1.0f;
        target[0] = 0.0f; target[1] = 0.0f; target[2] = 0.0f;
        up[0] = 0.0f; up[1] = 1.0f; up[2] = 0.0f;
    }
    ImGui::End();

    Camera* camera = scene->getCamera();
    camera->setEye(glm::vec3(eye[0], eye[1], eye[2]));
    camera->setTarget(glm::vec3(target[0], target[1], target[2]));
    camera->setUp(glm::vec3(up[0], up[1], up[2]));
    camera->resetView();
}

void Gui::renderRotationSettings() {
    static glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f);
    static float angle = 0.0f;
    static float limit = 1.0f;
    static const char* precision = "%.1f";

    ImGui::Begin("Rotation settings");

    ImGui::SliderFloat("axis_x", &axis.x, -limit, limit, precision, 0);
    ImGui::SliderFloat("axis_y", &axis.y, -limit, limit, precision, 0);
    ImGui::SliderFloat("axis_z", &axis.z, -limit, limit, precision, 0);
    ImGui::SliderFloat("angle", &angle, -2.0, 2.0f, precision, 0);

    if (ImGui::Button("Reset to default")) {
        axis = glm::vec3(0.0f, 1.0f, 0.0f);
        angle = 0.0f;
    }

    if (ImGui::Button("Apply rotation")) {
        scene->setObjectRotation(glm::normalize(axis), angle * 3.14159f);
    }

    ImGui::End();
}

void Gui::renderSimulationSettings() {
    static float time_step = 0.2f;
    static float elasticity = 0.5f;
    static float slipperiness = 0.5f;
    static float vertexFace = 0.3f;
    static float edgeEdge = 0.3f;
    static float inverseMass = 0.01f;
    static float gravityStrength = 5.0f;
    static float angularResistance = 0.0f;
    static float gravityTorque = 1.0f;
    static const char* precision = "%.3f";

    ImGui::Begin("Simulation settings");
 
    bool ed = CollisionHandler::enable_debug;
    const char* debug_text = ed ? "Disable Debug" : "Enable Debug";
    if(ImGui::Button(debug_text)) {
        CollisionHandler::enable_debug = !ed;
    }
    bool ei = CollisionHandler::enable_interactions;
    const char* interaction_text = ei ? "Disable interactions" : "Enable interactions";
    if (ImGui::Button(interaction_text)) {
        CollisionHandler::enable_interactions = !ei;
    }
    if (ImGui::Button("Rollback simulation")) {
        scene->rollbackSimulation();
    }

    if (ImGui::SliderFloat("TimeStep", &time_step, 0.0f, 0.2f, precision, 0)) {
        scene->resetSimulationEvents(time_step);
    }
    if (ImGui::SliderFloat("Elasticity", &elasticity, 0.0f, 1.0f, precision, 0)) {
        CollisionHandler::elasticity = elasticity;
    }
    if (ImGui::SliderFloat("Slipperiness", &slipperiness, 0.0f, 1.0f, precision, 0)) {
        CollisionHandler::slipperiness = slipperiness; slipperiness;
    }
    if (ImGui::SliderFloat("InverseMass", &inverseMass, 0.0001f, 0.1f, "%.4f", 0)) {
        for (SceneObject* s : scene->getSceneObjects()) {
            RigidBody::BodyData* b = &s->getBodyStateSolver()->Body;
            CuboidCollider* cc = dynamic_cast<CuboidCollider*>(s->getCollider());
            if (cc != nullptr) {
                float w = cc->getWidth();
                float h = cc->getHeight();
                float l = cc->getLength();

                b->invMass = inverseMass;

                b->Ibody = 1.0f / (12.0f * inverseMass) * glm::mat3(
                    glm::vec3(h * h + l * l, 0.0f, 0.0f),
                    glm::vec3(0.0f, w * w + l * l, 0.0f),
                    glm::vec3(0.0f, 0.0f, w * w + h * h)
                );

                b->Ibodyinv = glm::inverse(b->Ibody);
            }

            SphereCollider* sc = dynamic_cast<SphereCollider*>(s->getCollider());
            if (sc != nullptr) {
                b->invMass = inverseMass;
            }
        }
    }
    if (ImGui::SliderFloat("GravityStrength", &gravityStrength, 0.0f, 10.0f, precision, 0)) {
        for (SceneObject* s : scene->getSceneObjects()) {
            if (s->name != "terrain") {
                s->getBodyStateSolver()->Body.force = glm::vec3(0.0f, -1.0f, 0.0f) * gravityStrength;
            }
        }
    }

    //if (ImGui::SliderFloat("GravityTorque", &gravityTorque, 0.0f, 10.0f, precision, 0)) {
    //    RigidBody::BodyStateSolver::gravityTorque = gravityTorque;
    //}
    /*if (ImGui::SliderFloat("VertexFace", &vertexFace, 0.1f, 1.0f, precision, 0)) {
        CuboidCollider::vertexFaceThreshold = vertexFace;
    }
    if (ImGui::SliderFloat("EdgeEdge", &edgeEdge, 0.1f, 1.0f, precision, 0)) {
        CuboidCollider::edgeEdgeThreshold = edgeEdge;
    }*/

    /*if (ImGui::Button("Apply impulse")) {
        CollisionHandler::apply_impulse = true;
    }
    if (ImGui::Button("Apply displacement")) {
        CollisionHandler::apply_displacement = true;
    }*/

    /*if (ImGui::Button("Simulate forward")) {
        scene->updateSimulation(time_step);
    }
    if (ImGui::Button("Simulate backward")) {
        scene->updateSimulation(-time_step);
    }*/

    ImGui::End();
}

void Gui::renderTerrainSettings() {
    static float amplitude = 0.0f;
    static float frequency = 0.0f;
    static float phase = 0.0f;
    static int iterations = 0;
    static const char* precision = "%.2f";

    if (scene->getTerrain() == nullptr) {
        return;
    }

    ImGui::Begin("Terrain settings");
    ImGui::SliderFloat("amplitude", &amplitude, 0.0f, 1.0f, precision, 0);
    ImGui::SliderFloat("frequency", &frequency, 0.0f, 50.0f, precision, 0);
    ImGui::SliderFloat("phase", &phase, 0.0f, 25.0f, precision, 0);
    ImGui::SliderInt("iterations", &iterations, 1, 32);
    if (ImGui::Button("Reset to default")) {
        amplitude = 0.0f; phase = 0.1f; iterations = 3;
    }
    ImGui::End();

    scene->getTerrain()->setParameters(amplitude, frequency, phase, iterations);
}

void Gui::renderTesselationPicker() {
    static float min_tess = 64.0f;
    static float max_tess = 64.0f;
    static float min_dist = 0.0f;
    static float max_dist = 50.0f;

    if (scene->getTerrain() == nullptr) {
        return;
    }

    ImGui::Begin("Tesselation parameters");
    ImGui::SliderFloat("min_tess", &min_tess, 1.0f, 64.0f, "%.0f", 0);
    ImGui::SliderFloat("max_tess", &max_tess, 1.0f, 64.0f, "%.0f", 0);
    ImGui::SliderFloat("min_dist", &min_dist, 0.0f, 10.0f, "%.1f", 0);
    ImGui::SliderFloat("max_dist", &max_dist, 0.0f, 100.0f, "%.1f", 0);
    ImGui::End();

    scene->getTerrain()->setTesselationParameters((unsigned int)min_tess, (unsigned int)max_tess, min_dist, max_dist);
}

void Gui::renderLightingPicker() {
    static float ambientWeight = 0.159f;
    static float diffuseWeight = 0.786f;
    static float specularWeight = 1.274f;
    static float lightPowerDens[3] = { 1.0f, 1.0f, 1.0f };

    static float albedo[3] = { 0.0f, 0.0f, 0.0f };
	static float metallic = 0.0f;
	static float roughness = 0.0f;  
	static float ao = 1.0f;

    ImGui::Begin("Lighting parameters");
    ImGui::SliderFloat("ambientWeight", &ambientWeight, 0.0f, 3.0f, "%.3f", 0);
    ImGui::SliderFloat("diffuseWeight", &diffuseWeight, 0.0f, 3.0f, "%.3f", 0);
    ImGui::SliderFloat("specularWeight", &specularWeight, 0.0f, 3.0f, "%.3f", 0);
    ImGui::SliderFloat3("lightPowerDens", lightPowerDens, 0.0f, 100.0f, "%.2f", 0);
	ImGui::SliderFloat3("albedo", albedo, 0.0f, 5.0f, "%.2f", 0);
	ImGui::SliderFloat("metallic", &metallic, 0.0f, 1.0f, "%.2f", 0);
	ImGui::SliderFloat("roughness", &roughness, 0.0f, 1.0f, "%.2f", 0);
	ImGui::SliderFloat("ao", &ao, 0.0f, 1.0f, "%.2f", 0);
    ImGui::End();

    GpuProgram* phong = scene->getProgram(Scene::PHONG);
    phong->bind();
    phong->setFloat("w_amb", ambientWeight);
    phong->setFloat("w_dif", diffuseWeight);
    phong->setFloat("w_spe", specularWeight);

    GpuProgram* pbr = scene->getProgram(Scene::PBR);
    pbr->bind();
	pbr->setVec3("albedo", glm::vec3(albedo[0], albedo[1], albedo[2]));
	pbr->setFloat("metallic", metallic);
	pbr->setFloat("roughness", roughness);
	pbr->setFloat("ao", ao);
    pbr->setFloat("w_amb", ambientWeight);
    pbr->setFloat("w_dif", diffuseWeight);
    pbr->setFloat("w_spe", specularWeight);

    scene->getSun()->getLight()->powerDensity = glm::vec3(lightPowerDens[0], lightPowerDens[1], lightPowerDens[2]);
}

void Gui::initialize(GLFWwindow* window, Scene* _scene) {
    ImGui::CreateContext();
    io = &ImGui::GetIO(); (void)*io;
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io->WantCaptureKeyboard = false;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    scene = _scene;
}

void Gui::render(int sceneWidth, int windowWidth, int windowHeight) {
    glViewport(sceneWidth, 0, windowWidth - sceneWidth, windowHeight);
    renderRectangle(glm::vec3(0.2f, 0.2f, 0.3f));

    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    renderObjectPicker();
    renderPrimitivePicker();
    //renderRotationSettings();
    renderSimulationSettings();
    renderTerrainSettings();
    //renderTesselationPicker();
    renderFrameRateBox();
    renderLightingPicker();
}
