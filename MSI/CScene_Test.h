#pragma once
#include "CScene.h"
class CScene_Test :
    public CScene
{

public:
    virtual void update() override;
    virtual void Enter();
    virtual void Exit();

public:
    CScene_Test();
    ~CScene_Test();
};

