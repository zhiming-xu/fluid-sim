#include <cassert>
#include "camera.h"
#include <iostream>
#include "fluidsim.h"
#include "vertexrecorder.h"
// to render in 3D, define this macro,
// note: 3D rendering on CPU can be rather slow
#define threeD

float DROP_RADIUS;

using namespace std;

int FluidSim::pos_to_grid_index(float x, float y, float z) {
    int xIndex = (x - GRID_START_X) / CELL_SPACING;
    int yIndex = (y - GRID_START_Y) / CELL_SPACING;
    int zIndex = (z - GRID_START_Z) / CELL_SPACING;
#ifdef threeD
    return (xIndex * NUM_Y_INDICES + yIndex) * NUM_Z_INDICES + zIndex;
#else
    return (xIndex * NUM_Y_INDICES) + yIndex;
#endif
}

void FluidSim::clear_grid() {
    for(auto &grid: systemGrid)
        grid = std::vector<int>();
}

float getDis(float x, float y, float z, float cx, float cy, float cz) {
  return sqrt((pow(x-cx,2) + pow(y-cy,2) + pow(z-cz,2)));
}

float getMin(float a, float b, float c) {
  float d = min(a,b);
  float e = min(a,c);
  return min(d,e);
}

bool inCircle(float x, float y, float z) {
  float cx = 0;//(TANK_END_X - TANK_START_X)/2;
  float cy = -(TANK_END_Y - TANK_START_Y)/2;
  float cz = -(TANK_END_Z - TANK_START_Z)/2;

  float radius = getMin(TANK_END_X - TANK_START_X, TANK_END_Y - TANK_START_Y, TANK_END_Z - TANK_START_Z)/2;

  if (getDis(x,y,z,cx,cy,cz) < radius)
    return true;
  else
    return false;
}

FluidSim::FluidSim()
{
    // single particle that is dropped
    DROP_RADIUS = .04;
    vector<Vector3f> initialState;  // it is a tuple, (position, speed)
    vector<vector<int>> initialGrid;
    
    for (float x = GRID_START_X; x < GRID_END_X; x += CELL_SPACING)
        for (float y = GRID_START_Y; y < GRID_END_Y; y += CELL_SPACING)
#ifdef threeD
            for (float z = GRID_START_Z; z < GRID_END_Z; z += CELL_SPACING)
#endif
                initialGrid.push_back(vector<int>());
    int curStateIndex = 0;
    // particles that make up the water into which particle falls
    float z = .0f;
    for (float x = TANK_START_X; x < TANK_END_X; x += PARTICLE_SPACING)
        for (float y = TANK_START_Y; y < TANK_END_Y; y += PARTICLE_SPACING)
#ifdef threeD
            for (z = TANK_START_Z; z < TANK_END_Z; z += PARTICLE_SPACING)
#endif
            {

              if (!inCircle(x,y,z)) continue;
            //populate the initial grid with current index
            int gridIndex = FluidSim::pos_to_grid_index(x, y, z);
            Vector3f position = Vector3f(x, y + 2, z);
            initialState.push_back(position);
            initialState.emplace_back(Vector3f(0.0f, 0.0f, 0.0f));
            initialGrid[gridIndex].push_back(curStateIndex);
            curStateIndex++;
    }
    set_state(initialState);
    set_grid(initialGrid);
}

void FluidSim::update_grid(std::vector<Vector3f> state){
    clear_grid();
    for (int stateIndex = 0; stateIndex <(int) state.size(); stateIndex += 2) {
        Vector3f pos = state[stateIndex];
        int gridIndex = FluidSim::pos_to_grid_index(pos.x(), pos.y(), pos.z());
        // out of grid
        if (gridIndex < 0 || gridIndex >= NUM_TOTAL_INDICES)
            continue;
        // mapping from grid index to state index (position, velocity)
        else
	        systemGrid[gridIndex].push_back(stateIndex/2);
    }
}

std::vector<int> FluidSim::get_neighbors(int i, std::vector<Vector3f> state) {
    Vector3f iPos = state[2 * i];
    std::vector<int> neighboringIndices = vector<int>();
    float z = .0f;
    for (float x = iPos.x() - CELL_SPACING; x < iPos.x() + CELL_SPACING*2; x += CELL_SPACING)
        for (float y = iPos.y() - CELL_SPACING; y < iPos.y() + CELL_SPACING*2; y += CELL_SPACING)
#ifdef threeD
            for (z = iPos.z() - CELL_SPACING; z < iPos.z() + CELL_SPACING*2; z += CELL_SPACING)
#endif
            {
                int gridIndex = pos_to_grid_index(x, y, z);
                if (gridIndex >= 0 && gridIndex < NUM_TOTAL_INDICES) {
                for (auto neighborIndex : systemGrid[gridIndex]) {
                    Vector3f neighborDisplacement = state[2*neighborIndex] - iPos;
                    if (neighborDisplacement.abs() <= NEIGHBOR_RADIUS && neighborDisplacement.abs() != 0)
                        neighboringIndices.push_back(neighborIndex);
            }
        }
	}
    return neighboringIndices;
}

std::vector<Vector3f> FluidSim::compute_force(std::vector<Vector3f> state) {
    FluidSim::update_grid(state);

    std::vector<Vector3f> f;
    Vector3f fGravity = Vector3f(0.0, MASS * GRAVITY, 0.0);
    std::vector<std::vector<int>> particleNeighbors;
    std::vector<float> particleDensity;

    // first pass: calculate density of all particles
    for (int i = 0; i < (int) state.size() / 2; ++i) {
        auto nearestParticles = FluidSim::get_neighbors(i, state);
        float density = compute_density(i, state, nearestParticles);

        particleNeighbors.push_back(nearestParticles);
        particleDensity.push_back(density);
    }
    // second pass: calculate forces
    for (int i = 0; i < (int) state.size() / 2; ++i) {
        Vector3f velocity = state[2 * i + 1];
        std::vector<int> nearestParticles = particleNeighbors.at(i);
        Vector3f fPressure = compute_pressure(i, state, nearestParticles, particleDensity);
        Vector3f fViscosity = compute_viscocity(i, state, nearestParticles, particleDensity);
        Vector3f fExternal = compute_external();
        Vector3f totalForce = 1000 * fPressure + fViscosity + fGravity + fExternal;
        Vector3f acceleration = totalForce / MASS / 10;
        f.push_back(velocity);
        f.push_back(acceleration);
    }
    return f;
}

void FluidSim::draw(GLProgram& gl)
{
    const Vector3f SPHERE_COLOR(0., 0., 0.);
    gl.updateMaterial(SPHERE_COLOR);
    vector<Vector3f> currentState = get_state();
    for (int i=0; i<(int) currentState.size()/2; ++i) {
      gl.updateModelMatrix(Matrix4f::translation(get_position_i(currentState, i)));
      drawSphere(DROP_RADIUS, 10, 10);
    }
}

float FluidSim::compute_kernel(FluidSim::KernelType type, float r) {
  if (r < 0 || r > H_KERNEL) {
    return 0;
  }

  float numerator = 0;
  float denominator = 1;

  switch(type)
  {
      case Poly6:{
          numerator = 315 * pow( pow(H_KERNEL, 2) - pow(r, 2), 3);
          denominator = 64 * c_pi * pow(H_KERNEL, 9);
          break;
      }
      case Spiky:{
          float term1 = -pow(r, 3) / (2 * pow(H_KERNEL, 3));
          float term2 = pow(r, 2) / pow(H_KERNEL, 2);
          float term3 = H_KERNEL / (2 * r);
          numerator = 15 * (term1 + term2 + term3 - 1);
          denominator = 2 * c_pi * pow(H_KERNEL, 3);
          break;
      }
      case Viscosity:{
          numerator = 15 * pow(H_KERNEL - r, 3);
          denominator = c_pi * pow(H_KERNEL, 6);
          break;
      }
      default:
          std::cout<<"\033[31mKernel type unrecognized!\033[0m"<<endl;
  }
  return numerator / denominator;
}

float FluidSim::compute_density(int i, std::vector<Vector3f> state, std::vector<int> nearestParticles) {
    float density = SINGLE_PARTICLE_DENSITY;
    Vector3f x_i = get_position_i(state, i);

    for (int j = 0; j<(int) nearestParticles.size(); ++j) {
      float index = nearestParticles.at(j);
      Vector3f x_j = get_position_i(state, index);
      float r = (x_i - x_j).abs();
      float W = compute_kernel(Poly6, r);

      density += MASS * W;
  }

  return density;
}

Vector3f FluidSim::compute_pressure(int i, std::vector<Vector3f> state, std::vector<int> nearestParticles,
                                    std::vector<float> particleDensity) {
  Vector3f force = Vector3f();
  Vector3f x_i = get_position_i(state, i);
  float density_i = particleDensity.at(i);

  for (int j=0; j<(int) nearestParticles.size(); ++j) {
    Vector3f x_j = get_position_i(state, nearestParticles.at(j));
    float density_j = particleDensity.at(j);
    Vector3f r_ij = x_i - x_j;
    float q_ij = r_ij.abs() / H_KERNEL;

    float numerator1 = density_i + density_j - 2 * REST_DENSITY;
    float numerator2 = pow((1 - q_ij), 2);
    force += MASS * numerator1 * numerator2 * r_ij / (density_j * q_ij);
  }

  force = force * K_GAS_CONSTANT / (c_pi * pow(H_KERNEL, 4));
  return force;
}

Vector3f FluidSim::compute_viscocity(int i, std::vector<Vector3f> state,
                                     std::vector<int> nearestParticles,
                                     std::vector<float> particleDensity)
{
  Vector3f force = Vector3f();
  Vector3f x_i = get_position_i(state, i);
  Vector3f v_i = get_velocity_i(state, i);

  for (int j=0; j<(int) nearestParticles.size(); ++j) {
    float index = nearestParticles.at(j);
    Vector3f x_j = get_position_i(state, index);
    Vector3f v_j = get_velocity_i(state, index);
    float density_j = particleDensity.at(index);
    Vector3f r_ij = x_i - x_j;
    float q_ij = r_ij.abs() / H_KERNEL;

    force += MASS * (v_i - v_j) * (1 - q_ij) / density_j;
  }

  force = force * 40 * MU / (c_pi * pow(H_KERNEL, 4));
  return force;
}

Vector3f FluidSim::compute_external() {
  return Vector3f();
}
