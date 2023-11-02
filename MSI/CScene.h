#pragma once

class CObject;

class CScene
{
private:
	vector<CObject*> m_vecObj[(UINT)GROUP_TYPE::END];
	wstring m_strName;

	UINT m_iTileX;		// 타일 가로 개수
	UINT m_iTileY;		// 타일 세로 개수


public:
	void SetName(const wstring _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }

	UINT GetTileX() { return m_iTileX; }
	UINT GetTileY() { return m_iTileY; }



	virtual void finalupdate();		// 작업을 마무리해주는 업데이트
	virtual void render(HDC _dc);
	
	const vector<CObject*>& GetGroupObject(GROUP_TYPE _eType) { return m_vecObj[(UINT)_eType]; }

	vector<CObject*>& GetUIGroup() { return m_vecObj[(UINT)GROUP_TYPE::UI]; }

	virtual void update();
	virtual void Enter() = 0;	// 해당 Scene에 진입시 호출
	virtual void Exit() = 0;	// 해당 Scene에서 탈출시 호출

public:
	void AddObject(CObject* _pObj, GROUP_TYPE _eType) { m_vecObj[(UINT)_eType].push_back(_pObj); }
	void DeleteGroup(GROUP_TYPE _eTarget);
	void DeleteAll();

	void CreateTile(UINT _iXCount, UINT _iYCount);


public:
	CScene();
	// CScene을 상속받는 애들이 소멸될 수 있도록
	virtual ~CScene();
};

