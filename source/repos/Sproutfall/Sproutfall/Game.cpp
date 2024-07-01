#include <SFML/Graphics.hpp>
#include "Player.h"
#include "BackgroundLayer.h"
#include "SceneManager.h"
#include "Asteroid.h"
#include <random>
#include <ctime>

enum Scenes{TitleScreen, Space, Sky, Forest, Win};
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
    //window.setFramerateLimit(60);
    //window.setVerticalSyncEnabled(true);

    sceneManager.loadScene(TitleScreen);
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
            sceneManager.handleInput(&event);
        }
        if (sceneManager.isFullscreen() != m_Fullscreen)
        {
            if (!sceneManager.isFullscreen())
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
        float tf = clock.getElapsedTime().asSeconds();
        clock.restart();

        if (tf > 1.0 / 60.0)
           tf = 1.0 / 60.0;
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