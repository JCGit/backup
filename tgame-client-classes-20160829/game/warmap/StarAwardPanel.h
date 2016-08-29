#pragma once

#include "basic/BasePanel.h"


class StarAwardPanel : public BasePanel ,public CCTargetedTouchDelegate
{
public:
	StarAwardPanel(void);
	~StarAwardPanel(void);

	CREATE_FUNC(StarAwardPanel);

	// override start

	virtual bool init();

	virtual void open();
	virtual void close();

	void show(const string& pdes, unsigned int packageid);

	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
private:
	void initUI();
	void onTouch(const CCPoint& stGlobalPt);
private:
	UIButton* m_yesBtn;
	UIButton* m_closeBtn;
	vector<UIImageView*> m_itemVec;
	UILabel* m_desTxt;
	vector<int> m_posXVec;
};

