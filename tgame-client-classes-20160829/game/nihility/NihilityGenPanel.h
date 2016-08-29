#ifndef NihilityGenPanel_h__
#define NihilityGenPanel_h__

#include "BasePanel.h"
#include "CSInfiniteMsg.pb.h"
#include "game/guide/GuideControl.h"

class CNihilityGenPanel : public BasePanel, public GuideContainer
{
public:
	CNihilityGenPanel();
	virtual ~CNihilityGenPanel();

	CREATE_FUNC(CNihilityGenPanel);

	bool init();

	void updateUI();

	void updateItemByIdx(unsigned int index, const vmsg::CSGen& info);

	void updateRandDesc(const vmsg::CSGen& info);

	virtual void open();
	virtual void close();

	virtual CCNode* getGuideNodeByName(const std::string &name);

	void startFight();
protected:
private:
	void initUI();
	void touchEventHandler(CCObject *pSender, TouchEventType type);
	void listScrollEvent(CCObject* obj, ScrollviewEventType type);

	void buyManualTip();
	void buyManual();

	UIButton* m_pCloseBtn;
	UIButton* m_pEnterBtn;
	UILabel* m_pDescTxt;
	ImageView* m_pDescBg;
	ImageView* m_pSliderImg;
	ListView* m_pList;

	float m_fSliderMinY;
	float m_fSliderMaxY;
};

#endif // NihilityGenPanel_h__
