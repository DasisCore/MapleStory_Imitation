#pragma once
#include "CObject.h"
class CEffect :
    public CObject
{
private:
    CObject* m_pOwner;
    int m_iCurFrame;
    int m_iEndFrame;
    float m_fFrameDuration;
    float m_fCurTime;
    wstring m_strFilePath;
    Image* m_pImage;

public:
    virtual void update() override;
    virtual void render(HDC _dc) override;
    CEffect* Clone() { return new CEffect(*this); }

public:
    CEffect(CObject* _pObj, int _iEndFrame, float _fDuration);
    ~CEffect();

};

