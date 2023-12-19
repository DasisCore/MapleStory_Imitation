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
    Vec2 m_vMapSize;
    bool m_bGizmo;

    CUI* m_pUI;
    MOUSE_TOOL_TYPE m_eType;

    bool m_bDrag;
    Vec2 m_vDragStart;

public:
    void SetTileIdx();

    void SaveTileData();
    void LoadTileData();
    void SaveTile(const wstring& _strFilePath);

    void ChangeTool(int _i) { m_eType = (MOUSE_TOOL_TYPE)_i; }
    void toggleGizmo() { m_bGizmo = !m_bGizmo; };
    void CreateMap(int _iWidth, int _iHeight);
    void ResetMap();


private:
    // FootHold�� �����ϱ� ���� �巡�� �ϴ� ���
    void FootHoldDrag();
    void renderDrag(HDC _dc);
    void CreateFootHold();

public:
    void DrawGizmo(HDC _dc);

    // ���� ũ�Ⱑ �������ٸ�, ������Ʈ�� ��ġ�� ���� ������ ���� ȸ��ó���Ѵ�.
    void renderMapBase(HDC _dc);

public:
    virtual void update() override;
    virtual void render(HDC _dc) override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CScene_Tool();
    ~CScene_Tool();

};

