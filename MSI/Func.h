#pragma once

class CObject;
class CSprite;
class AI;

void CreateObject(CObject* _pObj, GROUP_TYPE _eGroup);
void DeleteObject(CObject* _pObj);
void ChangeScene(SCENE_TYPE _eNext);
void ChangeSprite(CSprite* _ePrev, CSprite* _eNext);
void ChangeAIState(AI* _pAI, MON_STATE _eNextState);


template<typename T1, typename T2>
void Safe_Delete_Map(map<T1, T2>& _map)
{
	typename map<T1, T2>::iterator iter = _map.begin();

	for (; iter != _map.end(); iter++)
	{
		if (nullptr != iter->second)
		{
			delete iter->second;
		}
	}
	_map.clear();
}

template<typename T>
void Safe_Delete_Vector(vector<T>& _vec)
{
	for (size_t i = 0; i < _vec.size(); i++)
	{
		if (_vec[i] != nullptr) delete _vec[i];
	}
	_vec.clear();
}

template<typename T>
void Safe_Delete_List(list<T>& _list)
{
	for (auto iter = _list.begin(); iter != _list.end(); iter++)
	{
		if (*iter != nullptr)
		{
			delete* iter;
			iter = _list.erase(iter); // 삭제 후에 반복자를 올바르게 증가시킴
		}
		else
		{
			++iter;
		}
	}
	_list.clear();
	_list.resize(0);
}

void SaveWString(const wstring& _str, FILE* _pFile);
void LoadWString(wstring& _str, FILE* _pFile);
wstring setprecision_float(float _f, int cnt);

// 특정 유형의 오브젝트를 저장
void SaveObject(const vector<CObject*> _vecObj, FILE* _pFile);

// 이 템플릿은 BackGround, Foothold, Ground등의 특수한 클래스에만 적용 가능.
template<typename T>
T* ReadObject(FILE* pFile)
{
    // 오브젝트 이름 읽기
    wstring strName;
    LoadWString(strName, pFile);

    // Position
    float fPosX = 0.f;
    float fPosY = 0.f;
    fread(&fPosX, sizeof(float), 1, pFile);
    fread(&fPosY, sizeof(float), 1, pFile);
    Vec2 vPos = Vec2(fPosX, fPosY);

    // Scale
    float fScaleX = 0.f;
    float fScaleY = 0.f;
    fread(&fScaleX, sizeof(float), 1, pFile);
    fread(&fScaleY, sizeof(float), 1, pFile);
    Vec2 vScale = Vec2(fScaleX, fScaleY);

    // Collider
    bool bCollider = false;
    fread(&bCollider, sizeof(bool), 1, pFile);
    Vec2 vColOffset = Vec2(0.f, 0.f);
    Vec2 vColScale = Vec2(0.f, 0.f);

    if (bCollider)
    {
        float fOffsetX = 0.f;
        float fOffsetY = 0.f;
        fread(&fOffsetX, sizeof(float), 1, pFile);
        fread(&fOffsetY, sizeof(float), 1, pFile);
        vColOffset = Vec2(fOffsetX, fOffsetY);

        float fColScaleX = 0.f;
        float fColScaleY = 0.f;
        fread(&fColScaleX, sizeof(float), 1, pFile);
        fread(&fColScaleY, sizeof(float), 1, pFile);
        vColScale = Vec2(fColScaleX, fColScaleY);
    }

    // Animator
    bool bAnimator = false;
    fread(&bAnimator, sizeof(bool), 1, pFile);

    vector<wstring> vecPath;
    if (bAnimator)
    {
        UINT animSize = 0;
        fread(&animSize, sizeof(UINT), 1, pFile);

        for (int i = 0; i < animSize; i++)
        {
            wstring animPath = L"";
            LoadWString(animPath, pFile);
            vecPath.push_back(animPath);
        }
    }

    // RigidBody
    bool bRigidBody = false;
    fread(&bRigidBody, sizeof(bool), 1, pFile);

    // Gravity
    bool bGravity = false;
    fread(&bGravity, sizeof(bool), 1, pFile);

    // Create and return the specific object
    return new T(strName, vPos, vScale, bCollider, vColOffset, vColScale, bAnimator, vecPath, bGravity, bRigidBody);
}
