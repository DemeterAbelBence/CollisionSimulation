#pragma once

struct TerrainData {
	float amplitude;
	float frequency;
	float phase;
	unsigned int iterations;

	unsigned int minimal_tesselation = 0;
	unsigned int maximal_tesselation = 0;

	float minimal_distance = 0.0f;
	float maximal_distance = 0.0f;
};