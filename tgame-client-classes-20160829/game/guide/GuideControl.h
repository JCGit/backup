#ifndef GuideControl_h__
#define GuideControl_h__

#include "vlib/base/CVSingleton.h"

#include "GuideContainer.h"
#include "GuideView.h"
#include "GuideCallFunc.h"

typedef enum GuideID { // ������ƥ��
	GUIDE_ID_NEWCOMER = 1,		// ���ֽ���
	GUIDE_ID_FIRST_TASK,		// ��һ������
	GUIDE_ID_EQUIP_TAKEON,		// װ������
	GUIDE_ID_WEAPON_LVUP,		// ǹе����
	GUIDE_ID_WEAPON_STEP,		// ǹе����
	GUIDE_ID_WEAPON_LVUP_AGAIN, // �ٴ�ǹе����
	GUIDE_ID_LOTTERY_ADVANCED,	// �߼�̽��
	GUIDE_ID_NOTHING_LAND,		// ����֮��
	GUIDE_ID_EQUIP_LVUP,		// װ������
	GUIDE_ID_LOTTERY_BATCH,		// ʮ����
	GUIDE_ID_FRIEND,			// ��Ӻ���
	GUIDE_ID_ELITE_BATTLE,		// ��Ӣ����
	GUIDE_ID_ARENA,				// ������
	GUIDE_ID_REFIT,				// ��װ
	GUIDE_ID_WEAPON_ARRAY,		// ս��
	GUIDE_ID_GENE_STRENGTHEN,	// ����ǿ��
} GUIDE_ID;

typedef enum GuideUIType { // ������ƥ��
	GUIDE_UI_NONE = 0,			// ���������ʾ����
	GUIDE_UI_CITY_SCREEN,		// ͬ��������
	GUIDE_UI_SUB_ENTRY,			// �������
	GUIDE_UI_ROLE,				// ��ɫ
	GUIDE_UI_EQUIP_INFO,		// װ����Ϣ
	GUIDE_UI_EQUIP_LIB,			// װ������
	GUIDE_UI_EQUIP_SEL,			// װ��ѡ��
	GUIDE_UI_WEAPON_LIB,		// ǹе��
	GUIDE_UI_WEAPON_SELECT,		// ǹеѡ��
	GUIDE_UI_WEAPON_INFO,		// ǹе��Ϣ
	GUIDE_UI_WEAPON_MATERIAL,	// ǹе����
	GUIDE_UI_WEAPON_STEP_DONE,	// ǹе���׳ɹ�
	GUIDE_UI_LOTTERY,			// �ᱦ���
	GUIDE_UI_WAR_MAP,			// �ؿ�ѡ��
	GUIDE_UI_WAR_ENTER,			// ���븱��
	GUIDE_UI_FRIEND,			// ����
	GUIDE_UI_ARENA,				// ������
	GUIDE_UI_ARENA_MATCH,		// ������ƥ��
	GUIDE_UI_NOTHING_LAND,		// ����֮��
	GUIDE_UI_UILAYER,			// UIBoxLayer
	GUIDE_UI_COMMON_DIALOG,		// �����Ի���
	GUIDE_UI_EQUIP_LIST,		// ǹе�б�
	GUIDE_UI_NOTHING_LAND_ENTER,// ����֮�س���
	GUIDE_UI_ELITE_WAR_ENTER,	// ���뾫Ӣ����
	GUIDE_UI_REFIT,				// ��װ
	GUIDE_UI_REFIT_MATERIAL,	// ��װ����
	GUIDE_UI_WEAPON_ARRAY,		// ս��
	GUIDE_UI_WEAPON_SEL,		// ǹеѡ��
	GUIDE_UI_GENE_STRENGTHEN,	// ����ǿ��
	GUIDE_UI_STRENGTHEN_EFFECT, // ǿ��Ч��
	GUIDE_UI_CHANGEWEAPON_INFO,//��λ��ǹе��Ϣ
	GUIDE_UI_DAILYTASK,		//ÿ������
	GUIDE_UI_SIGNIN,      //ǩ��
	GUIDE_UI_EVOLUTION,   //���ܽ���
} GUIDE_UI_TYPE;

enum GUIDE_MSG_ID {
	GUIDE_MSG_SHOW, 
	GUIDE_MSG_HIDE, 
	GUIDE_MSG_TOUCH, 
};

class GuideControl 
{
public:
	enum ErrorCode { // ���������п��ܲ����Ĵ�����
		ERROR_INVALID_GUIDE_ID = 1, // ��Ч������ID
		ERROR_INVALID_STEP_ID, // ��Ч�Ĳ���ID
		ERROR_TOUCH_OUTSIDE_SCREEN, // ������������Ļ֮��
		ERROR_TOUCH_ZERO, // ��������Ϊ��
		ERROR_TOUCH_TOO_SMALL, // ��������С��UI���ŵ���С�ߴ�
		ERROR_NULL_TOUCH_NODE, // ������������ʾ����Ϊ��
	};
	
public:
	GuideControl();
	~GuideControl();

	void init(); // ��ʼ��

	void finalize(); // free

	bool isGuideDuring(); // �Ƿ����������ڽ�����

	void trigger(unsigned int uTriggerType, unsigned int uLinkID);

	void startGuide(unsigned int uGuideID); // ��ʼ����

	void pushStep(); // ����һ�����裬�ȴ�ִ��

	void execStep(); // ִ��һ�����裬�ȴ����

	void nextStep(); // һ��������ɣ�push��һ����û����һ������ɴ˴�����

	void endGuide(); // ��������

	void addGuide();

	void removeGuide();

	void showGuide(); // ��ʾ������ͼ

	void hideGuide(); // ����������ͼ

	bool isGuideContainer(GUIDE_UI_TYPE type); // �Ƿ���������������ʾ��������

	bool isGuideNode(CCNode *pNode); // �Ƿ���������������ʾ����

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

	GuideView *m_pGuideView; // ������ͼ

	CCNode *m_pGuideContainer; // ������������ʾ��������

	GuideCallFunc *m_pGuideCallFunc; // �ص����

	CCNode *m_pGuideNode; // ������������ʾ����

	bool m_bIsGuideDuring; // �Ƿ��������ڽ���

	unsigned int m_uGuideID; // ��ǰ����ID

	unsigned int m_uStepID; // ��ǰ����ID

	int m_nStepIdx; // ��ǰ�����Ĳ�������

	unsigned int m_uRemainTimes; // ��ǰ����ʣ�����

	CCRect m_stShowRect; // ������������ʾ����ľ�������

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