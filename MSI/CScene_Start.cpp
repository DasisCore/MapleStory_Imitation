#include "global.h"

#include "CScene_Start.h"
#include "CObject.h"

CScene_Start::CScene_Start()
{
}

CScene_Start::~CScene_Start()
{
}

void CScene_Start::Enter()
{
	// ���Խ� ������Ʈ �߰�
	CObject* pObj = new CObject;
	pObj->SetName(L"First Object");
	pObj->SetPos(Vec2(640.f, 384.f));
	pObj->SetScale(Vec2(100.f, 100.f));

	AddObject(pObj, GROUP_TYPE::PLAYER);
}

void CScene_Start::Exit()
{
}


