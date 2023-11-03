#pragma once
#include "CObject.h"

class CTexture;

class CTile :
    public CObject
{
private:
    CTexture* m_pTileTex;
    int m_iImgIdx;

public:
    void SetTexture(CTexture* _pTex) { m_pTileTex = _pTex; }

    void AddImgIdx() { m_iImgIdx++; }


public:
    virtual void update() override;
    virtual void render(HDC _dc) override;

    virtual void Save(FILE* _pFile);
    virtual void Load(FILE* _pFile);

    virtual CTile* Clone() override { return new CTile(*this); }

public:
    CTile();
    ~CTile();
};

