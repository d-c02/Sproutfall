
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "BackgroundLayer.h"
#include "SceneManager.h"
#include "Asteroid.h"
#include <random>
#include <ctime>

enum Scenes{TitleScreen, Space, Sky, Ground, Win};
int Game();
int main()
{
    Game();
    _CrtDumpMemoryLeaks();
    return 0;
}

int Game()
{
    srand(time(NULL));
    ::_CrtSetDbgFlag(::_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
    sf::RenderWindow window(sf::VideoMode(1280, 960), "Sproutfall");
    SceneManager sceneManager(1280, 960, &window);
    sceneManager.loadScene(Space);
    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            sceneManager.handleInput(&event);
        }
        float tf = clock.getElapsedTime().asSeconds();
        if (tf > 0.15)
            tf = 0.15;
        sceneManager.Update(tf);
        clock.restart();
        window.clear();

        window.draw(sceneManager);
        window.display();
    }
    return 0;
}