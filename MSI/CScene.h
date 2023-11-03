#pragma once

class CObject;

class CScene
{
private:
	vector<CObject*> m_vecObj[(UINT)GROUP_TYPE::END];
	wstring m_strName;

	UINT m_iTileX;		// Ÿ�� ���� ����
	UINT m_iTileY;		// Ÿ�� ���� ����


public:
	void SetName(const wstring _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }

	UINT GetTileX() { return m_iTileX; }
	UINT GetTileY() { return m_iTileY; }



	virtual void finalupdate();		// �۾��� ���������ִ� ������Ʈ
	virtual void render(HDC _dc);
	
	const vector<CObject*>& GetGroupObject(GROUP_TYPE _eType) { return m_vecObj[(UINT)_eType]; }

	vector<CObject*>& GetUIGroup() { return m_vecObj[(UINT)GROUP_TYPE::UI]; }

	virtual void update();
	virtual void Enter() = 0;	// �ش� Scene�� ���Խ� ȣ��
	virtual void Exit() = 0;	// �ش� Scene���� Ż��� ȣ��

public:
	void AddObject(CObject* _pObj, GROUP_TYPE _eType) { m_vecObj[(UINT)_eType].push_back(_pObj); }
	void DeleteGroup(GROUP_TYPE _eTarget);
	void DeleteAll();

	void CreateTile(UINT _iXCount, UINT _iYCount);
	void LoadTile(const wstring& _strRelativePath);


public:
	CScene();
	// CScene�� ��ӹ޴� �ֵ��� �Ҹ�� �� �ֵ���
	virtual ~CScene();
};

