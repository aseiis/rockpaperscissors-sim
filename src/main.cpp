#include <SFML/Graphics.hpp>
#include <iostream>
#include <format>
#include <chrono>
#include <vector>
#include <random>
#include "../include/Object.h"
#include "../include/TextureManager.h"

enum GAME_MODE {
    SIM_MODE = 0,
    PLAYER_MODE = 1
};

float distance(const sf::Vector2f& p1, const sf::Vector2f& p2) {
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    return std::sqrt(dx * dx + dy * dy);
}

void updateFps(auto& lastTimestamp, int& framecount, int& fps);
void spawn(int amount, const sf::Vector2f limits[2], std::vector<Object>& parentCollection);

int main()
{
    // FLAGS______________________
    const bool DEBUG_MODE = false;
    int CUR_GAME_MODE = PLAYER_MODE;
    //____________________________

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

    std::vector<Object*> objects;
    int curObjectID = 0;
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
        Object* obj = new Object(&textureManager, curObjectID);
        obj->m_type = static_cast<ObjectType>(objt_distr(gen));
        obj->setPosition(sf::Vector2f(xdistr(gen), ydistr(gen)));
        obj->m_velocity = sf::Vector2f(fdistr(gen), fdistr(gen));
        obj->updateTexture();
        
        objects.push_back(obj);
    }

    Object* player = nullptr;
    float playerSpeed = 1.f;
    int playerID = curObjectID;

    sf::Text playerLabel;
    playerLabel.setFont(defFont);
    playerLabel.setCharacterSize(12);
    playerLabel.setString("Player");

    if(CUR_GAME_MODE == GAME_MODE::PLAYER_MODE)
    {
        player = new Object(&textureManager, curObjectID);
        player->m_type = ObjectType::SCISSORS;
        player->updateTexture();
        player->setPosition(sf::Vector2f(0, 0));
        player->m_velocity = sf::Vector2f(0, 0);
        objects.push_back(player);
        
        playerLabel.setPosition(sf::Vector2f(player->getPosition().x - 2, player->getPosition().y + 2));
    }

    
    //====

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        sf::Vector2f playerMovement = sf::Vector2f(0, 0);
        
        if(event.type == sf::Event::KeyPressed){
            if(event.key.code == sf::Keyboard::Escape){
                window.close();
            }
            if(CUR_GAME_MODE == PLAYER_MODE && event.key.code == sf::Keyboard::Space){
                player->mutateToNext();
            }
        }

        if(CUR_GAME_MODE == PLAYER_MODE) {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)){
                playerMovement.y = -1 * playerSpeed;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
                playerMovement.y = 1 * playerSpeed;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
                playerMovement.x = -1 * playerSpeed;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
                playerMovement.x = 1 * playerSpeed;
            }

            player->m_velocity = playerMovement;
            //std::cout << "player movement: " << playerMovement.x << " " << playerMovement.y << "\n";
            //std::cout << "player velocity: " << player->m_velocity.x << " " << player->m_velocity.y << "\n";
        }
        
        // Checking collision with boundaries and move accordingly

        for(auto& obj : objects){
            if(obj->m_ID != playerID){
                if(obj->getPosition().x <= bottomLeftZoneLimit.x){
                    obj->m_velocity.x = -obj->m_velocity.x;
                }
                if(obj->getPosition().y <= bottomLeftZoneLimit.y){
                    obj->m_velocity.y = -obj->m_velocity.y;
                }
                if(obj->getPosition().x >= topRightZoneLimit.x){
                    obj->m_velocity.x = -obj->m_velocity.x;
                }
                if(obj->getPosition().y >= topRightZoneLimit.y){
                    obj->m_velocity.y = -obj->m_velocity.y;
                }
            }
            obj->move();
        }

        // Checking collisions with other objects
        // Possible enhancements: 
        // 1. Cache collisions
        // 2. Use a grid system

        std::vector<Object*>::iterator objIterator = objects.begin();

        for(objIterator; objIterator != objects.end(); objIterator++){
            Object* currentObj = *objIterator;
            //std::cout << "Object " << objIterator->m_ID << " :" << std::endl;
            for(auto& extobj : objects){
                if(currentObj->m_ID != extobj->m_ID) {
                    if(distance(currentObj->getPosition(), extobj->getPosition()) < 20 ) {
                            //std::cout << "Obj type: " << static_cast<int>(obj.m_type) << " // Extobj type: " << static_cast<int>(extobj.m_type) << std::endl; 
                            int battleRes = (currentObj->m_type - extobj->m_type + 3) % 3;
                            if(battleRes == 1){
                                extobj->mutate(currentObj->m_type);
                            } else if(battleRes == 2) {
                                currentObj->mutate(extobj->m_type);
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
            objectsCount[obj->m_type]++;
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
        
        if(CUR_GAME_MODE == PLAYER_MODE) {
            playerLabel.setPosition(sf::Vector2f(player->getPosition().x - 16, player->getPosition().y + 16));
            window.draw(playerLabel);
        }

        for(auto& obj : objects){
            if(obj->m_ID >-1)
                obj->draw(window, DEBUG_MODE);
        }

        window.display();
    }

    // Clean!
    for(auto ptr : objects)
    {
        delete ptr;
    };

    objects.clear();
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
