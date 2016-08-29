#ifndef __DEFINES_H___
#define __DEFINES_H___
#pragma once
#include "cocos2d.h"

// 1 - convenience measurements
#define SCREEN CCDirector::sharedDirector()->getWinSize()
#define CENTER ccp(SCREEN.width / 2, SCREEN.height / 2)
#define CURTIME do {                                                        \
	timeval time;                                                           \
	gettimeofday(&time, NULL);                                              \
	unsigned long millisecs = (time.tv_sec * 1000) + (time.tv_usec / 1000); \
	return (float)millisecs;                                                \
} while (0)

// 2 - convenience functions
#define random_range(low, high) (rand() % (high - low + 1)) + low
#define frandom (float)rand() / UINT64_C(0x100000000)
#define frandom_range(low, high) ((high - low) * frandom) + low

#ifdef WIN32
#define IS_AUDIO_OPEN true
#else
#define IS_AUDIO_OPEN true
#define to_string intToString
/*
namespace std {
    #define to_string ::intToString
}
*/
#endif

inline std::string intToString(const int n)
{
	char att[100];
	snprintf(att, 100, "%d" ,n);  
	return std::string(att);
}


inline std::string intToString(const unsigned long long n)
{
    char att[100];
    snprintf(att, 100, "%lld" ,n); 
    return std::string(att);
}

inline std::string intToString(const unsigned int n)
{
    char att[100];
    snprintf(att, 100, "%u" ,n);  
    return std::string(att);
}


// 3 - enumerations
typedef enum _ActionState {
	kActionStateNone = 0,
	kActionStateIdle,   //1
	kActionStateAttack, //2
	kActionStateWalk,   //3
	kActionStateAttackAndWalk,  //边攻击边移动
	kActionStateHurt,
	kActionStateKnockedOut,
	kActionStateAtkCounterMarch,
	kActionStateWander, //四处游荡
	kActionStateWalkToDst, //游荡至目标点
	kActionStateDie,
	kActionStateDying,
	kActionStateDash, //冲击
	kActionStateJump, //跳跃
	kActionStateDizzy, //眩晕，播放idle动作，但是不能移动，不能攻击
	kActionStateBackDash, //向后闪冲
	kActionStateImmobilize, //不能移动，但是可以攻击
	kActionStateBlind,		//可以移动，但是不能攻击	
	kActionStateKnockDown,  //被打倒，被打飞，但是没死
	kActionStateKick,		//伸脚踢人
	kActionStateWin,		//胜利动作
	kActionStateForwardDash,	//向前冲
	kActionStateEmerge,		//出场动作中
	kActionStateJumpAtk,	//跳跃攻击
} ActionState;

// 4 - structures
typedef struct _BoundingBox {
	cocos2d::CCRect actual;
	cocos2d::CCRect original;
} BoundingBox;

enum FlyTraitType{
	E_FLY_HORIZENTAL = 0,
	E_FLY_VIRTICAL = 1,
	E_FLY_FOLLOW = 2,
	E_FLY_HORIZENTAL_FOLLOW = 3,
	E_FLY_VIRTICAL_FOLLOW = 4,
};

enum{
	E_FLY_STATE_PREPARE = 0,
	E_FLY_STATE_TARGET_CHASE = 1,
};

#define OPTIMAL_HEIGHT 700.0

#define SOCKET_RECV_INTERVAL 10 //执行几个mainloop后去recv socket

#endif