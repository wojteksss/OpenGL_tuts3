#include <iostream>
#include <chrono>
#include <thread>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw_gl3.h"

#include "resources/tests/TestClearColor.h"


const GLint WIDTH = 800, HEIGHT = 600;
void printGL_Version();
const float fWIDTH = WIDTH, fHEIGHT = 600;


int main(void)
{
    glfwInit();
    
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", nullptr, nullptr);
    
    int screenWidth, screenHeight;
    GLCall(glfwGetFramebufferSize(window, &screenWidth, &screenHeight));
    
    if (nullptr == window)
    {
        glfwTerminate();
        return EXIT_FAILURE;
    }
    
    GLCall(glfwMakeContextCurrent(window));
    glfwSwapInterval(1);
    
    glewExperimental = GL_TRUE;
    
    if(glewInit() != GLEW_OK) {
        std::cout << "Hej! to jest error" << std::endl;
        return EXIT_FAILURE;
    }
    
    printGL_Version();

    {
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        
        Renderer renderer;
        
        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();
        
        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;
        
        testMenu->RegisterTest<test::TestClearColor>("Clear Color");
        
        while (!glfwWindowShouldClose(window))
        {
            GLCall(glClearColor(1.0f, 0.4f, 0.0f, 0.5f));
            renderer.Clear();
            
            ImGui_ImplGlfwGL3_NewFrame();
            if (currentTest)
            {
                currentTest->OnUpdate(0.0f);
                currentTest->OnRender();
                ImGui::Begin("Test");
                if(currentTest != testMenu && ImGui::Button("<-"))
                {
                    delete currentTest;
                    currentTest = testMenu;
                }
                currentTest->OnImGuiRender();
                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
            
            glfwSwapBuffers(window);
            GLCall(glfwPollEvents());
        }
        delete currentTest;
        if(currentTest != testMenu)
            delete testMenu;
    }
    
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

void printGL_Version()
{
    std::cout << std::endl << std::endl;
    std::cout << "=======================================================" << std::endl;
    std::cout << "=======================================================" << std::endl;
    std::cout << "======== OpenGL - version: " << glGetString(GL_VERSION) << " =========" << std::endl;
    std::cout << "=======================================================" << std::endl;
    std::cout << "=======================================================" << std::endl;
    std::cout << "GL_TYPE: " << GL_FLOAT << std::endl;
}

