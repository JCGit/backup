#ifndef SceneItem_h__
#define SceneItem_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "role/QStageEntity.h"
#include "CfgMaster/SceneItemCfgMaster.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;

enum ITEM_STATE{ //物件现在的状态
	IDLE_STATE = 1,			//一般状态
	HURT_STATE = 2,			//受伤状态
	INJURED_STATE = 3,		//重伤状态
	DIE_STATE				//死亡状态
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
	static const string SCENE_ITEM_DISAPPEAR;		//item消失
	static const string SCENE_ITEM_FALLING_EVENT;	//场景物件掉落事件
public:
	CSceneItem();
	~CSceneItem();

	CREATE_FUNC(CSceneItem);

	virtual void onExit();
	virtual void onEnter();

	void loadRes();		//加载资源

	//物件自身的事件
	void idle();		//空闲
	void hurtWithDemage(float fDamage);	//收到攻击
	void injured();		//当血量在一定值一下时，为重伤状态
	bool checkIsDie();		//检测血量是否为0，做相应的操作
	void die();				//血量为0的操作

	void update(float dt);	//更新
protected:
	void handleItemMoveCallEvent(CCArmature * armature, MovementEventType type, const char * name);	//物件受伤动画播放完成动画
private:
	void initDataFromCfg();	//从配置中读取数据

	void showHurt();

	void buffForHero(CQStageEntity* pstEntity);		//根据buff类型对猪脚产生效果

	CC_PROPERTY(int, _cfgId, CfgID);				//配置id
	CC_SYNTHESIZE(CItemObj*, _itemObj, ItemObj);	//物件的数据
	CC_SYNTHESIZE(int, _maxHp, MaxHP);			//物件血量，只有阻挡可破坏的才有
	CC_SYNTHESIZE(int, _hp, HP);			//物件血量，只有阻挡可破坏的才有
	CC_SYNTHESIZE(int, _itemType, ItemType);		//物件类型
	CC_SYNTHESIZE(string, _resPath, ResPath);		//资源路径
	CC_SYNTHESIZE(string, _stExistVoice, ExistVoice);	//出现在场景中的音效
	CC_SYNTHESIZE(string, _stHurtVoice, HurtVoice);	 //受击音效
	CC_SYNTHESIZE(string, _stDieVoice, DieVoice);	//破坏、消失音效

	CC_SYNTHESIZE(int, _itemState, ItemState);		//物件现在的状态

	CC_PROPERTY(CCArmature*, _mArmature, Armature);	//动画

	CC_SYNTHESIZE(SceneItemCfg*, pstCfg, Cfg);		//配置

	CC_SYNTHESIZE(CCSize, _blockSize, BlockSize);	//阻挡大小

	float m_fStartTime;
	float m_fBaseScle;
};

#endif // SceneItem_h__
