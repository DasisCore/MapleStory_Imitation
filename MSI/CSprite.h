#pragma once
#include "CObject.h"


class CSprite :
    public CObject
{
    Image* m_pImage;
    // 해당 이미지가 타겟팅 되었는지 확인
    bool m_bTarget;
    float m_fRatio;

    Vec2 m_vPrevPos;

private:
    // 이미지의 테두리를 그리는 함수.
    void DrawLine(HDC _dc);


public:
    virtual void update() override;
    virtual void render(HDC _dc) override;
    CSprite* Clone() { return new CSprite(*this); }

public:
    CSprite(wstring _strAbsolutePath);
    ~CSprite();
};

