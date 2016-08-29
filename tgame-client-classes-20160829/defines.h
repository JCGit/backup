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
	kActionStateAttackAndWalk,  //�߹������ƶ�
	kActionStateHurt,
	kActionStateKnockedOut,
	kActionStateAtkCounterMarch,
	kActionStateWander, //�Ĵ��ε�
	kActionStateWalkToDst, //�ε���Ŀ���
	kActionStateDie,
	kActionStateDying,
	kActionStateDash, //���
	kActionStateJump, //��Ծ
	kActionStateDizzy, //ѣ�Σ�����idle���������ǲ����ƶ������ܹ���
	kActionStateBackDash, //�������
	kActionStateImmobilize, //�����ƶ������ǿ��Թ���
	kActionStateBlind,		//�����ƶ������ǲ��ܹ���	
	kActionStateKnockDown,  //���򵹣�����ɣ�����û��
	kActionStateKick,		//�������
	kActionStateWin,		//ʤ������
	kActionStateForwardDash,	//��ǰ��
	kActionStateEmerge,		//����������
	kActionStateJumpAtk,	//��Ծ����
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

#define SOCKET_RECV_INTERVAL 10 //ִ�м���mainloop��ȥrecv socket

#endif