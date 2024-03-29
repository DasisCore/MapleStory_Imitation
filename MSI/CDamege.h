#pragma once
#include "CObject.h"

enum class DAMEGE_TYPE
{
    NORED,
    VIOLET,
    END,
};


class CDamege :
    public CObject
{
private:
    // 데미지가 화면에 출력되는 시간.
    int m_iDamege;
    float m_fDisplay;

    Bitmap* m_pBitmap;
    Image* m_pImage;

    float m_fAlpha;

    Graphics* m_pGraphics;

public:
    void DrawWithAlpha(HDC hdc, Image* image, float alpha);

public:
    virtual void update() override;
    virtual void render(HDC _dc) override;
    CDamege* Clone() { return new CDamege(*this); }

public:
    CDamege(int _iDamege, DAMEGE_TYPE _eDamegeType);
    ~CDamege();
};

