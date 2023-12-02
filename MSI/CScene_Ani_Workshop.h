#pragma once
#include "CScene.h"

class CScene_Ani_Workshop :
    public CScene
{
private:
    Image* m_pImage;


    // Workshop에서 사용하는 해상도 (좀 더 커지므로 따로 보관함)
    int m_iScreenWidth;
    int m_iScreenHeight;


    // 불러온 이미지의 배율
    float m_fRatio;

public:
    virtual void update() override;
    virtual void render(HDC _dc) override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    void LoadTexture();

private:
    // 이미지를 불러올 때, 확장자를 확인하는 함수.
    bool CheckImageFormat(wstring _wStr);


public:
    CScene_Ani_Workshop();
    ~CScene_Ani_Workshop();
};

