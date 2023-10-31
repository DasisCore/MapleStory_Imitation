#pragma once

class CObject;

class CScene
{
private:
	vector<CObject*> m_vecObj[(UINT)GROUP_TYPE::END];
	wstring m_strName;


public:
	void SetName(const wstring _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }

	virtual void finalupdate();		// �۾��� ���������ִ� ������Ʈ
	virtual void render(HDC _dc);
	
	const vector<CObject*>& GetGroupObject(GROUP_TYPE _eType) { return m_vecObj[(UINT)_eType]; }

	virtual void update();
	virtual void Enter() = 0;	// �ش� Scene�� ���Խ� ȣ��
	virtual void Exit() = 0;	// �ش� Scene���� Ż��� ȣ��

public:
	void AddObject(CObject* _pObj, GROUP_TYPE _eType) { m_vecObj[(UINT)_eType].push_back(_pObj); }
	void DeleteGroup(GROUP_TYPE _eTarget);
	void DeleteAll();

public:
	CScene();
	// CScene�� ��ӹ޴� �ֵ��� �Ҹ�� �� �ֵ���
	virtual ~CScene();
};
