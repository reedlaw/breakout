// Local Headers
#include "glitter.hpp"
#include "shader.hpp"
#include "game.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <iostream>
#include <cstdio>
#include <cstdlib>

Game Breakout(mWidth, mHeight);

int main(int argc, char * argv[]) {

  // Load GLFW and Create a Window
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  auto mWindow = glfwCreateWindow(mWidth, mHeight, "Breakout", nullptr, nullptr);

  // Check for Valid Context
  if (mWindow == nullptr) {
    fprintf(stderr, "Failed to Create OpenGL Context");
    return EXIT_FAILURE;
  }

  // Create Context and Load OpenGL Functions
  glfwMakeContextCurrent(mWindow);
  gladLoadGL();
  fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

  // OpenGL configuration
  glViewport(0, 0, mWidth, mHeight);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Initialize game
  Breakout.Init();

  // DeltaTime variables
  GLfloat deltaTime = 0.0f;
  GLfloat lastFrame = 0.0f;

  // Start Game within Menu State
  Breakout.State = GAME_ACTIVE;

  GLint movement = 0; // -1 for left, 0 for none, 1 for right
  GLboolean action = false;

  // Rendering Loop
  while (glfwWindowShouldClose(mWindow) == false) {
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(mWindow, true);

    if (glfwGetKey(mWindow, GLFW_KEY_LEFT) == GLFW_PRESS)
      movement = -1;

    if (glfwGetKey(mWindow, GLFW_KEY_RIGHT) == GLFW_PRESS)
      movement = 1;

    if ((glfwGetKey(mWindow, GLFW_KEY_LEFT) != GLFW_PRESS) && (glfwGetKey(mWindow, GLFW_KEY_RIGHT) != GLFW_PRESS))
      movement = 0;

    if (glfwGetKey(mWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
      action = true;

    glfwPollEvents();
        
    // Calculate delta time
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
        
    // Manage user input
    Breakout.ProcessInput(deltaTime, movement, action);

    // Update Game state
    Breakout.Update(deltaTime);

    // Background Fill Color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    Breakout.Render();
        
    glfwSwapBuffers(mWindow);
  }
  glfwTerminate();
  return EXIT_SUCCESS;
}
