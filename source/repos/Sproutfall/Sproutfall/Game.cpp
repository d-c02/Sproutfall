#include <SFML/Graphics.hpp>
#include "Player.h"
#include "BackgroundLayer.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 960), "Sproutfall");
    Player player(&window);
    sf::Sprite backgroundSprite;
  
    sf::View view;
    view.setSize(1280, 960);
    //REMOVE ONCE DONE TESTING
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("Textures/bg.png"))
    {
        cout << "Background texture load failure";
    }
    backgroundSprite.setScale(2, 2);
    backgroundSprite.setTexture(backgroundTexture);

    sf::Texture* backgroundTexture2 = new sf::Texture();
    if (!backgroundTexture2->loadFromFile("Textures/space_stars_small.png"))
    {
        cout << "space texture load failure";
    }
    BackgroundLayer bgLayer(&player, 0.0f, (float) view.getSize().x, (float) view.getSize().y, backgroundTexture2);
    bgLayer.setScale(2, 2);

    sf::Texture* backgroundTexture3 = new sf::Texture();
    if (!backgroundTexture3->loadFromFile("Textures/space_stars_big.png"))
    {
        cout << "space 2 texture load failure jajaja";
    }
    BackgroundLayer bgLayer2(&player, 1.0f, (float)view.getSize().x, (float)view.getSize().y, backgroundTexture3);
    bgLayer2.setScale(2, 2);

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
        bgLayer.Update(tf);
        bgLayer2.Update(tf);
        clock.restart();
        window.clear();
        view.setCenter(640,player.getPosition().y);
        
        window.setView(view);
        //window.draw(backgroundSprite);
        window.draw(bgLayer);
        window.draw(bgLayer2);
        window.draw(player);
        window.display();
    }

    return 0;
}