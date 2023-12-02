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

CScene_Ani_Workshop::CScene_Ani_Workshop()
	: m_MainHDC(nullptr)
	, m_pImage(nullptr)
	, m_iScreenWidth(0)
	, m_iScreenHeight(0)
{
}

CScene_Ani_Workshop::~CScene_Ani_Workshop()
{
	if (m_pImage) delete m_pImage;

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
	
	// 메인 DC 저장.
	m_MainHDC = CCore::GetInst()->GetMainDC();


	// 모니터의 해상도 얻기
	m_iScreenWidth = GetSystemMetrics(SM_CXSCREEN) * 0.7f;
	m_iScreenHeight = GetSystemMetrics(SM_CYSCREEN) * 0.7f;

	HBITMAP hBit = CreateCompatibleBitmap(m_MainHDC, m_iScreenWidth, m_iScreenHeight);
	HBITMAP hOldBit = (HBITMAP)SelectObject(m_MainHDC, hBit);
	DeleteObject(hOldBit);

	// workshop 백버퍼를 생성하고 기존 백버퍼와 교체한다.
	CTexture* pTex = CResMgr::GetInst()->CreateTexture(L"Workshop", (UINT)m_iScreenWidth, (UINT)m_iScreenHeight);
	CCore::GetInst()->SetMemTex(pTex);
	
	CCore::GetInst()->ChangeWindowSize(Vec2(m_iScreenWidth, m_iScreenHeight), false);

	// 화면 정 중앙의 값 계산
	int centerX = (m_iScreenWidth) / 2;
	int centerY = (m_iScreenHeight) / 2;

	RECT rt = { 0, 0, (long)m_iScreenWidth, (long)m_iScreenHeight };
	SetWindowPos(CCore::GetInst()->GetMainHwnd(), nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, SWP_NOSIZE);
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
	ofn.lpstrFilter = L"ALL\0*.*\0png\0*.png\0bmp\0*.bmp\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	// 컨텐츠 Path를 가져옴
	wstring strTextureFolder = CPathMgr::GetInst()->GetContentPath();
	strTextureFolder += L"Texture";

	ofn.lpstrInitialDir = strTextureFolder.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// 파일 읽기가 성공했다면.
	if (GetOpenFileName(&ofn))
	{
		wstring strRelativePath = CPathMgr::GetInst()->GetRelativePath(szName);
		m_pImage = Image::FromFile(szName);
	};

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

	Graphics graphics(_dc);
	if (m_pImage)
	{
		graphics.DrawImage(m_pImage, 0, 0);
	}
}


