#include <SFML/Graphics.hpp>
#include "Player.h"
#include "BackgroundLayer.h"
#include "SceneManager.h"
#include "Asteroid.h"
#include <random>
#include <ctime>

enum Scenes{TitleScreen, Space, Sky, Ground, Win};
bool m_Fullscreen;
int Game();
int main()
{
    Game();
    return 0;
}

int Game()
{
    srand(time(NULL));
    m_Fullscreen = false;
    sf::RenderWindow window(sf::VideoMode(1280, 960), "Sproutfall");
    SceneManager sceneManager(window.getSize().x, window.getSize().y, &window);
    
    //TODO: Fix set framerate stuttering
    //window.setFramerateLimit();
    //window.setVerticalSyncEnabled(true);

    sceneManager.loadScene(Space);
    sf::Clock clock;
    float fpsTime = 0;
    float accumulator = 0;
    int fpsCtr = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::Resized)
                sceneManager.handleResize(event.size.width, event.size.height);
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::F)
                {
                    if (m_Fullscreen)
                    {
                        window.create(sf::VideoMode(1280, 960), "Sproutfall");
                        m_Fullscreen = false;
                    }
                    else
                    {
                        window.create(sf::VideoMode::getDesktopMode(), "Sproutfall", sf::Style::Fullscreen);
                        m_Fullscreen = true;
                    }
                    sceneManager.handleResize(window.getSize().x, window.getSize().y);
                }
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }
            }
            sceneManager.handleInput(&event);
        }
        float tf = clock.getElapsedTime().asSeconds();
        clock.restart();

        //if (tf > 1.0 / 60.0)
        //   tf = 1.0 / 60.0;
        accumulator += tf;

        while (accumulator > 1.0 / 60)
        {
            sceneManager.Update(tf);
            accumulator -= tf;
        }


        fpsTime += tf;
        fpsCtr++;

        if (fpsTime >= 1.0f)
        {
            cout << fpsCtr << " fps" << endl;
            fpsCtr = 0;
            fpsTime = 0.0f;
        }
        window.clear();

        window.draw(sceneManager);
        window.display();
    }
    return 0;
}