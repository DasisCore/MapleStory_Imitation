#include "global.h"
#include "CEffect.h"

#include "CPathMgr.h"
#include "CTimeMgr.h"
#include "CCamera.h"

CEffect::CEffect(CObject* _pObj, wstring _strPath, int _iEndFrame, float _fDuration, int _iDir, Vec2 _vOffset)
	: m_iCurFrame(0)
	, m_iEndFrame(_iEndFrame)
	, m_fFrameDuration(_fDuration)
	, m_fCurTime(_fDuration)
	, m_iDir(_iDir)
	, m_vOffset(_vOffset)
{
	m_pOwner = _pObj;
	m_strFilePath = CPathMgr::GetInst()->GetContentPath();
	m_strFilePath += (L"Texture\\Effect\\" + _strPath + L"\\");

	for (int i = 0; i < _iEndFrame; i++)
	{
		wstring strCurFramePath = m_strFilePath + std::to_wstring(i) + L".png";
		Image* pImage = Image::FromFile(strCurFramePath.c_str());
		m_vecImage.push_back(pImage);
	}

	m_fWidth = m_vecImage[0]->GetWidth();
	m_fHeight = m_vecImage[0]->GetHeight();
}

CEffect::~CEffect()
{
	if (m_vecImage.size() > 0)
	{
		for (int i = 0; i < m_vecImage.size(); i++)
		{
			if (m_vecImage[i] != nullptr) delete m_vecImage[i];
		}
	}
}

void CEffect::update()
{
	if (m_iCurFrame >= m_iEndFrame - 1)
	{
		DeleteObject(this);
		return;
	}

	if (m_fCurTime < 0)
	{
		m_fCurTime = m_fFrameDuration;
		m_iCurFrame++;
	}
	else
	{
		m_fCurTime -= fDT;
	}
}

void CEffect::render(HDC _dc)
{ 
	if (m_pGraphics == nullptr) m_pGraphics = new Graphics(_dc);

	Vec2 vPos = m_pOwner->GetPos();
	vPos = CCamera::GetInst()->GetRenderPos(vPos);
	Vec2 vScale = m_pOwner->GetScale();

	float x = vPos.x - vScale.x;
	float y = vPos.y - vScale.y;
	if (m_iDir == -1) x += (m_fWidth / 2);

	m_pGraphics->DrawImage(m_vecImage[m_iCurFrame], x + m_vOffset.x, y + m_vOffset.y, m_fWidth * m_iDir, m_fHeight);
}


