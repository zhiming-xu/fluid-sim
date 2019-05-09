#ifdef __APPLE__
#include <OpenGL/gl3.h>
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <cassert>

#include "vertexrecorder.h"
#include "starter3_util.h"
#include "camera.h"
#include "timestepper.h"
#include "fluidsim.h"
// define this macro to run in 3D
// #define threeD

using namespace std;

namespace
{

// Declarations of functions whose implementations occur later.
void initSystem();
void stepSystem();
void drawSystem();
void freeSystem();
void resetTime();

void initRendering();
void drawAxis();

// Some constants
const Vector3f LIGHT_POS(1.0f, 0.0f, 5.0f);
const Vector3f LIGHT_COLOR(46, 123, 247);
const Vector3f FLOOR_COLOR(252, 252, 252);

// time keeping
// current "tick" (e.g. clock number of processor)
uint64_t start_tick;
// number of seconds simulated
double simulated_s;

// Globals here.
TimeStepper* timeStepper;
float h;
char integrator;

Camera camera;
bool gMousePressed = false;
GLuint program_color;
GLuint program_light;

FluidSim* fluid;

// Function implementations
static void keyCallback(GLFWwindow* window, int key,
    int scancode, int action, int mods)
{
    if (action == GLFW_RELEASE) { // only handle PRESS and REPEAT
        return;
    }

    // Special keys (arrows, CTRL, ...) are documented
    // here: http://www.glfw.org/docs/latest/group__keys.html
    switch (key) {
    case GLFW_KEY_ESCAPE: // Escape key
        exit(0);
    case ' ':
    {
        Matrix4f eye = Matrix4f::identity();
        camera.SetRotation(eye);
        camera.SetCenter(Vector3f(0, 0, 0));
        break;
    }
    case 'R':
    {
        cout << "Resetting simulation\n";
        freeSystem();
        initSystem();
        resetTime();
        break;
    }
    default:
        cout << "Unhandled key press " << key << "." << endl;
    }
}

static void mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
    double xd, yd;
    glfwGetCursorPos(window, &xd, &yd);
    int x = (int)xd;
    int y = (int)yd;

    int lstate = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    int rstate = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
    int mstate = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
    if (lstate == GLFW_PRESS) {
        gMousePressed = true;
        camera.MouseClick(Camera::LEFT, x, y);
    }
    else if (rstate == GLFW_PRESS) {
        gMousePressed = true;
        camera.MouseClick(Camera::RIGHT, x, y);
    }
    else if (mstate == GLFW_PRESS) {
        gMousePressed = true;
        camera.MouseClick(Camera::MIDDLE, x, y);
    }
    else {
        gMousePressed = true;
        camera.MouseRelease(x, y);
        gMousePressed = false;
    }
}

static void motionCallback(GLFWwindow* window, double x, double y)
{
    if (!gMousePressed) {
        return;
    }
    camera.MouseDrag((int)x, (int)y);
}

void setViewport(GLFWwindow* window)
{
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);

    camera.SetDimensions(w, h);
    camera.SetViewport(0, 0, w, h);
    camera.ApplyViewport();
}

void drawAxis()
{
    glUseProgram(program_color);
    Matrix4f M = Matrix4f::translation(camera.GetCenter()).inverse();
    camera.SetUniforms(program_color, M);

    const Vector3f DKRED(1.0f, 0.5f, 0.5f);
    const Vector3f DKGREEN(0.5f, 1.0f, 0.5f);
    const Vector3f DKBLUE(0.5f, 0.5f, 1.0f);
    const Vector3f GREY(0.5f, 0.5f, 0.5f);

    const Vector3f ORGN(0, 0, 0);
    const Vector3f AXISX(5, 0, 0);
    const Vector3f AXISY(0, 5, 0);
    const Vector3f AXISZ(0, 0, 5);

    VertexRecorder recorder;
    recorder.record_poscolor(ORGN, DKRED);
    recorder.record_poscolor(AXISX, DKRED);
    recorder.record_poscolor(ORGN, DKGREEN);
    recorder.record_poscolor(AXISY, DKGREEN);
    recorder.record_poscolor(ORGN, DKBLUE);
    recorder.record_poscolor(AXISZ, DKBLUE);

    recorder.record_poscolor(ORGN, GREY);
    recorder.record_poscolor(-AXISX, GREY);
    recorder.record_poscolor(ORGN, GREY);
    recorder.record_poscolor(-AXISY, GREY);
    recorder.record_poscolor(ORGN, GREY);
    recorder.record_poscolor(-AXISZ, GREY);

    glLineWidth(3);
    recorder.draw(GL_LINES);
}


// initialize your particle systems
void initSystem()
{
    switch (integrator) {
    case 'e': timeStepper = new ForwardEuler(); break;
    case 't': timeStepper = new Trapezoidal(); break;
    case 'r': timeStepper = new RK4(); break;
    default: printf("Unrecognized integrator\n"); exit(-1);
    }

    fluid = new FluidSim();
}

void freeSystem() {
    delete timeStepper; timeStepper = nullptr;
    delete fluid; fluid = nullptr;
}

void resetTime() {
    simulated_s = 0;
    start_tick = glfwGetTimerValue();
}

void stepSystem()
{
    vector<Vector3f> state = fluid->get_state();
    vector<Vector3f> new_state;
    const float TANK_START_X = -1.0f;
    const float  TANK_END_X = 1.0f;
    const float TANK_START_Y = -1.0f;
    const float TANK_END_Y = 1.0f;
    const float TANK_START_Z = -1.0f;
    const float TANK_END_Z = 1.0f;
    for (int i = 0; i < (int)state.size() / 2; i++) {
        int randNum = rand() % 20 - 10;
        Vector3f pos = state[2*i];
        Vector3f velocity = state[2*i+1];
        if (pos.x() <= TANK_START_X)
            velocity = Vector3f(abs(velocity.x()), velocity.y(), velocity.z());
        if (pos.x() >= TANK_END_X)
            velocity = Vector3f(-0.3f*abs(velocity.x()), velocity.y(), velocity.z());
        if (pos.y() <= TANK_START_Y) {
            velocity = Vector3f(velocity.x() + randNum/100.0f, 0.6 * abs(velocity.y()) +
                                1.0f*(5.0f-abs(randNum))/100.0f, velocity.z() + randNum / 100.0f);
        }
#ifdef threeD
        randNum = rand() % 10 - 5;
        if (pos.z() <= TANK_START_Z) {
            velocity = Vector3f(velocity.x() + 1.0f*randNum/200.0f, 0.0, 0.4 * abs(velocity.y()) + 1.0f*(5.0f-abs(randNum))/100.0f);
        }
#endif
        new_state.push_back(pos);
        new_state.push_back(velocity);
    }
    fluid->set_state(new_state);
    timeStepper->takeStep(fluid, h);
    simulated_s += h;
}

// Draw the current particle positions
void drawSystem()
{
    // GLProgram wraps up all object that
    // particle systems need for drawing themselves
    GLProgram gl(program_light, program_color, &camera);
    gl.updateLight(LIGHT_POS, LIGHT_COLOR.xyz()); // once per frame

    fluid->draw(gl);

    // set uniforms for floor
    gl.updateMaterial(FLOOR_COLOR);
    gl.updateModelMatrix(Matrix4f::translation(0, -1.08, 0));
    // draw floor
    drawQuad(50.);
}

//-------------------------------------------------------------------

void initRendering()
{
    // Clear to black
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}
}

// Main routine.
// Set up OpenGL, define the callbacks and start the main loop
int main(int argc, char** argv)
{
    if (argc != 3) {
        printf("Usage: %s <e|t|r> <timestep>\n", argv[0]);
        printf("\te: Integrator: Forward Euler\n");
        printf("\tt: Integrator: Trapezoid\n");
        printf("\tr: Integrator: RK 4\n");
        printf("\n");
        printf("Try: %s t 0.001\n", argv[0]);
        printf("\tfor trapezoid (1ms steps)\n");
        printf("Or: %s r 0.01\n", argv[0]);
        printf("\tfor RK4 (10ms steps)\n");
        return -1;
    }

    integrator = argv[1][0];
    h = (float)atof(argv[2]);
    printf("Using Integrator %c with time step %.4f\n", integrator, h);


    GLFWwindow* window = createOpenGLWindow(1024, 1024, "Final Project");

    // setup the event handlers
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseCallback);
    glfwSetCursorPosCallback(window, motionCallback);

    initRendering();

    // The program object controls the programmable parts
    // of OpenGL. All OpenGL programs define a vertex shader
    // and a fragment shader.
    program_color = compileProgram(c_vertexshader, c_fragmentshader_color);
    if (!program_color) {
        printf("Cannot compile program\n");
        return -1;
    }
    program_light = compileProgram(c_vertexshader, c_fragmentshader_light);
    if (!program_light) {
        printf("Cannot compile program\n");
        return -1;
    }

    camera.SetDimensions(600, 600);
    camera.SetPerspective(50);
    camera.SetDistance(5);

    // Setup particle system
    initSystem();

    // Main Loop
    resetTime();
    while (!glfwWindowShouldClose(window)) {
        // Clear the rendering window
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        setViewport(window);

        if (gMousePressed) {
            drawAxis();
        }

        uint64_t now = glfwGetTimerValue();
        stepSystem();

        // Draw the simulation
        drawSystem();

        // Make back buffer visible
        glfwSwapBuffers(window);

        // Check if any input happened during the last frame
        glfwPollEvents();
    }

    // All OpenGL resource that are created with
    // glGen* or glCreate* must be freed.
    glDeleteProgram(program_color);
    glDeleteProgram(program_light);

    return 0;	// This line is never reached.
}
