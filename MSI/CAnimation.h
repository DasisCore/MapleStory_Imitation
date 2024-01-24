#pragma once

class CAnimator;
class CTexture;

struct tAnimFrm
{
	Vec2 vLT;
	Vec2 vSlice;
	Vec2 vOffset;
	float fDuration;
};


class CAnimation
{
private:
	wstring m_strName;
	CAnimator* m_pAnimator;
	CTexture* m_pTex;			// Animation�� ����ϴ� �ؽ���
	vector<tAnimFrm> m_vecFrm;	// ��� ������ ����
	int m_iCurFrm;				// ���� ������ ����
	float m_fAccTime;

	bool m_bIsReverse;
	
	bool m_bFinish;				// ��� �� ���� ����

public:
	const wstring& GetName() { return m_strName; }
	bool IsFinish() { return m_bFinish; }
	void SetFrame(int _iFrameIdx) 
	{
		m_bFinish = false;
		m_iCurFrm = _iFrameIdx;
		m_fAccTime = 0.f;
	}

	tAnimFrm& GetFrame(int _iIdx) { return m_vecFrm[_iIdx]; }
	UINT GetMaxFrame() { return (UINT)m_vecFrm.size(); }

private:
	void SetName(const wstring& _strName) { m_strName = _strName; }

public:
	void update();
	void render(HDC _dc, float _fAlpha = 255);
	void Create(CTexture* _pTex, vector<tAnimFrm> _vFrame, float _fDuration);
	// �ǰ��� �ִϸ��̼�
	void Create_rewind(CTexture* _pTex, vector<tAnimFrm> _vFrame, float _fDuration);

public:
	void Save(const wstring& _strRelativePath);
	void Load(const wstring& _strRelativePath);

public:
	CAnimation();
	~CAnimation();


	friend class CAnimator;
};

