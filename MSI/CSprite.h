#pragma once
#include "CObject.h"


struct tSpriteInfo
{
    // �ش� �̹����� Ÿ���� �Ǿ�����
    bool bTarget;
    
    // �̹����� ���� ũ��
    float fOriginWidth;
    float fOriginHeight;

    // �̹����� ���� ����
    float fWidthRatio;
    float fHeightRatio;

    // �̹����� ���� ũ��
    float fCurWidth;
    float fCurHeight;

    // �̹����� ���� ���� ����
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
    // �̹����� �׵θ��� �׸��� �Լ�.
    void DrawBorder(HDC _dc);

    // �̹����� Ŭ������ ��� ó��
    void IsLbtnDown();

    // �̹����� �巡�� ���� ��� ó��
    void IsLbtnDrag();

    // ���콺�� �׵θ� ������ �ִ��� Ȯ���ϴ� �Լ�
    bool IsPointInBorder();

    // �׵θ� �ڵ� �׸���
    void renderImageHandle(HDC _dc);

    // �׵θ� �ڵ鿡 ���콺�� ��ġ���ִ��� 
    int IsHandleIn();

    // �׵θ� �ڵ��� ��� �巡�� ���� ���.
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

