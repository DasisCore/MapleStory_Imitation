#include "global.h"
#include "CEffect.h"

#include "CPathMgr.h"
#include "CTimeMgr.h"
#include "CCamera.h"


CEffect::CEffect(CObject* _pObj, int _iEndFrame, float _fDuration)
	: m_iCurFrame(0)
	, m_iEndFrame(_iEndFrame)
	, m_fFrameDuration(_fDuration)
	, m_fCurTime(_fDuration)
{
	m_pOwner = _pObj;
	m_strFilePath = CPathMgr::GetInst()->GetContentPath();
	m_strFilePath += L"Texture\\Effect\\";

	wstring strCurFramePath = m_strFilePath + std::to_wstring(m_iCurFrame) + L".png";
	m_pImage = Image::FromFile(strCurFramePath.c_str());
}

CEffect::~CEffect()
{
	if (m_pImage != nullptr) delete m_pImage;
}

void CEffect::update()
{
	if (m_iCurFrame == m_iEndFrame + 1) DeleteObject(this);

	else if (m_fCurTime < 0)
	{
		m_fCurTime = m_fFrameDuration;
		m_iCurFrame++;
		delete m_pImage;
		wstring strCurFramePath = m_strFilePath + std::to_wstring(m_iCurFrame) + L".png";
		m_pImage = Image::FromFile(strCurFramePath.c_str());
	}
	else
	{
		m_fCurTime -= fDT;
	}
}

void CEffect::render(HDC _dc)
{
	Graphics graphics(_dc);

	float fWidth = m_pImage->GetWidth();
	float fHeight = m_pImage->GetHeight();

	Vec2 vPos = m_pOwner->GetPos();
	vPos = CCamera::GetInst()->GetRenderPos(vPos);
	Vec2 vScale = m_pOwner->GetScale();

	graphics.DrawImage(m_pImage, vPos.x - vScale.x, vPos.y - vScale.y, fWidth, fHeight);
}


