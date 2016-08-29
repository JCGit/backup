#ifndef NihilityResultPanel_h__
#define NihilityResultPanel_h__

#include "BasePanel.h"
#include "ImageNumber.h"
#include "CSInfiniteMsg.pb.h"

class CNihilityResultPanel : public BasePanel
{
public:
	CNihilityResultPanel();
	virtual ~CNihilityResultPanel();

	CREATE_FUNC(CNihilityResultPanel);

	bool init();

	void updateUI();

	void close();

	bool static compareByType(const vmsg::CSAwardElement& stInfoA, const vmsg::CSAwardElement& stInfoB);	
protected:
private:
	void initUI();
	void touchEventHandler(CCObject *pSender, TouchEventType type);
	void listScrollEvent(CCObject* obj, ScrollviewEventType type);

	UIButton* m_pSureBtn;
	UIButton* m_pShareBtn;
	UIImageView* m_pRoleImg;
	UILabel* m_pScoreTxt;
	UILabel* m_pCoinTxt;
	UILabel* m_pMaxTxt;

	ImageNumber* m_pScoreLabel;
	ImageView* m_pSliderImg;

	UILayout* m_pItemDemo;
	ScrollView* m_pAwardList;

	vector<vmsg::CSAwardElement> m_stAwardInfo; //当前列表显示的材料

	float m_fSliderMinY;
	float m_fSliderMaxY;
};

#endif // NihilityResultPanel_h__
