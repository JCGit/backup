#ifndef FightObj_h__
#define FightObj_h__

#include "CSWarMapMsg.pb.h"
#include <vector>
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

enum FIGHT_TYPE{
	GUIDE_FIGHT_TYPE,		//ָ��ս���ؿ�
	GUIDE_NORMAL_FIGHT_TYPE,	//ָ��������ս���ؿ�
	NORMAL_FIGHT_TYPE,		//��ͨ�ؿ�
	ENDLESS_FIGHT_TYPE,		//�޾�ģʽ
	NIHILITY_FIGHT_TYPE,	//����֪��
	ELITE_FIGHT_TYPE,		//��Ӣ����
	DAILY_FIGHT_TYPE,		//�ճ�����
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

	void restoreRealWeapon();		//����ʱǹе���ݻ�ԭ����ʵǹе

	void swichToRealWeapon();		//����ʱǹе�л�����ʵǹе

	void resetCost();				//ÿ��ս����ʼ���ø������ҩ����
protected:
private:
	unsigned int m_uMapID;

	vmsg::CSWarMapEndRqst checkInfo;		//У������
	vmsg::CSWarMapEndRsp awardInfo;			//������Ϣ

	vmsg::CSWeapon m_stRealWeapon;			//��ʱ��ǹǰ��ǹе��Ϣ

	CC_SYNTHESIZE(unsigned int, _score, Score);
	CC_SYNTHESIZE(unsigned int, _totalTime, TotalTime);
	CC_SYNTHESIZE(unsigned int, _maxKill, MaxKill);
	CC_SYNTHESIZE(unsigned int, _scorePercent, ScorePercent);
	CC_SYNTHESIZE(unsigned int, _mark, Mark);					//�ؿ�����
	CC_SYNTHESIZE(unsigned int, _evolutionCnt, EvolutionCnt);	//������ʹ�ô���

	CC_SYNTHESIZE(unsigned int, _fightType, FightType);			//ս�����ͣ����ڽ��볡�����ж�

	CC_SYNTHESIZE(bool, _isForceQuit, IsForceQuit);

	CC_SYNTHESIZE(unsigned int, _ep, Ep);					//��¼��ǰep

	CC_SYNTHESIZE(unsigned int, m_uReliveCost, ReliveCost);		//��������
	CC_SYNTHESIZE(unsigned int, m_uFillBulletCost, FillBulletCost);	//����ҩ����

	CC_SYNTHESIZE(float, m_fCurBulletVal, CurBulletVal);		//��ǰǹе��ʣ��ĵ�ҩ
};

#define P_FIGHT_OBJ CVLib::CVSingleton<CFightObj>::Instance()

#endif // FightObj_h__
