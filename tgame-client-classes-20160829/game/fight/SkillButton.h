#ifndef SkillButton_h__
#define SkillButton_h__

#include "BaseLayout.h"
#include "FireButton.h"
#include "vlib/QEventDispatcher.h"

using namespace CVLib;

class SkillButton : public BaseLayout, public CQEventDispatcher
{
public: 
	static const string SKILL_BTN_CLICK_EVENT;
public:
	SkillButton();
	virtual ~SkillButton();

	//idx��ʾ����˳��0��ǹе���ܡ�1����������2������(���ܲ���ȥ��skillID)
	static SkillButton* create(UILayout *pRefLayout, int idx, int cdTime, int skillID);

	virtual bool initWithLayout(UILayout *pRefLayout, int idx, int cdTime, int skillID);

	int getSkillID(){
		return m_uSkillID;
	} 

	void updateSkillIcon(unsigned int skillID);

	bool getBtnEnable(){
		return m_bIsBtnEnable;
	}

	void setBtnEnable(bool var);

	void startCD();

	void pauseCD();
	void resumeCD();
protected:

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void skillCoolDownCallBack();			//������ȴ�ص�

	void updateTime(float dt);

private:
	FireButton* m_pSkillBtn;
	ImageView* m_pSkillIcon;
	CCProgressTimer* m_pProgressTimer;		//��ȴcd 
	UILabel* m_pTimeTxt;					//����ʱ

	CC_SYNTHESIZE(bool, _isInCD, IsInCD);		//�Ƿ���cd��
	CC_SYNTHESIZE(bool, _isEnable, IsEnable);
	CC_PROPERTY(bool, _isUsable, IsUsable);	   //�Ƿ�ҵ�

	bool m_bIsBtnEnable;				//��ť�Ƿ����

	int m_uCDTime;
	int m_uSkillID;
	int m_uIDX;

	int m_uTimeCount;
};

#endif // SkillButton_h__
