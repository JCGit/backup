/**********************************************************************
FileName:       CVRand.h
Author:         LincLiu
Date:           2010-09-20
Description:    生成随机数的类
Version:        1.0
Function List:  
History:         
<author>  <date>   <version>   <desc>
LincLiu  2010/09/20    1.0        create  
**********************************************************************/
#ifndef __CTRAND_H__
#define __CTRAND_H__
//#include <unistd.h>
#include <stdlib.h>
#include <cmath>
#include <time.h>

namespace CVLib
{
static bool m_bInited = false;
class CRandom
{
public:
    //roll取一个随机数 不包含max
    static unsigned int GetRand(unsigned int iMax)
    {
        if (iMax == 0)
        {
            return 0;
        }

        if (!m_bInited)
        {
            Init();
        }

        return (unsigned int)rand() % iMax;
    }
    //roll 取一个随机数包涵max
    static unsigned int GetRandHaveMax(unsigned int iMax)
    {
        return GetRand(iMax) + 1;
    }
private:
   

    static void Init()
    {
        //srand((unsigned int)time(NULL) + getpid());
        srand((unsigned int)time(NULL));
        m_bInited = true;
    }
};

}
#endif

