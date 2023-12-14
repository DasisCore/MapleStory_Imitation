#pragma once

// 유니티 빌드를 사용하기 위해 pch를 사용하지 않고,
// global.h를 만들어 사용할 것.

#include <Windows.h>
#include <iostream>

// 리스트 뷰를 위한 헤더
#include <CommCtrl.h>
// + 추가 종속성에 comctl32.lib


#include <math.h>
#include <assert.h>

#include "define.h"
#include "struct.h"

// GDI+ (이미지 처리)
#include <gdiplus.h>

using namespace Gdiplus;

#pragma comment (lib, "Gdiplus.lib")

// for TransparentBlt
#pragma comment(lib, "Msimg32.lib")

#include <list>
using std::list;


#include <vector>
using std::vector;

#include <string>
using std::string;
using std::wstring;

#include <map>
using std::map;
using std::make_pair;

#include "func.h"
