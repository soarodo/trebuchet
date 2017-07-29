#include "player.h"

Player::Player(QString name,int levelDifficulty)
{
    this->namePlayer=name;
    this->levelDifficulty=levelDifficulty;
    this->pointPrevious=0;
    this->pointTotal=0;
    this->setRemaining=10;
}

void Player::addPoints(int point)
{
    this->pointPrevious=point;
    this->pointTotal+=point;
    this->setRemaining--;
}
