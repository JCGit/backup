#ifndef GuideControl_h__
#define GuideControl_h__

#include "vlib/base/CVSingleton.h"

#include "GuideContainer.h"
#include "GuideView.h"
#include "GuideCallFunc.h"

typedef enum GuideID { // 与配置匹配
	GUIDE_ID_NEWCOMER = 1,		// 新手进入
	GUIDE_ID_FIRST_TASK,		// 第一个任务
	GUIDE_ID_EQUIP_TAKEON,		// 装备穿戴
	GUIDE_ID_WEAPON_LVUP,		// 枪械升级
	GUIDE_ID_WEAPON_STEP,		// 枪械升阶
	GUIDE_ID_WEAPON_LVUP_AGAIN, // 再次枪械升级
	GUIDE_ID_LOTTERY_ADVANCED,	// 高级探秘
	GUIDE_ID_NOTHING_LAND,		// 虚无之地
	GUIDE_ID_EQUIP_LVUP,		// 装备升级
	GUIDE_ID_LOTTERY_BATCH,		// 十连抽
	GUIDE_ID_FRIEND,			// 添加好友
	GUIDE_ID_ELITE_BATTLE,		// 精英副本
	GUIDE_ID_ARENA,				// 竞技场
	GUIDE_ID_REFIT,				// 改装
	GUIDE_ID_WEAPON_ARRAY,		// 战备
	GUIDE_ID_GENE_STRENGTHEN,	// 基因强化
} GUIDE_ID;

typedef enum GuideUIType { // 与配置匹配
	GUIDE_UI_NONE = 0,			// 不需操作显示对象
	GUIDE_UI_CITY_SCREEN,		// 同屏主界面
	GUIDE_UI_SUB_ENTRY,			// 二级入口
	GUIDE_UI_ROLE,				// 角色
	GUIDE_UI_EQUIP_INFO,		// 装备信息
	GUIDE_UI_EQUIP_LIB,			// 装备养成
	GUIDE_UI_EQUIP_SEL,			// 装备选择
	GUIDE_UI_WEAPON_LIB,		// 枪械库
	GUIDE_UI_WEAPON_SELECT,		// 枪械选择
	GUIDE_UI_WEAPON_INFO,		// 枪械信息
	GUIDE_UI_WEAPON_MATERIAL,	// 枪械材料
	GUIDE_UI_WEAPON_STEP_DONE,	// 枪械进阶成功
	GUIDE_UI_LOTTERY,			// 夺宝奇兵
	GUIDE_UI_WAR_MAP,			// 关卡选择
	GUIDE_UI_WAR_ENTER,			// 进入副本
	GUIDE_UI_FRIEND,			// 好友
	GUIDE_UI_ARENA,				// 竞技场
	GUIDE_UI_ARENA_MATCH,		// 竞技场匹配
	GUIDE_UI_NOTHING_LAND,		// 虚无之地
	GUIDE_UI_UILAYER,			// UIBoxLayer
	GUIDE_UI_COMMON_DIALOG,		// 弹出对话框
	GUIDE_UI_EQUIP_LIST,		// 枪械列表
	GUIDE_UI_NOTHING_LAND_ENTER,// 虚无之地出击
	GUIDE_UI_ELITE_WAR_ENTER,	// 进入精英副本
	GUIDE_UI_REFIT,				// 改装
	GUIDE_UI_REFIT_MATERIAL,	// 改装材料
	GUIDE_UI_WEAPON_ARRAY,		// 战备
	GUIDE_UI_WEAPON_SEL,		// 枪械选择
	GUIDE_UI_GENE_STRENGTHEN,	// 基因强化
	GUIDE_UI_STRENGTHEN_EFFECT, // 强化效果
	GUIDE_UI_CHANGEWEAPON_INFO,//阵位上枪械信息
	GUIDE_UI_DAILYTASK,		//每日任务
	GUIDE_UI_SIGNIN,      //签到
	GUIDE_UI_EVOLUTION,   //技能进化
} GUIDE_UI_TYPE;

enum GUIDE_MSG_ID {
	GUIDE_MSG_SHOW, 
	GUIDE_MSG_HIDE, 
	GUIDE_MSG_TOUCH, 
};

class GuideControl 
{
public:
	enum ErrorCode { // 引导过程中可能产生的错误码
		ERROR_INVALID_GUIDE_ID = 1, // 无效的引导ID
		ERROR_INVALID_STEP_ID, // 无效的步骤ID
		ERROR_TOUCH_OUTSIDE_SCREEN, // 引导区域在屏幕之外
		ERROR_TOUCH_ZERO, // 引导区域为零
		ERROR_TOUCH_TOO_SMALL, // 引导区域小于UI缩放的最小尺寸
		ERROR_NULL_TOUCH_NODE, // 引导操作的显示对象为空
	};
	
public:
	GuideControl();
	~GuideControl();

	void init(); // 初始化

	void finalize(); // free

	bool isGuideDuring(); // 是否有引导正在进行中

	void trigger(unsigned int uTriggerType, unsigned int uLinkID);

	void startGuide(unsigned int uGuideID); // 开始引导

	void pushStep(); // 加入一个步骤，等待执行

	void execStep(); // 执行一个步骤，等待完成

	void nextStep(); // 一个步骤完成，push下一步，没有下一步则完成此次引导

	void endGuide(); // 结束引导

	void addGuide();

	void removeGuide();

	void showGuide(); // 显示引导视图

	void hideGuide(); // 隐藏引导视图

	bool isGuideContainer(GUIDE_UI_TYPE type); // 是否是引导操作的显示对象父容器

	bool isGuideNode(CCNode *pNode); // 是否是引导操作的显示对象

	bool isNewcomer();

	void setNewcomer(bool isNew);

	GuideContainer* getGuideContainer(unsigned int uViewType);

	void checkGuideNodeExit();

	bool isStoryStep();

	bool isTaskTriggerGuide(unsigned int uTaskID);

	unsigned int getGuideID();

	unsigned int getStepID();

private:
	bool m_bInitialized;

	GuideView *m_pGuideView; // 引导视图

	CCNode *m_pGuideContainer; // 引导操作的显示对象父容器

	GuideCallFunc *m_pGuideCallFunc; // 回调检测

	CCNode *m_pGuideNode; // 引导操作的显示对象

	bool m_bIsGuideDuring; // 是否有引导在进行

	unsigned int m_uGuideID; // 当前引导ID

	unsigned int m_uStepID; // 当前步骤ID

	int m_nStepIdx; // 当前引导的步骤索引

	unsigned int m_uRemainTimes; // 当前步骤剩余次数

	CCRect m_stShowRect; // 引导操作的显示对象的矩形区域

	bool m_bIsNewcomer;

};

#define P_GUIDE_CTRL CVLib::CVSingleton<GuideControl>::Instance()

#define GUIDE_TRIGGER(uTriggerType, uLinkID) \
do { \
	P_GUIDE_CTRL->trigger((uTriggerType), (uLinkID)); \
} while(0)

#define GUIDE_SHOW(uGuideUIType) \
do { \
	if(P_GUIDE_CTRL->isGuideDuring() && P_GUIDE_CTRL->isGuideContainer((uGuideUIType))) { \
		P_GUIDE_CTRL->execStep(); \
	} \
} while(0)

#define GUIDE_TOUCH(pNode) \
do { \
	if(P_GUIDE_CTRL->isGuideDuring() && P_GUIDE_CTRL->isGuideNode((pNode))) { \
		P_GUIDE_CTRL->nextStep(); \
	} \
} while(0)

#endif // GuideControl_h__