#include <SFML/Graphics.hpp>
#include <iostream>
#include <format>
#include <chrono>
#include <vector>
#include <random>
#include "../include/Object.h"
#include "../include/TextureManager.h"

float distance(const sf::Vector2f& p1, const sf::Vector2f& p2) {
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    return std::sqrt(dx * dx + dy * dy);
}

void updateFps(auto& lastTimestamp, int& framecount, int& fps);
void spawn(int amount, const sf::Vector2f limits[2], std::vector<Object>& parentCollection);

int main()
{
    const bool DEBUG_MODE = false;

    sf::Vector2f bottomLeftZoneLimit(200, 200);
    sf::Vector2f topRightZoneLimit(1600, 800);
    const sf::Vector2f zoneLimits[2] = { bottomLeftZoneLimit, topRightZoneLimit };

    uint windowWidth = 1920;
    uint windowHeight = 1080;
    auto window = sf::RenderWindow{ { windowWidth, windowHeight }, "Rock Paper Scissors" };
    window.setFramerateLimit(144);

    //====

    // Game initialization

    sf::Font defFont;
    if (!defFont.loadFromFile("../assets/vera.ttf")) {
        std::cout << "ERROR: Couldn't load default font!";
    }
    
    TextureManager textureManager = TextureManager();

    int fps = 0;
    auto lastTimestamp = std::chrono::system_clock::now();
    int frameCount = 0;

    sf::Text fpsLabel;
    fpsLabel.setFont(defFont);
    fpsLabel.setCharacterSize(12);

    int objectsCount[3] = {-1, -1, -1};

    sf::Text rockCountLabel;
    rockCountLabel.setFont(defFont);
    rockCountLabel.setCharacterSize(12);
    rockCountLabel.setPosition(sf::Vector2f(0, 20));

    sf::Text paperCountLabel;
    paperCountLabel.setFont(defFont);
    paperCountLabel.setCharacterSize(12);
    paperCountLabel.setPosition(sf::Vector2f(0, 40));

    sf::Text scissorsCountLabel;
    scissorsCountLabel.setFont(defFont);
    scissorsCountLabel.setCharacterSize(12);
    scissorsCountLabel.setPosition(sf::Vector2f(0, 60));

    std::vector<Object> objects;
    int objectsAmount = 100;

    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator

    std::uniform_int_distribution<> objt_distr(0, 2);
    std::uniform_int_distribution<> xdistr(zoneLimits[0].x, zoneLimits[1].x);
    std::uniform_int_distribution<> ydistr(zoneLimits[0].y, zoneLimits[1].y); 
    std::uniform_real_distribution<> fdistr(-1.0, 1.0);

    // Objects initialization and texture setting

    for(int i=0; i<objectsAmount; i++)
    {
        Object obj(&textureManager, i);
        obj.type = static_cast<ObjectType>(objt_distr(gen));
        obj.setPosition(sf::Vector2f(xdistr(gen), ydistr(gen)));
        obj.velocity = sf::Vector2f(fdistr(gen), fdistr(gen));
        
        switch (obj.type) {
            case ObjectType::ROCK:
                obj.setTexture(textureManager.rockTexture);
                break;
            case ObjectType::PAPER:
                obj.setTexture(textureManager.paperTexture);
                break;
            case ObjectType::SCISSORS:
                obj.setTexture(textureManager.scissorsTexture);
                break;
            default:
                std::cout << "ERROR: Couldn't load texture based on type: type detection failed";
                break;
        }
        
        objects.push_back(obj);
    }

    //====

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if(event.type == sf::Event::KeyPressed){
                if(event.key.code == sf::Keyboard::Escape){
                    window.close();
                }
            }
        }

        // Checking collision with boundaries and move accordingly

        for(auto& obj : objects){
            if(obj.getPosition().x <= bottomLeftZoneLimit.x){
                obj.velocity.x = -obj.velocity.x;
            }
            if(obj.getPosition().y <= bottomLeftZoneLimit.y){
                obj.velocity.y = -obj.velocity.y;
            }
            if(obj.getPosition().x >= topRightZoneLimit.x){
                obj.velocity.x = -obj.velocity.x;
            }
            if(obj.getPosition().y >= topRightZoneLimit.y){
                obj.velocity.y = -obj.velocity.y;
            }
            obj.move();
        }

        // Checking collisions with other objects
        // Possible enhancements: 
        // 1. Cache collisions
        // 2. Use a grid system

        std::vector<Object>::iterator objIterator = objects.begin();

        for(objIterator; objIterator != objects.end(); objIterator++){
            //std::cout << "Object " << objIterator->ID << " :" << std::endl;
            for(auto& extobj : objects){
                if(objIterator->ID != extobj.ID) {
                    if(distance(objIterator->getPosition(), extobj.getPosition()) < 20 ) {
                            //std::cout << "Obj type: " << static_cast<int>(obj.type) << " // Extobj type: " << static_cast<int>(extobj.type) << std::endl; 
                            int battleRes = (objIterator->type - extobj.type + 3) % 3;
                            if(battleRes == 1){
                                extobj.mutate(objIterator->type);
                            } else if(battleRes == 2) {
                                objIterator->mutate(extobj.type);
                            }
                            break;
                    }
                }
            }
        }

        // Counting number of objects of x type

        objectsCount[0] = 0;
        objectsCount[1] = 0;
        objectsCount[2] = 0;

        for(auto& obj : objects){
            objectsCount[obj.type]++;
        }

        // UI

        rockCountLabel.setString(std::format("Rock: {}", objectsCount[0]));
        paperCountLabel.setString(std::format("Paper: {}", objectsCount[1]));
        scissorsCountLabel.setString(std::format("Scissors: {}", objectsCount[2]));

        updateFps(lastTimestamp, frameCount, fps);
        fpsLabel.setString(std::format("FPS: {}", fps));

        // DRAW

        window.clear();

        window.draw(fpsLabel);
        window.draw(rockCountLabel);
        window.draw(paperCountLabel);
        window.draw(scissorsCountLabel);
        
        for(auto& obj : objects){
            if(obj.ID >-1)
                obj.draw(window, DEBUG_MODE);
        }

        window.display();
    }
}

void updateFps(auto& lastTimestamp, int& currentFrameCount, int& fps)
{
    auto currentTimestamp = std::chrono::system_clock::now();
    std::chrono::duration<float> elapsedSeconds = currentTimestamp - lastTimestamp;
    currentFrameCount++;

    if (elapsedSeconds.count() >= 1.0f)
    {
        fps = currentFrameCount / elapsedSeconds.count();
        lastTimestamp = currentTimestamp;
        currentFrameCount = 0;
    }
}
