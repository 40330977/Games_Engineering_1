#include "engine.h"
#include "game.h"
#include "scenes/scene_menu.h"

using namespace std;

MenuScene menu;
Main_menu mainmenu;
options option;
Level1Scene level1;
Level2Scene level2;
Level3Scene level3;

sf::Sound sound;
//bool full;


//sf::Style:Fullscreen
int main() {
  Engine::Start(1280, 720, "Doors of Perception","null",&menu);
  //Engine::Start(1280, 720, "Platformer1", "sf::Style:Fullscreen", &menu);
  
}