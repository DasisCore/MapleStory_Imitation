#include "global.h"
#include "CDamegeFactory.h"

#include "CDamege.h"
#include "CObject.h"

#include "CScene.h"
#include "CSceneMgr.h"

#include "CComponent.h"
#include "CCollider.h"


void CDamegeFactory::CreateSingleDamege(CObject* _pObj, int _iDamege, DAMEGE_TYPE _eDamegeType)
{
	CDamege* pDamege = new CDamege(_iDamege, _eDamegeType);
	pDamege->SetName(L"DAMEGE");
	Vec2 vPos = _pObj->GetComponent()->GetCollider()->GetFinalPos();
	Vec2 vScale = _pObj->GetComponent()->GetCollider()->GetScale();
	pDamege->SetPos(Vec2(vPos.x, vPos.y - (vScale.y / 2.f)));

	CSceneMgr::GetInst()->GetCurScene()->AddObject(pDamege, GROUP_TYPE::DAMEGE);
}




