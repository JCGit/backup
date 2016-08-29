#ifndef FightObj_h__
#define FightObj_h__

#include "CSWarMapMsg.pb.h"
#include <vector>
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

enum FIGHT_TYPE{
	GUIDE_FIGHT_TYPE,		//指引战斗关卡
	GUIDE_NORMAL_FIGHT_TYPE,	//指引正常的战斗关卡
	NORMAL_FIGHT_TYPE,		//普通关卡
	ENDLESS_FIGHT_TYPE,		//无尽模式
	NIHILITY_FIGHT_TYPE,	//虚无知地
	ELITE_FIGHT_TYPE,		//精英副本
	DAILY_FIGHT_TYPE,		//日常副本
};

class CFightObj
{
public:
	CFightObj();
	~CFightObj();

	void setFightMapID(unsigned int mapID);
	unsigned int getFightMapID(){
		return m_uMapID;
	}

	vmsg::CSWarMapEndRqst& getCheckInfo();

	void setWarHideCondition(const vector<unsigned int>& stVec);

	void resetCheckInfo();

	void setAwardInfo(const vmsg::CSWarMapEndRsp& info);
	vmsg::CSWarMapEndRsp& getAwardInfo();

	void setEliteEndInfo(const vmsg::CSEndEliteWarMapRsp& info);

	void setDailyEndInfo(const vmsg::CSDailyInstancePassRsp& info);

	void setRealWeapon(const vmsg::CSWeapon& stInfo);
	vmsg::CSWeapon& getRealWeapon(){
		return m_stRealWeapon;
	}
	void resetRealWeapon();

	void restoreRealWeapon();		//将临时枪械数据还原成真实枪械

	void swichToRealWeapon();		//将临时枪械切换成真实枪械

	void resetCost();				//每次战斗开始重置复活，购买弹药消耗
protected:
private:
	unsigned int m_uMapID;

	vmsg::CSWarMapEndRqst checkInfo;		//校验数据
	vmsg::CSWarMapEndRsp awardInfo;			//奖励信息

	vmsg::CSWeapon m_stRealWeapon;			//临时换枪前的枪械信息

	CC_SYNTHESIZE(unsigned int, _score, Score);
	CC_SYNTHESIZE(unsigned int, _totalTime, TotalTime);
	CC_SYNTHESIZE(unsigned int, _maxKill, MaxKill);
	CC_SYNTHESIZE(unsigned int, _scorePercent, ScorePercent);
	CC_SYNTHESIZE(unsigned int, _mark, Mark);					//关卡评分
	CC_SYNTHESIZE(unsigned int, _evolutionCnt, EvolutionCnt);	//进化技使用次数

	CC_SYNTHESIZE(unsigned int, _fightType, FightType);			//战斗类型，用于进入场景的判断

	CC_SYNTHESIZE(bool, _isForceQuit, IsForceQuit);

	CC_SYNTHESIZE(unsigned int, _ep, Ep);					//记录当前ep

	CC_SYNTHESIZE(unsigned int, m_uReliveCost, ReliveCost);		//复活消耗
	CC_SYNTHESIZE(unsigned int, m_uFillBulletCost, FillBulletCost);	//购买弹药消耗

	CC_SYNTHESIZE(float, m_fCurBulletVal, CurBulletVal);		//当前枪械还剩余的弹药
};

#define P_FIGHT_OBJ CVLib::CVSingleton<CFightObj>::Instance()

#endif // FightObj_h__
