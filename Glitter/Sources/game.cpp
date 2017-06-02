#include "game.hpp"
#include "game_level.hpp"
#include "ball_object.hpp"
#include "resource_manager.hpp"
#include "sprite_renderer.hpp"

#include <iostream>

SpriteRenderer  *Renderer;

// Initial size of the player paddle
const glm::vec2 PLAYER_SIZE(100, 20);
// Initial velocity of the player paddle
const GLfloat PLAYER_VELOCITY(500.0f);

GameObject      *Player;

// Initial velocity of the Ball
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// Radius of the ball object
const GLfloat BALL_RADIUS = 12.5f;

BallObject     *Ball;

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
  ResourceManager::LoadTexture("paddle.png", true, "paddle");
  ResourceManager::LoadTexture("background.jpg", GL_FALSE, "background");
  ResourceManager::LoadTexture("awesomeface.png", GL_TRUE, "face");
  ResourceManager::LoadTexture("block.png", GL_FALSE, "block");
  ResourceManager::LoadTexture("block_solid.png", GL_FALSE, "block_solid");
  // Load levels
  GameLevel one; one.Load("one", this->Width, this->Height * 0.5);
  GameLevel two; two.Load("two", this->Width, this->Height * 0.5);
  GameLevel three; three.Load("three", this->Width, this->Height * 0.5);
  GameLevel four; four.Load("four", this->Width, this->Height * 0.5);
  this->Levels.push_back(one);
  this->Levels.push_back(two);
  this->Levels.push_back(three);
  this->Levels.push_back(four);
  this->Level = 0;
  // Player
  glm::vec2 playerPos = glm::vec2(
                                  this->Width / 2 - PLAYER_SIZE.x / 2,
                                  this->Height - PLAYER_SIZE.y);
  Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
  // Ball
  glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
  Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,
                        ResourceManager::GetTexture("face"));
}

void Game::Update(GLfloat dt)
{
  Ball->Move(dt, this->Width);
}


void Game::ProcessInput(GLfloat dt, GLint movement, GLboolean action)
{
  if (this->State == GAME_ACTIVE)
    {
      GLfloat velocity = PLAYER_VELOCITY * dt;
      // Move player
      if (movement == -1)
        {
          if (Player->Position.x >= 0) {
            Player->Position.x -= velocity;
            if (Ball->Stuck)
              Ball->Position.x -= velocity;
          }
        }
      if (movement == 1)
      {
        if (Player->Position.x <= this->Width - Player->Size.x) {
          Player->Position.x += velocity;
          if (Ball->Stuck)
            Ball->Position.x += velocity;
        }
      }
      if (action == true)
        Ball->Stuck = false;  
    }
}

void Game::Render()
{
  if(this->State == GAME_ACTIVE)
    {
      // Draw background
      Texture2D background;
      background = ResourceManager::GetTexture("background");
      Renderer->DrawSprite(background,
                           glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f
                           );
      // Draw all objects
      this->Levels[this->Level].Draw(*Renderer);
      Player->Draw(*Renderer);
      Ball->Draw(*Renderer);      
    }
}
