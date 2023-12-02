#pragma once
#include "CObject.h"


class CSprite :
    public CObject
{
    Image* m_pImage;
    // �ش� �̹����� Ÿ���� �Ǿ����� Ȯ��
    bool m_bTarget;
    float m_fRatio;

    Vec2 m_vPrevPos;

private:
    // �̹����� �׵θ��� �׸��� �Լ�.
    void DrawLine(HDC _dc);


public:
    virtual void update() override;
    virtual void render(HDC _dc) override;
    CSprite* Clone() { return new CSprite(*this); }

public:
    CSprite(wstring _strAbsolutePath);
    ~CSprite();
};

