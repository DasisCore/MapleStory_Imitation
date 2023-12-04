#pragma once
#include "CObject.h"
class CMarquee :
    public CObject
{
private:
    // �ڽ��� Ÿ���� �Ǿ�����
    bool m_bTarget;

    // �巡�� ���� ����.
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

