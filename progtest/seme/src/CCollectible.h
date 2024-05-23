#pragma once
#include "CGameObject.h"

/** \class CCollectible
An abstract class that serves as the parent class of individual collectible items.s
*/
class CCollectible : public CGameObject
{
public:
    CCollectible(CPos pos) : pos(pos){};
    virtual void doEffect(CGameState &gamestate) = 0;
    virtual void update(CGameState &gamestate, double deltaTime) override;

protected:
    CPos pos;
    bool collected = false;
};