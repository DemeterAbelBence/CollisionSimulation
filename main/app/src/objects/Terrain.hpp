#pragma once

#include "SceneObject.hpp"
#include "../visual/PatchSurface.hpp"
#include "../collision/TerrainCollider.hpp"
#include "../visual/drawdata/TerrainData.hpp"

class Terrain : public SceneObject {
private:
	TerrainData* terrainData;

public:
	Terrain(float size, unsigned int rezolution);

	void setParameters(float a, float f, float p, float i);
	void setTesselationParameters(unsigned int min_t, unsigned int max_t, float min_d, float max_d);

	void resetBodyState() override;
	void updateBodyState(float dt) override;
	void updateTerrainParameters();

	~Terrain();
};