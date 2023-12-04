#pragma once
#include "CObject.h"
class CMarquee :
    public CObject
{
private:
    // 자신이 타겟이 되었는지
    bool m_bTarget;

    // 드래그 시작 지점.
    Vec2 m_vDragStart;


public:
    virtual void update() override;
    virtual void render(HDC _dc) override;
    CMarquee* Clone() { return new CMarquee(*this); }

public:
    void SetTarget(bool _b) { m_bTarget = _b; }

public:
    CMarquee();
    ~CMarquee();
};

