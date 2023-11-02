#pragma once
#include "CUI.h"

class CPanelUI :
    public CUI
{
private:
    Vec2 m_vDragStart;


public:
    virtual void update() override;
    virtual void render(HDC _dc) override;


    virtual void MouseOn();
    virtual void MouseLbtnDown();
    virtual void MouseLbtnUp();
    virtual void MouseLbtnClicked();

    virtual CPanelUI* Clone() override { return new CPanelUI(*this); }

public:
    CPanelUI();
    ~CPanelUI();
};

