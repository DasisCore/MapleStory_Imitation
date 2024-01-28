#include "global.h"
#include "CCamera.h"

#include "CCore.h"
#include "CObject.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CResMgr.h"
#include "CTexture.h"

#include "CSceneMgr.h"
#include "CScene.h"

CCamera::CCamera()
	: m_pTargetObj(nullptr)
	, m_fTime(0.f)
	, m_fSpeed(300.f)
	, m_fAccTime(0.f)
	, m_pVeilTex(nullptr)
	, m_fTraceRange(120.f)
{
}

CCamera::~CCamera()
{

}

void CCamera::SetTarget(CObject* _pTarget)
{
	m_vLookAt = GetRealPos(_pTarget->GetPos());
	m_vPrevLookAt = GetRealPos(_pTarget->GetPos());
	m_vCurLookAt = GetRealPos(_pTarget->GetPos());
	m_vDiff = GetRealPos(_pTarget->GetPos());
	m_pTargetObj = _pTarget;
}

void CCamera::init()
{
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	m_pVeilTex = CResMgr::GetInst()->CreateTexture(L"CameraVeil", (UINT)vResolution.x, (UINT)vResolution.y);
}

void CCamera::update()
{
	if (m_pTargetObj)
	{
		// 타겟 오브젝트가 죽을 예정이라면
		if (m_pTargetObj->IsDead()) m_pTargetObj = nullptr;
		else
		{
			if (m_fTime <= m_fAccTime)
			{
				m_vLookAt = m_pTargetObj->GetPos();
			}
		}
	}

	if (KEY_HOLD(KEY::W))
	{
		m_vLookAt.y -= 500.f * fDT;
	}

	if (KEY_HOLD(KEY::S))
	{
		m_vLookAt.y += 500.f * fDT;
	}

	if (KEY_HOLD(KEY::D))
	{
		m_vLookAt.x += 500.f * fDT;
	}

	if (KEY_HOLD(KEY::A))
	{
		m_vLookAt.x -= 500.f * fDT;
	}

	// 화면 중앙좌표와 카메라 LookAt 좌표간의 차이값 계산
	CalDiff();
}

void CCamera::render(HDC _dc)
{
	if (CCore::GetInst()->GetRenderOption())
	{
		// 카메라의 추적 범위 그려보기
		Graphics graphics(_dc);
		Pen pen(Color(247, 206, 0), 4);
		Vec2 vResolution = CCore::GetInst()->GetResolution();
		vResolution = vResolution / 2.f;
		graphics.DrawEllipse(&pen, vResolution.x - m_fTraceRange, vResolution.y - m_fTraceRange, m_fTraceRange * 2, m_fTraceRange * 2);

		// 카메라의 현재 위치 랜더링
		Font font(L"Arial", 12);
		SolidBrush brush(Color(0, 0, 0));
		string temp = "Camera Positon : " + std::to_string((int)m_vCurLookAt.x) + " / " + std::to_string((int)m_vCurLookAt.y);
		wstring t = wstring(temp.begin(), temp.end());
		graphics.DrawString(t.c_str(), -1, &font, PointF(10.f, 100.f), &brush);
	}


	if (m_listEffect.empty()) return;

	tCamEffect &effect =	m_listEffect.front();
	effect.fCurTime += fDT;

	float fRatio = 0.f;	// 진행 비율

	if (fRatio < 0.f) fRatio = 0.f;
	if (fRatio > 1.f) fRatio = 1.f;

	fRatio = effect.fCurTime / effect.fDuration;

	int iAlpha = 0;

	if (effect.eEffect == CAM_EFFECT::FADE_OUT)
	{
		iAlpha = (int)(255.f * fRatio);
	}

	if (effect.eEffect == CAM_EFFECT::FADE_OUT)
	{
		iAlpha = (int)(255.f * (1.f - fRatio));
	}

	BLENDFUNCTION bf = {};
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = 0;
	bf.SourceConstantAlpha = iAlpha;

	AlphaBlend(_dc, 0, 0
		, m_pVeilTex->Width()
		, m_pVeilTex->Height()
		, m_pVeilTex->GetDC()
		, 0, 0
		, m_pVeilTex->Width()
		, m_pVeilTex->Height()
		, bf
	);

	if (effect.fDuration < effect.fCurTime)
	{
		m_listEffect.pop_front();
		return;
	}
}

void CCamera::CalDiff()
{
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	Vec2 vCenter = vResolution / 2.f;

	if (m_pTargetObj != nullptr)
	{
		// 타겟의 위치
		Vec2 vPos = m_pTargetObj->GetPos();
		vPos = GetRenderPos(vPos);

		float fLength = (vCenter - vPos).Length();

		if (fLength < m_fTraceRange)
		{
			m_fSpeed = 300.f;
			return;
		}
		else
		{
			Vec2 vLookDir = m_vLookAt - m_vPrevLookAt;
			if (!vLookDir.IsZero())
			{
				m_vCurLookAt = m_vPrevLookAt + vLookDir.Normalize() * m_fSpeed * fDT;

				// 맵의 내부에서는 카메라는 가속운동을 한다.
				m_fSpeed *= (1.006f);
			}
		}
	}

	// Map의 사이즈가 있다면, 카메라는 사이즈 밖을 보여줘서는 안된다.
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	Vec2 vMapSize = pCurScene->GetMapSize();

	// 맵의 사이즈가 정해져있다면.
	if (vMapSize != Vec2(0.f, 0.f))
	{
		vMapSize -= vResolution;
		vMapSize = vMapSize / 2.f;

		// 맵사이즈 내만 카메라가 보여준다.
		m_vCurLookAt.ClampX(-vMapSize.x, vMapSize.x);
		m_vCurLookAt.ClampY(-vMapSize.y, vMapSize.y);
		m_vLookAt.ClampX(-vMapSize.x, vMapSize.x);
		m_vLookAt.ClampY(-vMapSize.y, vMapSize.y);
		m_vPrevLookAt.ClampX(-vMapSize.x, vMapSize.x);
		m_vPrevLookAt.ClampY(-vMapSize.y, vMapSize.y);


		// 만약 카메라가 상하좌우의 끝에 붙어있다면.
		// 카메라의 이동속도를 500.f로 고정한다.
		if (m_vCurLookAt.x == -vMapSize.x
			|| m_vCurLookAt.x == vMapSize.x
			|| m_vCurLookAt.y == -vMapSize.y
			|| m_vCurLookAt.y == vMapSize.y)
		{
			m_fSpeed = 500.f;
		}
	}


	// 이전 LookAt과 현재 Look의 차이값을 보정해서 현재의 LookAt을 구한다.
	m_fAccTime += fDT;

	m_vDiff = m_vCurLookAt - vCenter;

	m_vPrevLookAt = m_vCurLookAt;
}
