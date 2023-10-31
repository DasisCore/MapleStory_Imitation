#pragma once

// 유니티 빌드를 사용하기 위해 pch를 사용하지 않고,
// global.h를 만들어 사용할 것.

#include <Windows.h>
#include <iostream>

#include <math.h>
#include <assert.h>

#include "define.h"
#include "struct.h"


// for TransparentBlt
#pragma comment(lib, "Msimg32.lib")


#include <vector>
using std::vector;

#include <string>
using std::string;
using std::wstring;

#include <map>
using std::map;
using std::make_pair;

#include "func.h"
