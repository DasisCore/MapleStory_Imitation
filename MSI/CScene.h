#pragma once

#include "CMonFactory.h"

class CObject;

class CScene
{
private:
	vector<CObject*> m_vecObj[(UINT)GROUP_TYPE::END];
	wstring m_strName;
	
	Vec2 m_vMapSize;

	CObject* m_pPlayer;


public:
	void SetName(const wstring _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }

	Vec2 GetMapSize() { return m_vMapSize; }
	void SetMapSize(Vec2 _vMapsize) { m_vMapSize = _vMapsize; }


	CObject* GetPlayer() { return m_pPlayer; }

	virtual void finalupdate();		// �۾��� ���������ִ� ������Ʈ
	virtual void render(HDC _dc);

	const vector<CObject*>& GetGroupObject(GROUP_TYPE _eType) { return m_vecObj[(UINT)_eType]; }
	vector<CObject*>& GetUIGroup() { return m_vecObj[(UINT)GROUP_TYPE::UI]; }

	virtual void start();
	virtual void update();
	virtual void Enter() = 0;	// �ش� Scene�� ���Խ� ȣ��
	virtual void Exit() = 0;	// �ش� Scene���� Ż��� ȣ��

public:
	void AddObject(CObject* _pObj, GROUP_TYPE _eType) { m_vecObj[(UINT)_eType].push_back(_pObj); }
	void RegisterPlayer(CObject* _pPlayer) { m_pPlayer = _pPlayer; }
	void DeleteGroup(GROUP_TYPE _eTarget);

	// UI�� ������ ��� ������Ʈ ����
	void DeleteAll();
	// UI�� ������ ������Ʈ ����
	void DeleteAll_Except_UI();

	void LoadSceneData(const wstring& _strFilePath);

public:
	CScene();
	// CScene�� ��ӹ޴� �ֵ��� �Ҹ�� �� �ֵ���
	virtual ~CScene();
};

