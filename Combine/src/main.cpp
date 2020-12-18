#include "imgui_setup.h"
#include "camera.h"
#include "renderengine.h"
#include "world.h"
#include "material.h"
#include "object.h"
#include "sphere.h"
#include "triangle.h"
#include "lightsource.h"
#include "pointlightsource.h"
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../depends/stb/stb_image.h"
#include "../depends/stb/stb_image_write.h"

#define RENDER_BATCH_COLUMNS 20 // Number of columns to render in a single go. Increase to gain some display/render speed!

Camera *camera;
RenderEngine *engine;

int screen_width = 800, screen_height = 600; // This is window size, used to display scaled raytraced image.
int image_width = 1920, image_height = 1080; // This is raytraced image size. Change it if needed.
GLuint texImage;
float aspect_ratio = (float) screen_width / (float) screen_height;

// GLFWwindow * mainWindow = nullptr;
GLuint program;
GLint uniform_texImage;

int main(int, char**)
{
    // Setup window
    GLFWwindow *window = setupWindow(screen_width, screen_height);

    ImVec4 clearColor = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);

    // Setup raytracer camera. This is used to spawn rays.
    Vector3D camera_position(1, 0, 10);
    Vector3D camera_target(0, 0, 0); //Looking down -Z axis
    Vector3D camera_up(0, 1, 0);
    float camera_fovy =  45;
    camera = new Camera(camera_position, camera_target, camera_up, camera_fovy, image_width, image_height);

    //Create a world
    World *world = new World;
    world->setAmbient(Color(1));
    world->setBackground(Color(0.1, 0.1, 0.1));
    
    Material *m = new Material(world);
    m->color = Color(0.1, 0.7, 0.0);
    m->ka = 0.8;
    m->kd = 0.9;
    // m->ks = 0.5;
    m->kr = 0.5;
    m->kt = 0.2;
    m->eta = 1.5;

    // Object *sphere = new Sphere(Vector3D(-7, 0, -4), 1.5, m);
    // world->addObject(sphere);
    
    Material *m1 = new Material(world);
    m1->color = Color(0.4, 0.2, 0.3);
    m1->ka = 0.8;
    m1->kd = 0.9;
    // m1->ks = 0.3;
    m1->kr = 0.3;
    m1->kt = 0.2;
    
    Object *sphere1 = new Sphere(Vector3D(5, 2, -4), 1, m1);
    sphere1->mb = true;
    world->addObject(sphere1);

    Material *m2 = new Material(world);
    m2->color = Color(0.5, 0.1, 0.5);
    m2->ka = 0.8;
    m2->kd = 0.9;
    m2->ks = 0.5;
    m2->kr = 0.5;
    m2->kt = 0.2;

    Material *m3 = new Material(world);
    m3->color = Color(0.1, 0.3, 0.3);
    m3->ka = 0.8;
    m3->kd = 0.9;
    m3->ks = 0.6;
    m3->kr = 0.3;
    m3->kt = 0.2;

    // Object *triangle = new Triangle(Vector3D(-6, 2, 0), Vector3D(-6, 0, -2), Vector3D(-4, 0, -2), m2);
    // world->addObject(triangle);

    // Object *triangle1 = new Triangle(Vector3D(-6, 2.5, 0), Vector3D(-6, 1, -1.5), Vector3D(-4, 1, -1.5), m3);
    // world->addObject(triangle1);

    Object *triangle2 = new Triangle(Vector3D(-3, 2, -2), Vector3D(-3, 0, -4.5), Vector3D(-1, 0, -4.5), m2);
    world->addObject(triangle2);

    Object *triangle3 = new Triangle(Vector3D(-3, 3, -3), Vector3D(-3, 1.5, -3.5), Vector3D(-1, 1.5, -3.5), m3);
    world->addObject(triangle3);


    Material *m4 = new Material(world);
    m4->color = Color(0.1, 0.7, 0.0);
    m4->ka = 0.8;
    m4->kd = 0.9;
    m4->ks = 0.5;
    m4->kr = 0.5;
    m4->kt = 0.2;

    Object *sphere2 = new Sphere(Vector3D(-7, -3, -4), 1, m4);
    sphere2->mb = false;
    world->addObject(sphere2);
    
    Material *m5 = new Material(world);
    m5->color = Color(0.2, 0.2, 0.9);
    m5->ka = 0.8;
    m5->kd = 0.9;
    m5->ks = 0.3;
    m5->kt = 0.2;
    
    Object *sphere3 = new Sphere(Vector3D(-3, -3.5, -1), 0.5, m5);
    sphere3->mb = false;
    world->addObject(sphere3);

    Material *m6 = new Material(world);
    m6->color = Color(0.1, 0.7, 0.0);
    m6->ka = 0.2;
    m6->kd = 0.9;
    m6->ks = 0.5;

    Object *sphere4 = new Sphere(Vector3D(5, -3, -5), 1.5, m6);
    sphere4->mb = false;
    world->addObject(sphere4);
    
    Material *m7 = new Material(world);
    m7->color = Color(1.0, 1.0, 1.0);
    m7->ka = 0.2;
    m7->kd = 0.9;
    m7->ks = 0.3;

    Object *sphere5 = new Sphere(Vector3D(4, -3.5, -3.5), 0.75, m7);
    sphere5->mb = false;
    world->addObject(sphere5);

    // Material *m8 = new Material(world);
    // m8->color = Color(0.1, 0.7, 0.0);
    // m8->ka = 0.4;
    // m8->kd = 0.6;
    // m8->ks = 0.5;
    // // m8->kr = 0.3;
    // // m8->kt = 0.2;
    // // m8->eta = 1.05;

    // Object *sphere6 = new Sphere(Vector3D(5, 0, -8), 3, m8);
    // sphere6->mb = false;
    // world->addObject(sphere6);
    
    // Material *m9 = new Material(world);
    // m9->color = Color(1.0, 1.0, 1.0);
    // m9->ka = 0.3;
    // m9->kd = 0.5;
    // m9->ks = 0.3;
    // // m9->kr = 0.3;
    // // m9->kt = 0.2;
    // // m9->eta = 1.1;
    
    // Object *sphere7 = new Sphere(Vector3D(-5, 0, -8), 3, m9);
    // sphere7->mb = false;
    // world->addObject(sphere7);

    // Material *m10 = new Material(world);
    // m10->color = Color(1.0, 0.0, 0.0);
    // m10->ka = 0.3;
    // m10->kd = 0.5;
    // m10->ks = 0.3;
    // m10->kr = 0.3;
    // m10->kt = 0.2;
    // m10->eta = 1.01;

    // Object *sphere8 = new Sphere(Vector3D(0, 0, 2), 2, m10);
    // sphere8->mb = false;
    // world->addObject(sphere8);

    LightSource *light = new PointLightSource(world, Vector3D(-2, 0, 10), Color(1, 1, 1));
    world->addLight(light);

    engine = new RenderEngine(world, camera);

    //Initialise texture
    glGenTextures(1, &texImage);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, camera->getBitmap());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        bool render_status;
        for(int i=0; i<RENDER_BATCH_COLUMNS; i++) 
            render_status = engine->renderLoop(); // RenderLoop() raytraces 1 column of pixels at a time.
        if(!render_status)
        {
            // Update texture
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texImage);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image_width, image_height, GL_RGB, GL_UNSIGNED_BYTE, camera->getBitmap());
        } 

        ImGui::Begin("Lumina", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("Size: %d x %d", image_width, image_height);
        if(ImGui::Button("Save")){
          char filename[] = "img.png";
          stbi_write_png(filename, image_width, image_height, 3, camera->getBitmap(),0);        
        }
        //Display render view - fit to width
        int win_w, win_h;
        glfwGetWindowSize(window, &win_w, &win_h);
        float image_aspect = (float)image_width/(float)image_height;
        float frac = 0.95; // ensure no horizontal scrolling
        ImGui::Image((void*)(intptr_t)texImage, ImVec2(frac*win_w, frac*win_w/image_aspect), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f));

        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    glDeleteTextures(1, &texImage);

    cleanup(window);

    return 0;
}
