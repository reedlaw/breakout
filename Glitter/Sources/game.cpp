#include "game.hpp"
#include "resource_manager.hpp"
#include "sprite_renderer.hpp"

SpriteRenderer  *Renderer;

Game::Game(GLuint width, GLuint height) 
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height) 
{ 

}

Game::~Game()
{
  delete Renderer;
}

void Game::Init()
{
    // Load shaders
    ResourceManager::LoadShader("sprite.vs", "sprite.fs", nullptr, "sprite");
    // Configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), 
        static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // Set render-specific controls
    Shader sprite;
    sprite = ResourceManager::GetShader("sprite");
    Renderer = new SpriteRenderer(sprite);
    // Load textures
    ResourceManager::LoadTexture("awesomeface.png", GL_TRUE, "face");
}

void Game::Update(GLfloat dt)
{

}


void Game::ProcessInput(GLfloat dt)
{

}

void Game::Render()
{
  Texture2D face;
  face = ResourceManager::GetTexture("face");
  Renderer->DrawSprite(face, glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));    
}
