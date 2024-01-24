#pragma once
#include "CObject.h"

class CEffect :
    public CObject
{
private:
    wstring m_strFilePath;
    Graphics* m_pGraphics;
    CObject* m_pOwner;

    int m_iCurFrame;
    int m_iEndFrame;
    float m_fFrameDuration;
    float m_fCurTime;

    vector<Image*> m_vecImage;

    float m_fWidth;
    float m_fHeight;

    int m_iDir;
    Vec2 m_vOffset;

public:
    virtual void update() override;
    virtual void render(HDC _dc) override;
    CEffect* Clone() { return new CEffect(*this); }

public:
    CEffect(CObject* _pObj, wstring _strPath, int _iEndFrame, float _fDuration, int _iDir, Vec2 _vOffset = Vec2(0.f, 0.f));
    ~CEffect();

};

