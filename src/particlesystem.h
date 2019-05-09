#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <vector>
#include <vecmath.h>
#include <cstdint>


// helper for uniform distribution
float rand_uniform(float low, float hi);

struct GLProgram;
class ParticleSystem
{
public:
    virtual ~ParticleSystem() {}

    // for a given state, evaluate derivative f(X,t)
    virtual std::vector<Vector3f> compute_force(std::vector<Vector3f> state) = 0;

    // getter method for the system's state
    std::vector<Vector3f> get_state() { return current_state; };

    // setter method for the system's state
    void set_state(const std::vector<Vector3f> &new_state) { current_state = new_state; };

	static inline Vector3f get_position_i(std::vector<Vector3f> state, int i) { return state.at(i*2); };
	static inline Vector3f get_velocity_i(std::vector<Vector3f> state, int i) { return state.at(i*2 + 1); };

 protected:
    std::vector<Vector3f> current_state;
};

/* GLProgram is a helper for updating uniform variables.
   Before drawing geometry, update the model matrix and diffuse color.
*/
class Camera;
struct GLProgram {
    // constructor
    GLProgram(uint32_t program_light, uint32_t program_color, Camera* camera);

    // Update the model matrix. View and projection matrix
    // are read from the camera.
	void updateModelMatrix(Matrix4f M) const;

    // Update material properties.
    // - The one argument version just sets the diffuse color
    // - With 2-3 arguments, also sets specular color
	void updateMaterial(Vector3f diffuseColor, 
        Vector3f ambientColor = Vector3f(-1, -1, -1),
        Vector3f specularColor = Vector3f(0, 0, 0),
        float shininess = 1.0f,
        float alpha = 1.0f) const;

    // Update lighting. Sets position and color of a single light source
    // in world space.
	void updateLight(Vector3f pos, Vector3f color = Vector3f(1, 1, 1)) const;

    void enableLighting();
    void disableLighting();

private:
    // member variables
    uint32_t active_program;
    uint32_t program_light;
    uint32_t program_color;
    const Camera* camera;
};
#endif
