#ifndef WATERSYSTEM_H
#define WATERSYSTEM_H

#include <vector>

#include "particlesystem.h"

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
