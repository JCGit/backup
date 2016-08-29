#ifndef NPC_h__
#define NPC_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "role/BubbleDialog.h"
#include "vlib/QEventDispatcher.h"
using namespace CVLib;
USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;

class NPCRole :public CCNode, public CQEventDispatcher
{
public:
	NPCRole();
	virtual ~NPCRole();

	CREATE_FUNC(NPCRole);

	virtual bool init();

	void update(float delta);

	virtual void onExit();
	virtual void onEnter();

	void LoadActionRes();

	void updateBubbleDialog();

	void updateTaskState();
protected:
	void MovementCallback(CCArmature * armature, MovementEventType type, const char * name);
private:
	void bubbleEventHandler(CCObject *pSender, BubbleEventType type);

	CC_PROPERTY(unsigned int, _cfgID, CfgID);
	CC_SYNTHESIZE(string, _resPath, ResPath);
	CC_SYNTHESIZE(CCArmature*, mArmature, Armature);

	CCLabelTTF* m_pNameTxt;

	CCArmature* m_pTaskArmature;		//任务问号动画

	string m_stNPCName;

	unsigned int m_uBubbleTime;			//气泡对话出现时间
	unsigned int m_uBubbleID;			//气泡ID
	unsigned int m_uLoopCnt;            //普通动作的loop次数

    int m_iBubbleCnt;
    int m_iCheckInterval;
};

#endif // NPC_h__
