#pragma once
#include "cgameobject.h"

class CGhost : public CGameObject
{
public:
    CGhost(CPos pos) : currentPos(pos){};
    virtual void update(CGameState &gamestate) override {};
    virtual void getNextPos(CGameState &gamestate) = 0;

protected:
    CPos currentPos;
    CPos nextPos;
    CPos targetPos;
};

class CManhattan : public CGhost
{
public:
    CManhattan(CPos pos) : CGhost(pos){};
    virtual void getNextPos(CGameState &gamestate) override {};
    virtual void draw() override {};
};

class CEuclid : public CGhost
{
public:
    CEuclid(CPos pos) : CGhost(pos){};
    virtual void getNextPos(CGameState &gamestate) override {};
    virtual void draw() override {};
};

class CMax : public CGhost
{
public:
    CMax(CPos pos) : CGhost(pos){};
    virtual void getNextPos(CGameState &gamestate) override {};
    virtual void draw() override {};
};