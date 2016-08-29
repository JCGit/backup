#ifndef Enemy_h__
#define Enemy_h__
#include "cocos2d.h"
#include "QStageEntity.h"
#include "game/instance/InstanceEventUtils.h"
#include "role/BubbleDialog.h"

USING_NS_CC;

class CEnemyData : public CCObject{
public:
	CREATE_FUNC(CEnemyData);
	int x;
	int y;
	int id;
	int type;	//刷怪类型
	int time;	//触发后时间
	int stage;	//处于第几阶段
	int num;	//刷怪数量
	int inter;	//间隔时间
	int dropCnt;	//掉落数量
	int touchID;	//触发ID，0为起始线触发
	int monsterType;	//怪物类型
	int key;		//唯一标识
	float brushTime;	//刷怪时间
	bool init(){return true;}
public:
	CEnemyData(){
		x = 0;
		y = 0;
		id = 0;
		type = 0;
		time = 0;
		stage = 0;
		dropCnt = 0;
		num = 0;
		inter = 0;
		touchID = 0;
		monsterType = 0;
		key = 0;
		brushTime = 0;
	}
};


class CEnemy : public CQStageEntity
{
public:
	CEnemy();
	virtual ~CEnemy();
	CREATE_FUNC(CEnemy);

	bool init();

	virtual void setScaleX(float fScaleX);
	virtual float getScaleX();
	virtual void setScale(float fScale);

	void update(float dt); 

	virtual void HurtWithDamage(float fDamage);
	virtual void HurtWithDamage(CHurtDamageObj stObj);

	virtual void ShowHurt(int fDamage);
	virtual void HPChange();
	virtual void HandleEntityDeath();
	virtual void OtherHurt();

	virtual void WalkToDstDoneCallback();

	void updateBubbleDialog();

	virtual void Emerge();

	virtual void SleepAct();			//休眠状态

	virtual void Active();			//激活动作

	void MovementCallback(CCArmature * armature, MovementEventType type, const char * name);
protected:
	
private:
	void bubbleEventHandler(CCObject *pSender, BubbleEventType type);

	CC_SYNTHESIZE(string, m_stHeadRes, HeadRes);
	CC_SYNTHESIZE(short, _type, EnemyType);	//敌人类型
	CC_PROPERTY(CEnemyData*, _monsterObj, MonsterObj);	//怪物的数据

	CC_SYNTHESIZE(bool, _isBubbleOn, IsBubbleOn);		//是否打开气泡对话开关

	unsigned int m_uBubbleTime;			//气泡对话出现时间
	unsigned int m_uBubbleID;			//气泡ID

	int m_iBubbleCnt;
	int m_iCheckInterval;

	CC_SYNTHESIZE(string, m_stHurtAnimation, HurtAnimation);  //怪物自身的hurt
};

#endif // Enemy_h__
