#include "Scene.hpp"

void Scene::initializeEvents() {
	using namespace KeyboardInput;

	cameraTransformationEventHandler
		.addEvent(Event<Camera, int>( new glfw::Key(GLFW_KEY_S), &Camera::moveUp, -1 ));
	cameraTransformationEventHandler
		.addEvent(Event<Camera, int>( new glfw::Key(GLFW_KEY_W), &Camera::moveUp, 1));
	cameraTransformationEventHandler
		.addEvent(Event<Camera, int>( new glfw::Key(GLFW_KEY_A), &Camera::moveRight, -1));
	cameraTransformationEventHandler
		.addEvent(Event<Camera, int>( new glfw::Key(GLFW_KEY_D), &Camera::moveRight, 1));
	cameraTransformationEventHandler
		.addEvent(Event<Camera, int>( new glfw::Key(GLFW_KEY_LEFT_SHIFT), &Camera::moveForward, -1));
	cameraTransformationEventHandler
		.addEvent(Event<Camera, int>( new glfw::Key(GLFW_KEY_LEFT_CONTROL), &Camera::moveForward, 1));
	cameraTransformationEventHandler
		.addEvent(Event<Camera, int>( new glfw::Key(GLFW_KEY_E), &Camera::rotate, -1));
	cameraTransformationEventHandler
		.addEvent(Event<Camera, int>( new glfw::Key(GLFW_KEY_Q), &Camera::rotate, 1));

	resetObjectEvents(0.05f);
	resetSimulationEvents(0.2f);
}

void Scene::initializeResources() {
	GpuProgram* phong_shader = new GpuProgram("phong_shader");
	phong_shader->readVertexSource("main/app/shaders/phongVertex.shader");
	phong_shader->readFragmentSource("main/app/shaders/phongFragment.shader");
	phong_shader->createProgram();
	programs.push_back(phong_shader);

	GpuProgram* texture_shader = new GpuProgram("texture_shader");
	texture_shader->readFragmentSource("main/app/shaders/textureFragment.shader");
	texture_shader->readVertexSource("main/app/shaders/textureVertex.shader");
	texture_shader->createProgram();
	programs.push_back(texture_shader);

	TesselationProgram* patch_shader = new TesselationProgram("patch_shader");
	patch_shader->readFragmentSource("main/app/shaders/patchFragment.shader");
	patch_shader->readVertexSource("main/app/shaders/patchVertex.shader");
	patch_shader->readTesselationControlSource("main/app/shaders/patchTesselationControl.shader");
	patch_shader->readTesselationEvaluationSource("main/app/shaders/patchTesselationEvaluation.shader");
	patch_shader->createProgram();
	programs.push_back(patch_shader);

	GpuProgram* assimp_model_shader = new GpuProgram("assimp_model_shader");
	assimp_model_shader->readVertexSource("main/app/shaders/assimpVertex.shader");
	assimp_model_shader->readFragmentSource("main/app/shaders/assimpFragment.shader");
	assimp_model_shader->createProgram();
	programs.push_back(assimp_model_shader);

	Texture* texture1 = new Texture("boxTexture");
	texture1->load("main/app/images/box1.jpg");
	textures.push_back(texture1);

	Texture* texture2 = new Texture("floorTexture");
	texture2->load("main/app/images/floor.jpg");
	textures.push_back(texture2);

	Texture* texture3 = new Texture("sunTexture");
	texture3->load("main/app/images/sun.jpg");
	textures.push_back(texture3);

	Texture* texture4 = new Texture("pinkTexture");
	texture4->load("main/app/images/pink.jpg");
	textures.push_back(texture4);

	Texture* texture5 = new Texture("dirtTexture");
	texture5->load("main/app/images/dirt.jpg");
	textures.push_back(texture5);

	Texture* texture6 = new Texture("ballTexture");
	texture6->load("main/app/images/football.jpg");
	textures.push_back(texture6);

	materials.push_back(new Material(
		glm::vec3(0.4f, 0.0f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.2f, 0.2f, 0.2f),
		10.0f
	));

	materials.push_back(new Material(
		glm::vec3(0.0f, 0.4f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.2f, 0.2f, 0.2f),
		10.0f
	));

	materials.push_back(new Material(
		glm::vec3(0.4f, 0.4f, 0.4f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.2f, 0.2f, 0.2f),
		10.0f
	));

	materials.push_back(new Material(
		glm::vec3(0.0f, 0.2f, 0.2f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.2f, 0.2f, 0.2f),
		10.0f
	));
}

Scene::Scene() {
	object_index = 0;
	move_camera = true;

	camera = new Camera();

	initializeResources();
	initializeEvents();
}

void Scene::getNextObject() {
	if (++object_index == sceneObjects.size()) {
		object_index = 0;
	}
}

void Scene::setPrimitiveType(unsigned int type) {
	for (SceneObject* g : sceneObjects)
		g->getMesh()->setPrimitive(type);
}

void Scene::setObjectRotation(glm::vec3 axis, float angle) {
	glm::mat4 R = Transformation::makeRotationMatrix(axis, angle);
	sceneObjects[object_index]->setRotation(R);
}

void Scene::resetObjectEvents(float speed) {
	using namespace KeyboardInput;

	objectTransformationEventHandler.removeAllEvents();

	objectTransformationEventHandler.addEvent(Event<SceneObject, glm::vec3>(
		new glfw::Key(GLFW_KEY_W), &SceneObject::translate, glm::vec3(0.0f, speed, 0.0f)));
	objectTransformationEventHandler.addEvent(Event<SceneObject, glm::vec3>(
		new glfw::Key(GLFW_KEY_S), &SceneObject::translate, glm::vec3(0.0f, -speed, 0.0f)));
	objectTransformationEventHandler.addEvent(Event<SceneObject, glm::vec3>(
		new glfw::Key(GLFW_KEY_D), &SceneObject::translate, glm::vec3(speed, 0.0f, 0.0f)));
	objectTransformationEventHandler.addEvent(Event<SceneObject, glm::vec3>(
		new glfw::Key(GLFW_KEY_A), &SceneObject::translate, glm::vec3(-speed, 0.0f, 0.0f)));
	objectTransformationEventHandler.addEvent(Event<SceneObject, glm::vec3>(
		new glfw::Key(GLFW_KEY_LEFT_SHIFT), &SceneObject::translate, glm::vec3(0.0f, 0.0f, -speed)));
	objectTransformationEventHandler.addEvent(Event<SceneObject, glm::vec3>(
		new glfw::Key(GLFW_KEY_LEFT_CONTROL), &SceneObject::translate, glm::vec3(0.0f, 0.0f, speed)));
}

void Scene::resetSimulationEvents(float delta_time) {
	using namespace KeyboardInput;

	simulationEventHandler.removeAllEvents();

	simulationEventHandler.addEvent(Event<Scene, float>(
		new glfw::Key(GLFW_KEY_LEFT), &Scene::updateSimulation, -delta_time));

	simulationEventHandler.addEvent(Event<Scene, float>(
		new glfw::Key(GLFW_KEY_RIGHT), &Scene::updateSimulation, delta_time));
}

void Scene::rollbackSimulation() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> momentumDist(-10.0f, 10.0f);
	std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * 3.14159f);
	std::uniform_real_distribution<float> axisDist(-1.0f, 1.0f);

	for (SceneObject* s : sceneObjects) {
		s->resetBodyState();
	}

	for (int i = 2; i < sceneObjects.size(); ++i) {
		SceneObject* s = sceneObjects[i];
		glm::vec3 axis = glm::normalize(glm::vec3(axisDist(gen), axisDist(gen), axisDist(gen)));
		s->setRotation(Transformation::makeRotationMatrix(axis, angleDist(gen)));
		s->getBodyStateSolver()->Body.P = glm::vec3(momentumDist(gen), momentumDist(gen), momentumDist(gen));
		s->getBodyStateSolver()->makeStateInitial();
	}
}

void Scene::create() {
	sun = new Sun(2.0f);
	sun->name = "sun";
	sun->setMeshResources(programs[TEX], textures[SUN], materials[BASIC]);
	sun->translate(glm::vec3(0.0f, 25.0f, -20.0f));
	sun->create();
	sceneObjects.push_back(sun);

	terrain = new Terrain(1.0f, 4);
	terrain->name = "terrain";
	terrain->getTransformation()->setScaleMatrix(glm::vec3(400.0f, 50.0f, 400.0f));
	terrain->translate(glm::vec3(-200.0f, -20.0f, -200.0f));
	terrain->setMeshResources(programs[TESS], textures[DIRT], materials[BASIC]);
	terrain->getBodyStateSolver()->makeStateInitial();
	terrain->create();
	sceneObjects.push_back(terrain);

	// IRON BOX MODELS TOWER
	/*for (int i = 0; i < 5; i++) {
		IronBox* ironBox = new IronBox(1.0f - i * 0.2f, false);
		ironBox->name = "iron_box";
		ironBox->setMeshResources(programs[ASIM], nullptr, materials[BASIC]);
		ironBox->translate(glm::vec3(0.0f, 8.0f * i - 0.0f, 0.0f));
		ironBox->getBodyStateSolver()->makeStateInitial();
		ironBox->create();
		sceneObjects.push_back(ironBox);
	}*/

	// IRON BOX MODELS RANDOMLY
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> momentumDist(-25.0f, 25.0f); 
	std::uniform_real_distribution<float> heightDist(-3.0f, 3.0f); 
	std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * 3.14159f);
	std::uniform_real_distribution<float> axisDist(-1.0f, 1.0f); 
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			IronBox* ironBox = new IronBox(1.0f - i * 0.2f, false);
			ironBox->name = "iron_box";
			ironBox->setMeshResources(programs[ASIM], nullptr, materials[BASIC]);
			ironBox->translate(glm::vec3(12.0f * i, 15.0f + heightDist(gen), 12.0f * j));
			glm::vec3 axis = glm::normalize(glm::vec3(axisDist(gen), axisDist(gen), axisDist(gen)));
			ironBox->setRotation(Transformation::makeRotationMatrix(axis, angleDist(gen)));
			ironBox->getBodyStateSolver()->Body.P = glm::vec3(momentumDist(gen), momentumDist(gen), momentumDist(gen));
			ironBox->getBodyStateSolver()->makeStateInitial();
			sceneObjects.push_back(ironBox);
		}
	}

	// CPU MODELS RANDOMLY
	/*for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			Box* b = new Box(5.0f, 5.0f, 5.0f, false);
			b->setMeshResources(programs[PHONG], textures[BOX], materials[(i + j) % 3]);
			b->translate(glm::vec3(12.0f * i, 15.0f + heightDist(gen), 12.0f * j));

			glm::vec3 axis = glm::normalize(glm::vec3(axisDist(gen), axisDist(gen), axisDist(gen)));
			b->setRotation(Transformation::makeRotationMatrix(axis, angleDist(gen)));

			b->getBodyStateSolver()->Body.P = glm::vec3(momentumDist(gen), momentumDist(gen), momentumDist(gen));
			b->getBodyStateSolver()->makeStateInitial();
			b->create();
			sceneObjects.push_back(b);
		}
	}*/

	//CPU BOX MODELS TOWER
	/*for (int i = 1; i < 6; ++i) {
		float  dim = 10.0f - 1.2f * i;
		Box* b = new Box(dim, dim, dim, false);
		b->setMeshResources(programs[PHONG], textures[BOX], materials[i % 3]);
		b->translate(glm::vec3(10.0f, 9.0f * i, 10.0f));
		b->getBodyStateSolver()->makeStateInitial();
		b->create();
		sceneObjects.push_back(b);
	}*/
}

void Scene::draw() const {
	for (SceneObject* s : sceneObjects)
		s->draw(*camera);

	if (sceneObjects[object_index] != terrain) {
		sceneObjects[object_index]->getMesh()->drawMeshFrame(*camera);
	}	
}

void Scene::updateSimulation(float delta_time) {
	for(SceneObject* sceneObject : sceneObjects) {
		sceneObject->updateBodyState(delta_time);
		sceneObject->updateTransformations();
	}
}

void Scene::update(GLFWwindow* window) {
	if (move_camera) {
		if(cameraTransformationEventHandler.handleEvents(window, camera)) {
			camera->resetView();
		}
	}
	else {
		objectTransformationEventHandler.handleEvents(window, sceneObjects[object_index]);
	}

	for (SceneObject* s : sceneObjects) {
		s->update(sceneObjects);
	}

	CollisionHandler::handleCollisions(sceneObjects, *camera);
	simulationEventHandler.handleEvents(window, this);
}

Scene::~Scene() {
	for(SceneObject* s : sceneObjects) {
		s->getMesh()->setProgram(nullptr);
		s->getMesh()->setTexture(nullptr);
		s->getMesh()->setMaterial(nullptr);
	}

	delete camera;
}
