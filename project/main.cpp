// glew must be before glfw
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// contains helper functions such as shader compiler
#include "icg_helper.h"

#include <glm/gtc/matrix_transform.hpp>
#include "framebuffer.h"

#include "Cube/Cube.h"
#include "grid/grid.h"

#include "trackball.h"

#define PI 3.14f

Cube cube;
Grid grid;

int window_width = 800;
int window_height = 600;
float z_axis = 0.0f;

using namespace glm;

FrameBuffer framebuffer;

mat4 projection_matrix;
mat4 view_matrix;
mat4 trackball_matrix;
mat4 old_trackball_matrix;
mat4 cube_scale;
mat4 quad_model_matrix;

float mouse_click_y = 0.0f;
bool is_zooming = false;
float ZOOM_FACTOR = 4.0f;
mat4 matrix_click = mat4(1.0f);

Trackball trackball;

mat4 OrthographicProjection(float left, float right, float bottom,
                            float top, float near, float far) {
    assert(right > left);
    assert(far > near);
    assert(top > bottom);
    mat4 projection = mat4(1.0f);
    projection[0][0] = 2.0f / (right - left);
    projection[1][1] = 2.0f / (top - bottom);
    projection[2][2] = -2.0f / (far - near);
    projection[3][3] = 1.0f;
    projection[3][0] = -(right + left) / (right - left);
    projection[3][1] = -(top + bottom) / (top - bottom);
    projection[3][2] = -(far + near) / (far - near);
    return projection;
}

mat4 PerspectiveProjection(float fovy, float aspect, float near, float far) {
    
    assert(fovy > 0.0f);
    assert(aspect > 0.0f);
    assert(far > near);

    float top = near * tan((PI/180.0f) * (fovy/2.0f));
    float bottom = -top;
    float right = top * aspect;
    float left = -right;

    mat4 projection = mat4(1.0f);
    projection[0][0] = (2.0f * near) / (right-left);
    projection[1][1] = (2.0f * near) / (top-bottom);
    projection[2][2] = -1.0f * ((far + near) / (far - near));
    projection[3][3] = 0.0f;
    projection[2][0] = (right + left) / (right - left);
    projection[2][1] = (top + bottom) / (top - bottom);
    projection[3][2] = (-2.0f * far * near) / (far - near);
    projection[2][3] = -1.0f;
    return projection;
}

mat4 LookAt(vec3 eye, vec3 center, vec3 up) {
    // we need a function that converts from world coordinates into camera coordiantes.
    //
    // cam coords to world coords is given by:
    // X_world = R * X_cam + eye
    //
    // inverting it leads to:
    //
    // X_cam = R^T * X_world - R^T * eye
    //
    // or as a homogeneous matrix:
    // [ r_00 r_10 r_20 -r_0*eye
    //   r_01 r_11 r_21 -r_1*eye
    //   r_02 r_12 r_22 -r_2*eye
    //      0    0    0        1 ]

    vec3 z_cam = normalize(eye - center);
    vec3 x_cam = normalize(cross(up, z_cam));
    vec3 y_cam = cross(z_cam, x_cam);

    mat3 R(x_cam, y_cam, z_cam);
    R = transpose(R);

    mat4 look_at(vec4(R[0], 0.0f),
                 vec4(R[1], 0.0f),
                 vec4(R[2], 0.0f),
                 vec4(-R * (eye), 1.0f));
    return look_at;
}

void Init() {
    // sets background color
    glClearColor(0.937, 0.937, 0.937 /*gray*/, 1.0 /*solid*/);
    
    grid.Init();

    // enable depth test.
    glEnable(GL_DEPTH_TEST);

    // TODO 3: once you use the trackball, you should use a view matrix that
    // looks straight down the -z axis. Otherwise the trackball's rotation gets
    // applied in a rotated coordinate frame.
    // uncomment lower line to achieve this.
    view_matrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, -4.0f));

    trackball_matrix = IDENTITY_MATRIX;

    // scaling matrix to scale the cube down to a reasonable size.
    cube.scale(glm::vec3(0.25f, 0.25f, 0.25f));

    quad_model_matrix = translate(mat4(1.0f), vec3(0.0f, -0.25f, 0.0f));
}

// gets called for every frame.
void Display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const float time = glfwGetTime();

    cube.Draw(view_matrix, projection_matrix);

    // draw a quad on the ground.
    grid.Draw(time, trackball_matrix * quad_model_matrix, view_matrix, projection_matrix);
}

// transforms glfw screen coordinates into normalized OpenGL coordinates.
vec2 TransformScreenCoords(GLFWwindow* window, int x, int y) {
    // the framebuffer and the window doesn't necessarily have the same size
    // i.e. hidpi screens. so we need to get the correct one
    int width;
    int height;
    glfwGetWindowSize(window, &width, &height);
    return vec2(2.0f * (float)x / width - 1.0f,
                1.0f - 2.0f * (float)y / height);
}

void MouseButton(GLFWwindow* window, int button, int action, int mod) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double x_i, y_i;
        glfwGetCursorPos(window, &x_i, &y_i);
        vec2 p = TransformScreenCoords(window, x_i, y_i);
        trackball.BeingDrag(p.x, p.y);
        old_trackball_matrix = trackball_matrix;
        // Store the current state of the model matrix.
    }
}

void MousePos(GLFWwindow* window, double x, double y) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        vec2 p = TransformScreenCoords(window, x, y);
        // TODO 3: Calculate 'trackball_matrix' given the return value of
        // trackball.Drag(...) and the value stored in 'old_trackball_matrix'.
        // See also the mouse_button(...) function.
        // trackball_matrix = ...
        
        trackball_matrix = trackball.Drag(p.x,p.y) * old_trackball_matrix;

    }

    // zoom
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        vec2 p = TransformScreenCoords(window, x, y);
        //checking if the cursor has moved up or down to decrese/increase the translation in the z axis
        if (!is_zooming) {
            is_zooming = true;
            mouse_click_y = p.y;
            matrix_click = view_matrix;
        }
        float z_axis = (mouse_click_y - p.y)*ZOOM_FACTOR;
        view_matrix = translate(matrix_click, vec3(0.0f, 0.0f, z_axis));

    } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
        is_zooming = false;
        mouse_click_y = 0.0f;
    }
}

// Gets called when the windows/framebuffer is resized.
void SetupProjection(GLFWwindow* window, int width, int height) {
    window_width = width;
    window_height = height;

    cout << "Window has been resized to "
         << window_width << "x" << window_height << "." << endl;

    glViewport(0, 0, window_width, window_height);

    projection_matrix = PerspectiveProjection(45.0f, (GLfloat)window_width / window_height, 0.1f, 100.0f);
}

void ErrorCallback(int error, const char* description) {
    fputs(description, stderr);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}


int main(int argc, char *argv[]) {
    // GLFW Initialization
    if(!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return EXIT_FAILURE;
    }

    glfwSetErrorCallback(ErrorCallback);

    // hint GLFW that we would like an OpenGL 3 context (at least)
    // http://www.glfw.org/faq.html#how-do-i-create-an-opengl-30-context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // attempt to open the window: fails if required version unavailable
    // note some Intel GPUs do not support OpenGL 3.2
    // note update the driver of your graphic card
    GLFWwindow* window = glfwCreateWindow(window_width, window_height,
                                          "Trackball", NULL, NULL);
    if(!window) {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // makes the OpenGL context of window current on the calling thread
    glfwMakeContextCurrent(window);

    // set the callback for escape key
    glfwSetKeyCallback(window, KeyCallback);

    // set the framebuffer resize callback
    glfwSetFramebufferSizeCallback(window, SetupProjection);

    // set the mouse press and position callback
    glfwSetMouseButtonCallback(window, MouseButton);
    glfwSetCursorPosCallback(window, MousePos);

    // GLEW Initialization (must have a context)
    // https://www.opengl.org/wiki/OpenGL_Loading_Library
    glewExperimental = GL_TRUE; // fixes glew error (see above link)
    if(glewInit() != GLEW_NO_ERROR) {
        fprintf( stderr, "Failed to initialize GLEW\n");
        return EXIT_FAILURE;
    }

    cout << "OpenGL" << glGetString(GL_VERSION) << endl;

    // initialize our OpenGL program
    Init();

    // update the window size with the framebuffer size (on hidpi screens the
    // framebuffer is bigger)
    glfwGetFramebufferSize(window, &window_width, &window_height);
    SetupProjection(window, window_width, window_height);

    // render loop
    while(!glfwWindowShouldClose(window)){
        Display();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    grid.Cleanup();
    cube.Cleanup();

    // close OpenGL window and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}
