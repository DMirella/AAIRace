#include "car.h"
#include "road.h"
#include "game_session.h"

#include <vector>
#include <SFML/Graphics.hpp>
 
using namespace sf;

int main() {
  std::shared_ptr<RenderWindow> window = std::make_shared<RenderWindow>(sf::VideoMode(1280, 720), "Lesson 4. kychka-pc.ru"); 

	GameSession game_session(window, 4, 150);
 
  sf::Clock clock;
	while (window->isOpen())	
	{
    float time = clock.getElapsedTime().asMicroseconds();
    clock.restart(); //перезагружает время
    time = time/200; //скорость игры
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->close();
		}
		game_session.CheckControllers();
		game_session.ProcessGameEvents();
		game_session.Update(time);

		window->clear();
		game_session.DrawEntites();
		window->display();
	}
 
}