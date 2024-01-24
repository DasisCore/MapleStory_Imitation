#pragma once

#include "CMonFactory.h"

class CObject;

struct MON_GEN_INFO
{
	MON_TYPE eMonType;
	wstring strName;
	Vec2 vPos;
	Vec2 vScale;
	bool bCollider;
	Vec2 vColOffset;
	Vec2 vColScale;
	bool bAnimaiton;
	vector<wstring> vecPath;
	bool bGravity;
	bool bRigidBody;
};

class CScene
{
private:
	vector<CObject*> m_vecObj[(UINT)GROUP_TYPE::END];
	wstring m_strName;
	
	Vec2 m_vMapSize;

	CObject* m_pPlayer;

	// 해당 맵에 어느정도의 몬스터가 초기에 존재하는지
	// 이를 토대로 매 시간마다 몬스터 젠을 시킬 것.
	int m_iMonGen;
	float m_GenCycle;
	vector<MON_GEN_INFO> m_vecMonGenData;


public:
	void SetName(const wstring _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }

	Vec2 GetMapSize() { return m_vMapSize; }
	void SetMapSize(Vec2 _vMapsize) { m_vMapSize = _vMapsize; }


	CObject* GetPlayer() { return m_pPlayer; }

	virtual void finalupdate();		// 작업을 마무리해주는 업데이트
	virtual void render(HDC _dc);

	const vector<CObject*>& GetGroupObject(GROUP_TYPE _eType) { return m_vecObj[(UINT)_eType]; }
	vector<CObject*>& GetUIGroup() { return m_vecObj[(UINT)GROUP_TYPE::UI]; }

	virtual void start();
	virtual void update();
	virtual void Enter() = 0;	// 해당 Scene에 진입시 호출
	virtual void Exit() = 0;	// 해당 Scene에서 탈출시 호출

public:
	void AddObject(CObject* _pObj, GROUP_TYPE _eType) { m_vecObj[(UINT)_eType].push_back(_pObj); }
	void RegisterPlayer(CObject* _pPlayer) { m_pPlayer = _pPlayer; }
	void DeleteGroup(GROUP_TYPE _eTarget);

	// UI를 포함한 모든 오브젝트 삭제
	void DeleteAll();
	// UI를 제외한 오브젝트 삭제
	void DeleteAll_Except_UI();

	void LoadSceneData(const wstring& _strFilePath);

	void MonsterGenCountCheck();
	void MonGenerate();

public:
	CScene();
	// CScene을 상속받는 애들이 소멸될 수 있도록
	virtual ~CScene();
};

