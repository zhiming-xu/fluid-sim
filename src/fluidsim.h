#ifndef WATERSYSTEM_H
#define WATERSYSTEM_H

#include <vector>

#include "particlesystem.h"

const float c_pi = 3.14159265358979323846f;

const float TANK_STANDARD_MINUS = -1.0f;
const float TANK_STANDARD_PLUS = 1.0f;
#ifdef threeD
const float PARTICLE_SPACING = 0.24f;
#else
const float PARTICLE_SPACING = 0.08f;
#endif
const float CELL_SPACING = 0.08f;

const float TANK_START_X = TANK_STANDARD_MINUS;
const float TANK_END_X = TANK_STANDARD_PLUS;
const float TANK_START_Y = TANK_STANDARD_MINUS;
const float TANK_END_Y = 0.0f;
const float TANK_START_Z = TANK_STANDARD_MINUS;
const float TANK_END_Z = 0.0f;

const float GRID_START_X = TANK_STANDARD_MINUS - CELL_SPACING;
const float GRID_END_X = TANK_STANDARD_PLUS + CELL_SPACING;
const float GRID_START_Y = TANK_STANDARD_MINUS - CELL_SPACING;
const float GRID_END_Y = 1.0f;
const float GRID_START_Z = TANK_STANDARD_MINUS - CELL_SPACING;
const float GRID_END_Z = 1.0f;

const float NUM_X_INDICES = (GRID_END_X - GRID_START_X) / CELL_SPACING;
const float NUM_Y_INDICES = (GRID_END_Y - GRID_START_Y) / CELL_SPACING;
const float NUM_Z_INDICES = (GRID_END_Z - GRID_START_Z) / CELL_SPACING;
#ifdef threeD
const float NUM_TOTAL_INDICES = NUM_X_INDICES * NUM_Y_INDICES * NUM_Z_INDICES;
#else
const float NUM_TOTAL_INDICES = NUM_X_INDICES * NUM_Y_INDICES;
#endif
const float NEIGHBOR_RADIUS = CELL_SPACING;

const float GRAVITY = -9.8f;
const float MASS = 1.0f;
const float H_KERNEL = 1.5f;
const float K_GAS_CONSTANT = 0.01f;

const float MU = 0.0000001f;
const float REST_DENSITY = 0.001f;
const float SINGLE_PARTICLE_DENSITY = 10.f;
extern float DROP_RADIUS;

class FluidSim : public ParticleSystem
{
public:
	enum KernelType {
	  Poly6,
	  Spiky,
	  Viscosity
	};

    FluidSim();
    std::vector<Vector3f> compute_force(std::vector<Vector3f> state) override;
    void draw(GLProgram&);
	
private:
    //list of state indices
    std::vector<std::vector<int>> systemGrid;
    void set_grid(const std::vector<std::vector<int>> &newGrid) { systemGrid = newGrid; };
    void clear_grid();
    void update_grid(std::vector<Vector3f> state);
    int pos_to_grid_index(float x, float y, float z);
	std::vector<int> get_neighbors(int i, std::vector<Vector3f> state);
	float compute_kernel(KernelType type, float r);
	float compute_density(int i, std::vector<Vector3f> state, std::vector<int> nearestParticles);
	Vector3f compute_pressure(int i, std::vector<Vector3f> state, std::vector<int> nearestParticles,
                              std::vector<float> particleDensity);
	Vector3f compute_viscocity(int i, std::vector<Vector3f> state, std::vector<int> nearestParticles,
                               std::vector<float> particleDensity);
	Vector3f compute_external();
};

#endif
