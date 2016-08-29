#ifndef NihilityPanel_h__
#define NihilityPanel_h__

#include "BasePanel.h"
#include "ImageNumber.h"
#include "guide/GuideControl.h"

class CNihilityPanel : public BasePanel, public GuideContainer
{
public:
	CNihilityPanel();
	virtual ~CNihilityPanel();

	CREATE_FUNC(CNihilityPanel);

	bool init();

	virtual CCNode* getGuideNodeByName(const string &name);

	void updateUI();

	void updateRank(int rank);

	void open();
	void close();
	void closeLoad();

	void sendStart();
protected:
private:
	void initUI();
	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UIButton* m_pCloseBtn;
	UIButton* m_pNormalBtn;

	ImageView* m_pRankTipImg;

	ImageNumber* m_pScoreTxt;
	ImageNumber* m_pRankTxt;
	ImageNumber* m_pFloorTxt;
	ImageNumber* m_pManualTxt;
};

#endif // NihilityPanel_h__
