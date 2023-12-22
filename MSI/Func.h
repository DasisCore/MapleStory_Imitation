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

