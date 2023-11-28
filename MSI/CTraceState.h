#pragma once
#include "CState.h"

class CObject;

class CTraceState :
    public CState
{
private:
    CObject* m_pObj;


public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void update() override;

public:
    CTraceState();
    ~CTraceState();
};

