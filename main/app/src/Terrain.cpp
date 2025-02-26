#include "Terrain.hpp"

Terrain::Terrain(float size, unsigned int rezolution) {
	terrainData = new TerrainData{
		0.05f,  // amplitude
		0.1f,   // frequency
		0.18f,  // phase
		6,      // iterations
		64,     // minimal_tesselation
		64,     // maximal_tesselation
		0.0f,   // minimal_distance
		50.0f   // maximal_distance
	};

	mesh = new PatchSurface(size, rezolution);
	collider = new TerrainCollider(terrainData);

	auto bodyData = RigidBody::BodyData{
		0.0f,					// InvMass
		glm::mat3(0.0f),        // Ibody
		glm::mat3(0.0f),        // Ibodyinv

		glm::vec3(0.0f),        // X 
		glm::mat3(1.0f),        // R 
		glm::vec3(0.0f),        // P
		glm::vec3(0.0f),        // L 

		glm::mat3(1.0f),        // Iinv 
		glm::vec3(0.0f),        // vel 
		glm::vec3(0.0f),        // omega 

		glm::vec3(0.0f, 0.0f, 0.0f),        // force
		glm::vec3(0.0f)                      // torque
	};
	bodyStateSolver = new RigidBody::BodyStateSolver(bodyData);

	transformation = new Transformation();
	mesh->setTransformation(transformation);
	collider->setTransformation(transformation);
}

void Terrain::setParameters(float a, float f, float p, float i) {
	terrainData->amplitude = a;
	terrainData->frequency = f;
	terrainData->phase = p;
	terrainData->iterations = i;
}

void Terrain::setTesselationParameters(unsigned int min_t, unsigned int max_t, float min_d, float max_d) {
	terrainData->minimal_tesselation = min_t;
	terrainData->maximal_tesselation = max_t;
	terrainData->minimal_distance = min_d;
	terrainData->maximal_distance = max_d;
}

void Terrain::resetBodyState() { 
	bodyStateSolver->rollbackToInitial();
	updateTransformations();
}

void Terrain::updateBodyState(float dt) { 
	bodyStateSolver->updateState(dt);
}

void Terrain::update(const std::vector<SceneObject*>& gameObjects) {
	GpuProgram* program = mesh->getProgram();

	program->bind();
	program->setFloat("amplitude_factor", terrainData->amplitude);
	program->setFloat("frequency_factor", terrainData->frequency);
	program->setFloat("phase_factor", terrainData->phase);
	program->setInt("iterations", terrainData->iterations);

	program->setFloat("MIN_DIST", terrainData->minimal_distance);
	program->setFloat("MAX_DIST", terrainData->maximal_distance);
	program->setInt("MIN_TESS", terrainData->minimal_tesselation);
	program->setInt("MAX_TESS", terrainData->maximal_tesselation);
}

Terrain::~Terrain() {
	TerrainCollider* terrainCollider = dynamic_cast<TerrainCollider*>(collider);
	terrainCollider->setTerrainData(nullptr);
	delete terrainCollider;

	delete mesh;
	delete collider;
	delete bodyStateSolver;
	delete transformation;
}


