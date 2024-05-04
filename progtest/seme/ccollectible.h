#pragma once
#include "cgameobject.h"

/** \class CCollectible
An abstract class that serves as the parent class of individual collectible items.s
*/
class CCollectible : public CGameObject
{
public:
    CCollectible(CPos pos) : pos(pos){};
    virtual void doEffect(CGameState &gamestate) = 0;
    virtual void update(CGameState &gamestate) override {}; // definition so that we can compile without .cpp source files.

protected:
    CPos pos;
    bool collected = false;
};

/** \class CCoin
A class that represents a coin that increases player score on collection.
*/
class CCoin : public CCollectible
{
public:
    CCoin(CPos pos) : CCollectible(pos){};
    virtual void doEffect(CGameState &gamestate) override {};
    virtual void draw() override {};
};

/** \class CPowerUp
A class that represents a power pelet that allows the player to eat the gosts for a limited time.
*/
class CPowerUp : public CCollectible
{
public:
    CPowerUp(CPos pos) : CCollectible(pos){};
    virtual void doEffect(CGameState &gamestate) override {};
    virtual void draw() override {};
};

/** \class CPortal
A class that represents a portal that allows teleport the player.
*/
class CPortal : public CCollectible
{
public:
    CPortal(CPos pos, CPos linkPos) : CCollectible(pos), linkPos(linkPos){};
    virtual void doEffect(CGameState &gamestate) override {};
    virtual void draw() override {};

private:
    CPos linkPos;
};