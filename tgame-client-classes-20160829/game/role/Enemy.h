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
	int type;	//ˢ������
	int time;	//������ʱ��
	int stage;	//���ڵڼ��׶�
	int num;	//ˢ������
	int inter;	//���ʱ��
	int dropCnt;	//��������
	int touchID;	//����ID��0Ϊ��ʼ�ߴ���
	int monsterType;	//��������
	int key;		//Ψһ��ʶ
	float brushTime;	//ˢ��ʱ��
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

	virtual void SleepAct();			//����״̬

	virtual void Active();			//�����

	void MovementCallback(CCArmature * armature, MovementEventType type, const char * name);
protected:
	
private:
	void bubbleEventHandler(CCObject *pSender, BubbleEventType type);

	CC_SYNTHESIZE(string, m_stHeadRes, HeadRes);
	CC_SYNTHESIZE(short, _type, EnemyType);	//��������
	CC_PROPERTY(CEnemyData*, _monsterObj, MonsterObj);	//���������

	CC_SYNTHESIZE(bool, _isBubbleOn, IsBubbleOn);		//�Ƿ�����ݶԻ�����

	unsigned int m_uBubbleTime;			//���ݶԻ�����ʱ��
	unsigned int m_uBubbleID;			//����ID

	int m_iBubbleCnt;
	int m_iCheckInterval;

	CC_SYNTHESIZE(string, m_stHurtAnimation, HurtAnimation);  //���������hurt
};

#endif // Enemy_h__
