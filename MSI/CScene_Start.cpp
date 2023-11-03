#include "global.h"

#include "CScene_Start.h"
#include "CCore.h"
#include "CObject.h"
#include "CPlayer.h"
#include "CMonster.h"

#include "CTexture.h"
#include "CPathMgr.h"

#include "CComponent.h"
#include "CCollisionMgr.h"

#include "CSceneMgr.h"
#include "CKeyMgr.h"

#include "CCamera.h"

CScene_Start::CScene_Start()
{
}

CScene_Start::~CScene_Start()
{
}

void CScene_Start::update()
{
	// �θ� ������ �ִ� update() ���� ȣ��
	CScene::update();

	if (KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::TOOL);
	}

	if (KEY_TAP(KEY::LBTN))
	{
		Vec2 vLookAt = CCamera::GetInst()->GetRealPos(MOUSE_POS);
		CCamera::GetInst()->SetLookAt(vLookAt);
	}

}

void CScene_Start::Enter()
{
	// ���Խ� ������Ʈ �߰�
	CObject* pPlayer = new CPlayer;
	pPlayer->SetName(L"First Player");
	pPlayer->SetPos(Vec2(640.f, 384.f));
	pPlayer->SetScale(Vec2(100.f, 100.f)); 

	AddObject(pPlayer, GROUP_TYPE::PLAYER);
	//CreateObject(pPlayer, GROUP_TYPE::PLAYER);

	//CObject* pOtherPlayer = pPlayer->Clone();
	//pOtherPlayer->SetPos(Vec2(100.f, 400.f));
	//CreateObject(pOtherPlayer, GROUP_TYPE::PLAYER);


	//CCamera::GetInst()->SetTarget(pPlayer);



	// ���Խ� ������Ʈ �߰�
	CObject* pMonster = new CMonster;
	pMonster->SetName(L"First Monster");
	pMonster->SetPos(Vec2(640.f, 100.f));
	pMonster->SetScale(Vec2(100.f, 50.f));

	////AddObject(pMonster, GROUP_TYPE::MONSTER);


	//// ������Ʈ �߰��ϴ� �̺�Ʈ
	CreateObject(pMonster, GROUP_TYPE::MONSTER);


	// ex-> Ÿ�� �ε�



	// �浹 ����
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);

	// Camera Look ����
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	CCamera::GetInst()->SetLookAt(vResolution / 2.f);

}

void CScene_Start::Exit()
{
	DeleteAll();

	CCollisionMgr::GetInst()->Reset();
}


