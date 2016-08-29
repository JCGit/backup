#ifndef SceneItem_h__
#define SceneItem_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "role/QStageEntity.h"
#include "CfgMaster/SceneItemCfgMaster.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;

enum ITEM_STATE{ //������ڵ�״̬
	IDLE_STATE = 1,			//һ��״̬
	HURT_STATE = 2,			//����״̬
	INJURED_STATE = 3,		//����״̬
	DIE_STATE				//����״̬
};

class CItemObj : public CCObject
{
public:
	CREATE_FUNC(CItemObj);
	int x;
	int y;
	int id;
	int stage;
	int touchId;
	int dropId;
	int dropType;
	int dropParty;
	bool init(){return true;}
public:
	CItemObj(){
		x = 0;
		y = 0;
		id = 0;
		stage = 0;
		touchId = 0;
		dropId = 0;
		dropType = 0;
		dropParty = 0;
	}
};

class CSceneItem : public CCNode, public CQEventDispatcher
{
public:
	static const string SCENE_ITEM_DISAPPEAR;		//item��ʧ
	static const string SCENE_ITEM_FALLING_EVENT;	//������������¼�
public:
	CSceneItem();
	~CSceneItem();

	CREATE_FUNC(CSceneItem);

	virtual void onExit();
	virtual void onEnter();

	void loadRes();		//������Դ

	//���������¼�
	void idle();		//����
	void hurtWithDemage(float fDamage);	//�յ�����
	void injured();		//��Ѫ����һ��ֵһ��ʱ��Ϊ����״̬
	bool checkIsDie();		//���Ѫ���Ƿ�Ϊ0������Ӧ�Ĳ���
	void die();				//Ѫ��Ϊ0�Ĳ���

	void update(float dt);	//����
protected:
	void handleItemMoveCallEvent(CCArmature * armature, MovementEventType type, const char * name);	//������˶���������ɶ���
private:
	void initDataFromCfg();	//�������ж�ȡ����

	void showHurt();

	void buffForHero(CQStageEntity* pstEntity);		//����buff���Ͷ���Ų���Ч��

	CC_PROPERTY(int, _cfgId, CfgID);				//����id
	CC_SYNTHESIZE(CItemObj*, _itemObj, ItemObj);	//���������
	CC_SYNTHESIZE(int, _maxHp, MaxHP);			//���Ѫ����ֻ���赲���ƻ��Ĳ���
	CC_SYNTHESIZE(int, _hp, HP);			//���Ѫ����ֻ���赲���ƻ��Ĳ���
	CC_SYNTHESIZE(int, _itemType, ItemType);		//�������
	CC_SYNTHESIZE(string, _resPath, ResPath);		//��Դ·��
	CC_SYNTHESIZE(string, _stExistVoice, ExistVoice);	//�����ڳ����е���Ч
	CC_SYNTHESIZE(string, _stHurtVoice, HurtVoice);	 //�ܻ���Ч
	CC_SYNTHESIZE(string, _stDieVoice, DieVoice);	//�ƻ�����ʧ��Ч

	CC_SYNTHESIZE(int, _itemState, ItemState);		//������ڵ�״̬

	CC_PROPERTY(CCArmature*, _mArmature, Armature);	//����

	CC_SYNTHESIZE(SceneItemCfg*, pstCfg, Cfg);		//����

	CC_SYNTHESIZE(CCSize, _blockSize, BlockSize);	//�赲��С

	float m_fStartTime;
	float m_fBaseScle;
};

#endif // SceneItem_h__
