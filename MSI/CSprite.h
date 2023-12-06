#pragma once
#include "CObject.h"


struct tSpriteInfo
{
    // 해당 이미지가 타겟팅 되었는지
    bool bTarget;
    
    // 이미지의 원래 크기
    float fOriginWidth;
    float fOriginHeight;

    // 이미지의 현재 비율
    float fWidthRatio;
    float fHeightRatio;

    // 이미지의 현재 크기
    float fCurWidth;
    float fCurHeight;

    // 이미지의 가로 세로 비율
    double dRatio;
};

class CSprite :
    public CObject
{
    Image* m_pImage;
    tSpriteInfo m_tInfo;
    
    bool m_bMouseOn;
    int m_iHandle;
    Vec2 m_vDragStart;

private:
    // 이미지의 테두리를 그리는 함수.
    void DrawBorder(HDC _dc);

    // 이미지를 클릭했을 경우 처리
    void IsLbtnDown();

    // 이미지를 드래그 했을 경우 처리
    void IsLbtnDrag();

    // 마우스가 테두리 영역에 있는지 확인하는 함수
    bool IsPointInBorder();

    // 테두리 핸들 그리기
    void renderImageHandle(HDC _dc);

    // 테두리 핸들에 마우스가 위치해있는지 
    int IsHandleIn();

    // 테두리 핸들을 잡고 드래그 했을 경우.
    void HandleDrag(int _point);

public:
    void update_tInfo();
    void SetTarget(bool _b) { m_tInfo.bTarget = _b; }
    Image* GetSprite() { return m_pImage; }


public:
    virtual void update() override;
    virtual void render(HDC _dc) override;
    CSprite* Clone() { return new CSprite(*this); }

public:
    CSprite(wstring _strAbsolutePath);
    ~CSprite();
};

