#pragma once
#include "BaseState.h"
#include "Game.h"

class DifficultyState : public BaseState
{
public:
    DifficultyState(Game* game);

    void processInput(sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Texture& mBackgroundTexture;
    sf::Sprite mBackgroundSprite;

    sf::Font& mFont;

    // Các nút bấm dạng chữ
    sf::Text mTitleText;
    sf::Text mEasyText;
    sf::Text mNormalText;
    sf::Text mHardText;
};
