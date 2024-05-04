#pragma once
#include "cgameobject.h"

class CCollectible : public CGameObject
{
public:
    CCollectible(CPos pos) : pos(pos){};
    virtual void doEffect(CGameState &gamestate) = 0;
    virtual void update(CGameState &gamestate) override {};

protected:
    CPos pos;
    bool collected = false;
};

class CCoin : public CCollectible
{
public:
    CCoin(CPos pos) : CCollectible(pos){};
    virtual void doEffect(CGameState &gamestate) override {};
    virtual void draw() override {};
};

class CPowerUp : public CCollectible
{
public:
    CPowerUp(CPos pos) : CCollectible(pos){};
    virtual void doEffect(CGameState &gamestate) override {};
    virtual void draw() override {};
};

class CPortal : public CCollectible
{
public:
    CPortal(CPos pos, CPos linkPos) : CCollectible(pos), linkPos(linkPos){};
    virtual void doEffect(CGameState &gamestate) override {};
    virtual void draw() override {};

private:
    CPos linkPos;
};