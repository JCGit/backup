#ifndef __ZGAME_BASIC_TYPES_H__
#define __ZGAME_BASIC_TYPES_H__

#include "vlib/base/CVLog.h"

using namespace CVLib;

const int INVALID_INDEX = -1;

//namespace __gnu_cxx
//{
//	template <int N>
//	struct hash< CVLib::CVSizeString<N> >
//	{
//		size_t operator()(const CVLib::CVSizeString<N>& s) const
//		{
//			return __stl_hash_string(s.Get());
//		}
//	};
//}

typedef unsigned int TimeInt_t;

//-------所有概率参数的分母为10000
enum enumCommonConsts
{
    EGAME_DENOM    =   10000,    ///< 所有概率参数的默认分母
    
    EUINT_MAX       =   0xFFFFFFFF, ///< unsigned int max val
    EINT_MAX        =   0x7FFFFFFF,
    ESHORT_MAX      =   0x7FFF,
    ESHORT_MIN      =   -32768,    
    EUSHORT_MAX     =   0xFFFF,
    EUCHAR_MAX = 0xFF,
};

typedef unsigned long long UINT64_t; //unsigned int 64 for all uint id, %lld
typedef UINT64_t UNIT64_t; // 要改之处略多，先兼容这个拼写错误

typedef long long int64;
typedef unsigned long long uint64;


//配置id
typedef unsigned int ID_CFG_ID_t;
//场景ID
typedef unsigned int ID_SCENE_ID_t;

//最大路径长度
const int MAX_PATHNAME_LEN = 256;

typedef unsigned long  UIN_t; //unsigned int for uin

#endif


