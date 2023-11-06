#pragma once
#include "CState.h"
class CTraceState :
    public CState
{
private:


public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void update() override;

public:
    CTraceState();
    ~CTraceState();
};

