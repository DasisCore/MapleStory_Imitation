#pragma once
#include "CScene.h"
class CScene_Tool :
    public CScene
{
private:
    CUI* m_pUI;

public:
    void SetTileIdx();

    void SaveTileData();
    void LoadTileData();
    void SaveTile(const wstring& _strFilePath);




public:
    virtual void update() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CScene_Tool();
    ~CScene_Tool();

};

