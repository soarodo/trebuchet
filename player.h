#ifndef PLAYER_H
#define PLAYER_H

#include<iostream>
#include<String>
#include<QString>

using namespace std;
class Player
{
private:
    int setRemaining;
    QString namePlayer;
    int levelDifficulty;
    int pointTotal;
    int pointPrevious;
public:
    Player(QString name,int levelDifficulty);
    void addPoints(int point);
    int getPointTotal(){return pointTotal;}
    int getPointPrevious(){return pointPrevious;}
    int getSetRemaining(){return setRemaining;}
    int getLevelDifficulty(){return levelDifficulty;}
    QString getName(){return namePlayer;}

};

#endif // PLAYER_H
