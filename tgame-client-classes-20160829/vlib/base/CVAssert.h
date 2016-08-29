/**
* ¶ÏÑÔ
* @author thinkry
*/

#ifndef __CVLIB_ASSERT_H__
#define __CVLIB_ASSERT_H__

//#include "qqgame/BacktraceInfo.hpp"
#include "CVLog.h"

/**
* ¶ÏÑÔºê
*/
#ifdef _DEBUG
    #define CV_ASSERT(exp) \
        if(!(exp)) { \
	        CV_FATAL(("assert failed at %s line %d: %s", __FILE__, __LINE__, #exp));\
            QQGameServerLib::CBacktraceInfo stBackTrace;\
            stBackTrace.GenBacktraceInfo();\
            CV_FATAL((stBackTrace.GetAllBackTraceInfo()));\
	        assert(exp); \
        } else {}
#else
    #define CV_ASSERT(exp)      
#endif

#endif //__CVLIB_ASSERT_H__
