
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
    //_CrtDumpMemoryLeaks();
    return 0;
}

int Game()
{
    srand(time(NULL));
    cout << rand() << endl;
    //::_CrtSetDbgFlag(::_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
    sf::RenderWindow window(sf::VideoMode(1280, 960), "Sproutfall");
    Player player(&window);
    /*sf::View view;
    view.setSize(1280, 960);*/
    sf::Texture texture;
    if (!texture.loadFromFile("Textures/asteroid.png"))
    {
        "Asteroid load fail!";
    }
    Asteroid asteroid(&texture);
    asteroid.setPosition(100, 100);
    SceneManager sceneManager(&player, 1280, 960);
    sceneManager.loadScene(Space);
    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            player.handleInput(&event);
        }
        float tf = clock.getElapsedTime().asSeconds();
        player.Update(tf);
        sceneManager.Update(tf);
        asteroid.Update(tf);
        clock.restart();
        window.clear();
        //window.draw(backgroundSprite);
        window.draw(sceneManager);
        window.draw(player);
        window.draw(asteroid);
        window.display();
    }
    return 0;
}