#include "../include/TextureManager.h"

TextureManager::TextureManager()
{
    if (!rockTexture.loadFromFile("../assets/rock.png")) {
        std::cout << "ERROR: Couldn't load texture"; 
    }

    if (!paperTexture.loadFromFile("../assets/paper.png")) {
        std::cout << "ERROR: Couldn't load texture"; 
    }

    if (!scissorsTexture.loadFromFile("../assets/scissors.png")) {
        std::cout << "ERROR: Couldn't load texture"; 
    }
}