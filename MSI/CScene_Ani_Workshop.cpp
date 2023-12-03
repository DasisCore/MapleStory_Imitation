#include "global.h"
#include "CScene_Ani_Workshop.h"

#include "CUI.h"
#include "CPanelUI.h"
#include "CBtnUI.h"

#include "CCore.h"
#include "CTexture.h"
#include "CPathMgr.h"
#include "CResMgr.h"
#include "CKeyMgr.h"

#include "CSprite.h"

CScene_Ani_Workshop::CScene_Ani_Workshop()
	: m_iScreenWidth(0)
	, m_iScreenHeight(0)
	, m_pMainSprite(nullptr)
{
}

CScene_Ani_Workshop::~CScene_Ani_Workshop()
{
}

void CScene_Ani_Workshop::Enter()
{
	CUI* pPanelUI = new CPanelUI;
	pPanelUI->SetName(L"Panel UI");
	pPanelUI->SetScale(Vec2(300.f, 100.f));
	pPanelUI->SetPos(Vec2(300.f, 300.f));

	CBtnUI* pBtnUI = new CBtnUI;
	pBtnUI->SetName(L"Btn UI");
	pBtnUI->SetScale(Vec2(50.f, 25.f));
	pBtnUI->SetPos(Vec2(0.f, 0.f));
	pBtnUI->SetClickCallBack(this, (SCENE_MEMFUNC)&CScene_Ani_Workshop::LoadTexture);

	pPanelUI->AddChild(pBtnUI);
	AddObject(pPanelUI, GROUP_TYPE::UI);
	

	// ============================================================
	// ������ â�� �ػ� ������ ���� �۾�
	// ============================================================
	// 
	// ������� �ػ� ���
	m_iScreenWidth = GetSystemMetrics(SM_CXSCREEN) * 0.7f;
	m_iScreenHeight = GetSystemMetrics(SM_CYSCREEN) * 0.7f;

	// workshop ����۸� �����ϰ� ���� ����ۿ� ��ü�Ѵ�.
	CTexture* pTex = CResMgr::GetInst()->CreateTexture(L"Workshop", (UINT)m_iScreenWidth, (UINT)m_iScreenHeight);
	CCore::GetInst()->SetMemTex(pTex);
	
	CCore::GetInst()->ChangeWindowSize(Vec2(m_iScreenWidth, m_iScreenHeight), false);

	// ȭ�� �� �߾��� �� ��� (�� �����, �����츦 ������� �߾ӿ� ��ġ��Ű�� ����)
	int centerX = GetSystemMetrics(SM_CXSCREEN) / 2;
	int centerY = GetSystemMetrics(SM_CYSCREEN) / 2;

	RECT rt = { 0, 0, (long)m_iScreenWidth, (long)m_iScreenHeight };
	SetWindowPos(CCore::GetInst()->GetMainHwnd(), nullptr, centerX - (m_iScreenWidth / 2.f), centerY - (m_iScreenHeight / 2.f), rt.right - rt.left, rt.bottom - rt.top, SWP_NOSIZE);
}

void CScene_Ani_Workshop::Exit()
{
	CTexture* pMemTex = CResMgr::GetInst()->FindTexture(L"BackBuffer");
	assert(pMemTex);
	CCore::GetInst()->SetMemTex(pMemTex);
}

void CScene_Ani_Workshop::LoadTexture()
{
	OPENFILENAME ofn = {};

	wchar_t szName[256] = {};

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();
	ofn.lpstrFile = szName;
	ofn.nMaxFile = sizeof(szName);
	//ofn.lpstrFilter = L"ALL\0*.*\0png\0*.png\0";
	ofn.lpstrFilter = L"ALL\0*.*\0png\0*.png\0bmp\0*.bmp\0jpeg\0*.jpeg\0gif\0*.gif\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	// ������ Path�� ������
	wstring strTextureFolder = CPathMgr::GetInst()->GetContentPath();
	strTextureFolder += L"Texture";

	ofn.lpstrInitialDir = strTextureFolder.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// ���� �бⰡ �����ߴٸ�.
	if (GetOpenFileName(&ofn))
	{
		wstring temp(szName);
		if (!CheckImageFormat(temp))
		{
			MessageBox(nullptr, L"bmp, png, jpeg, gif Ȯ���ڸ� �����մϴ�.", L"Error Message", MB_OK);
			return;
		}

		wstring strAbsolutePath(szName);
		
		// �۾��� �ѹ��� �ϳ��� ��������Ʈ�� �����ϴ�.
		if (m_pMainSprite != nullptr)
		{
			DeleteObject(m_pMainSprite);
		}

		CSprite* pSprite = new CSprite(strAbsolutePath);
		pSprite->SetPos(Vec2(100.f, 100.f));
		AddObject(pSprite, GROUP_TYPE::SPRITE);

		m_pMainSprite = pSprite;
	}
}

bool CScene_Ani_Workshop::CheckImageFormat(wstring _wStr)
{
	wstring e = L"";

	bool flag = 0;
	int iLen = _wStr.length();
	for (int i = 0; i < iLen; i++)
	{
		if (_wStr[i] == L'.') flag = 1;
		if (flag == 1) e += _wStr[i];
	}

	if (e == L".png" || e == L".bmp" || e == L".gif" || e == L".jpeg") return true;
	return false;
}

void CScene_Ani_Workshop::update()
{
	CScene::update();

	if (KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::TOOL);
	}
}

void CScene_Ani_Workshop::render(HDC _dc)
{
	CScene::render(_dc);
}


