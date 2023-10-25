#include "global.h"
#include "CTimeMgr.h"


CTimeMgr::CTimeMgr()
	:m_llCurCount{0}
	,m_llPrevCount{0}
	,m_llFrequency{0}
{

}

CTimeMgr::~CTimeMgr()
{

}

void CTimeMgr::init()
{
	QueryPerformanceCounter(&m_llPrevCount);
	QueryPerformanceFrequency(&m_llFrequency);
}

void CTimeMgr::update()
{
	QueryPerformanceCounter(&m_llCurCount);

	m_dDT = (double)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (double)m_llFrequency.QuadPart;

	m_llPrevCount = m_llCurCount;

	// ���� DT�� 60�� �ѱ�ٸ� ������ 60���� ������Ų��.
#ifdef _DEBUG
	if (m_dDT > (1. / 60.)) m_dDT = (1. / 60.);
#endif
}
