#pragma once
#include "CScene.h"

enum class MOUSE_TOOL_TYPE
{
    DEFAULT,
    FOOTHOLD,
};

class CScene_Tool :
    public CScene
{
private:
    CUI* m_pUI;
    MOUSE_TOOL_TYPE m_eType;

    bool m_bGizmo;

public:
    void SetTileIdx();

    void SaveTileData();
    void LoadTileData();
    void SaveTile(const wstring& _strFilePath);

    void ChangeTool(int _i) { m_eType = (MOUSE_TOOL_TYPE)_i; }



public:

    void DrawGizmo(HDC _dc);



public:
    virtual void update() override;
    virtual void render(HDC _dc) override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CScene_Tool();
    ~CScene_Tool();

};

