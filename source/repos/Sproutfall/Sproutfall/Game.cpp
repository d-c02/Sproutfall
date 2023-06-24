#include <SFML/Graphics.hpp>
#include "Player.h"
#include "BackgroundLayer.h"
#include "SceneManager.h"

enum Scenes{TitleScreen, Space, Sky, Ground, Win};
int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 960), "Sproutfall");
    Player player(&window);
    
    /*sf::View view;
    view.setSize(1280, 960);*/

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
        clock.restart();
        window.clear();
        
        //window.draw(backgroundSprite);
        window.draw(sceneManager);
        window.draw(player);
        window.display();
    }

    return 0;
}