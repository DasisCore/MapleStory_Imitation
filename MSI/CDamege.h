#pragma once
#include "CObject.h"

class CDamege :
    public CObject
{
private:
    // �������� ȭ�鿡 ��µǴ� �ð�.
    int m_iDamege;
    float m_fDisplay;

    Bitmap* m_pBitmap;
    Image* m_pImage;

    float m_fAlpha;

public:
    void DrawWithAlpha(HDC hdc, Image* image, float alpha);

public:
    virtual void update() override;
    virtual void render(HDC _dc) override;
    CDamege* Clone() { return new CDamege(*this); }

public:
    CDamege(int _iDamege);
    ~CDamege();
};

