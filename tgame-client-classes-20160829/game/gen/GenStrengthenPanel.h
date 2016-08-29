#ifndef GenStrengthenPanel_h__
#define GenStrengthenPanel_h__

#include "BasePanel.h"
#include "game/guide/GuideControl.h"

class CGenStrengthenPanel : public BasePanel, public GuideContainer
{
public:
	CGenStrengthenPanel();
	virtual ~CGenStrengthenPanel();

	CREATE_FUNC(CGenStrengthenPanel);

	bool init();

	virtual CCNode* getGuideNodeByName(const std::string &name);

	void updateUI();

	void open();
	void close();

	void selectType(unsigned int uType);

	unsigned int getSelectType(){
		return m_uGenType;
	}

protected:
private:
	void initUI();
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void doGenCost();
	void doGen();

	Button* m_pCloseBtn;
	Button* m_pGenBtn;

	ImageView* m_pTypeImg;
	ImageView* m_pCostIcon;
	ImageView* m_pAtkBarBg;
	ImageView* m_pHpBarBg;
	ImageView* m_pDefBarBg;
	ImageView* m_pFullImg;

	Label* m_pTipTxt;
	Label* m_pCostTxt;
	Label* m_pAtkBarTxt;
	Label* m_pHpBarTxt;
	Label* m_pDefBarTxt;
	Label* m_pAtkUpTxt;
	Label* m_pHpUpTxt;
	Label* m_pDefUpTxt;

	LoadingBar *m_pAtkProgress;
	LoadingBar *m_pHpProgress;
	LoadingBar *m_pDefProgress;

	unsigned int m_uGenType;
	unsigned int m_uCostCnt;

	CCArray* m_pBtnLst;
};

#endif // GenStrengthenPanel_h__
