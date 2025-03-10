#pragma once

#include <random>

#include "src/util/gl/GpuProgram.hpp"
#include "src/util/gl/TesselationProgram.hpp"
#include "src/util/gl/VertexArray.hpp"
#include "src/util/glfw/KeyboardInput.hpp"
#include "src/util/glfw/Time.hpp"
#include "src/visual/drawdata/Material.hpp"
#include "src/util/Camera.hpp"

#include "src/visual/Mesh.hpp"
#include "src/util//asim/asim.hpp"

#include "src/collision/CollisionHandler.hpp"

#include "src/Box.hpp"
#include "src/Sun.hpp"
#include "src/Terrain.hpp"
#include "src/Sphere.hpp"
#include "src/IronBox.hpp"

class Scene {

private:
	std::vector<SceneObject*> sceneObjects;

	Camera* camera;
	Sun* sun;
	Terrain* terrain;

	std::vector<GpuProgram*> programs;
	enum programId { PHONG, TEX, TESS, ASIM };

	std::vector<Texture*> textures;
	enum texId { BOX, FLOOR, SUN, PINK, DIRT, BALL };

	std::vector<Material*> materials;
	enum matId { RED, GREEN, BLUE, BASIC };

	KeyboardInput::EventHandler<Camera, int> cameraTransformationEventHandler;
	KeyboardInput::EventHandler<SceneObject, glm::vec3> objectTransformationEventHandler;
	KeyboardInput::EventHandler<Scene, float> simulationEventHandler;

	asim::AssimpModel* assimpModel;

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