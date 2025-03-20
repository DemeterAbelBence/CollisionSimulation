#pragma once

#include <random>

#include "src/util/gl/GpuProgram.hpp"
#include "src/util/gl/VertexArray.hpp"
#include "src/util/glfw/KeyboardInput.hpp"
#include "src/util/glfw/Time.hpp"
#include "src/util/Camera.hpp"
#include "src/util//asim/asim.hpp"
#include "src/util/tex/tex.hpp"

#include "src/visual/drawdata/Material.hpp"
#include "src/visual/Mesh.hpp"

#include "src/collision/CollisionHandler.hpp"

#include "src/objects/Box.hpp"
#include "src/objects/Sun.hpp"
#include "src/objects/Terrain.hpp"
#include "src/objects/Sphere.hpp"
#include "src/objects/IronBox.hpp"
#include "src/objects/Sponza.hpp"

class Scene {

public:
	enum ProgramId { TEX, TESS, PHONG, PBR };
	enum MatId { RED, GREEN, BLUE, BASIC };
	enum TexId { SUN_TEX, TER_AMB, TER_NOR };

private:
	Camera* camera;
	Sun* sun;
	Terrain* terrain;

	std::vector<SceneObject*> sceneObjects;
	std::vector<GpuProgram*> programs;
	std::vector<Material*> materials;

	std::vector<tex::Texture*> textures;

	KeyboardInput::EventHandler<Camera, int> cameraTransformationEventHandler;
	KeyboardInput::EventHandler<SceneObject, glm::vec3> objectTransformationEventHandler;
	KeyboardInput::EventHandler<Scene, float> simulationEventHandler;

private:
	unsigned int object_index;
	bool move_camera;

private:
	void initializeEvents();
	void initializeResources();

public:
	Scene();

	Camera* getCamera() const { return camera; }
	void setMoveCamera(bool value) { move_camera = value; }
	bool getMoveCamera() const { return move_camera; }
	Terrain* getTerrain() const { return terrain; }
	Sun* getSun() const { return sun; }
	GpuProgram* getProgram(ProgramId programId) { return programs[programId]; }
	std::vector<SceneObject*> getSceneObjects() const { return sceneObjects; }
	SceneObject* getCurrentSceneObject() { return sceneObjects[object_index]; }

	void getNextObject();
	void setPrimitiveType(unsigned int type);
	void setObjectRotation(glm::vec3 axis, float angle);
	void resetObjectEvents(float speed);
	void resetSimulationEvents(float delta_time);
	void rollbackSimulation();

	void create();
	void draw() const;
	void updateSimulation(float delta_time);
	void update(GLFWwindow* window);

	~Scene();	
};