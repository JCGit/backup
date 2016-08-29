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

	//idx表示技能顺序，0：枪械技能、1：进化技、2：闪避(闪避不用去管skillID)
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

	void skillCoolDownCallBack();			//技能冷却回调

	void updateTime(float dt);

private:
	FireButton* m_pSkillBtn;
	ImageView* m_pSkillIcon;
	CCProgressTimer* m_pProgressTimer;		//冷却cd 
	UILabel* m_pTimeTxt;					//倒计时

	CC_SYNTHESIZE(bool, _isInCD, IsInCD);		//是否在cd中
	CC_SYNTHESIZE(bool, _isEnable, IsEnable);
	CC_PROPERTY(bool, _isUsable, IsUsable);	   //是否灰掉

	bool m_bIsBtnEnable;				//按钮是否可用

	int m_uCDTime;
	int m_uSkillID;
	int m_uIDX;

	int m_uTimeCount;
};

#endif // SkillButton_h__
