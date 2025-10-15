#include <iostream>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>

float screenWidth  = 800.0f;  // logical coords for your ortho
float screenHeight = 600.0f;

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // width/height = framebuffer size in pixels (su Retina è 2x o simile)
    glViewport(0, 0, width, height);
}

GLFWwindow* StartGLFW();
void DrawCircle(float centerX, float centerY, float radius, int res);

class Object {
public:
    std::vector<float> position;
    std::vector<float> velocity;
    float radius;
    Object(std::vector<float> position, std::vector<float> velocity, float radius = 15.0f){
        this->position = position;
        this->velocity = velocity;
        this->radius = radius;
    };
    void accelerate(float x, float y){
        this->velocity[0] += x;
        this->velocity[1] += y;
    };
    void updatePos() {
        this->position[0] += this->velocity[0];
        this->position[1] += this->velocity[1];
    }

    void DrawCircle(float cx, float cy, float r, int res) {
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);
        for (int i = 0; i <= res; ++i) {
            float angle = 2.0f * 3.14159265358979323846f * (float)i / (float)res;
            float x = cx + std::cos(angle) * r;
            float y = cy + std::sin(angle) * r;
            glVertex2f(x, y);
        }
        glEnd();
    }
};


int main() {
    GLFWwindow* window = StartGLFW();

    std::vector<Object> objs = {
        Object(std::vector<float>{200.0f, 500.0f}, std::vector<float>{0.0f, 10.0f}, 20.0f),
        Object(std::vector<float>{200.0f, 300.0f}, std::vector<float>{0.0f, 10.0f}, 20.0f),
    };

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        for (auto& obj:objs) {
            obj.accelerate(0.0f, 9.81/20.0f);
            obj.updatePos();
            obj.DrawCircle(obj.position[0], obj.position[1], obj.radius, 20);
            if (obj.position[1] + obj.radius < 0 || obj.position[1] + obj.radius > screenHeight) {
                obj.velocity[1] *= -0.95f; // reverse and dampen velocity
            }
            if (obj.position[0] + obj.radius< 0 || obj.position[0] + obj.radius > screenWidth) {
                obj.velocity[0] *= -0.95f; // reverse and dampen velocity
            }
        }

        //collision between objects
        for (size_t i = 0; i < objs.size(); ++i) {
            for (size_t j = i + 1; j < objs.size(); ++j) {
                float dx = objs[j].position[0] - objs[i].position[0];
                float dy = objs[j].position[1] - objs[i].position[1];
                float distance = std::sqrt(dx * dx + dy * dy);
                if (distance < objs[i].radius + objs[j].radius) {
                    // Simple elastic collision response
                    objs[i].velocity[0] *= -0.95f;
                    objs[i].velocity[1] *= -0.95f;
                    objs[j].velocity[0] *= -0.95f;
                    objs[j].velocity[1] *= -0.95f;
                }
            }
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

GLFWwindow* StartGLFW(){
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return nullptr;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Gravity Simulation", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glClearColor(0.f, 0.f, 0.f, 1.f);

    int fbw, fbh;
    glfwGetFramebufferSize(window, &fbw, &fbh);
    glViewport(0, 0, fbw, fbh);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, screenHeight, 0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    return window;
}
