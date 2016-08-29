#ifndef GenStrengthenEffectPanel_h__
#define GenStrengthenEffectPanel_h__

#include "BasePanel.h"
#include "game/guide/GuideControl.h"

class CGenStrengthenEffectPanel : public BasePanel, public GuideContainer
{
public:
	CGenStrengthenEffectPanel();
	virtual ~CGenStrengthenEffectPanel();

	CREATE_FUNC(CGenStrengthenEffectPanel);

	bool init();

	virtual CCNode* getGuideNodeByName(const std::string &name);

	void updateUI();

	void open();
	void close();

protected:
private:
	void initUI();
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void setAddStr(int addVal, unsigned int totalAdd, Label* pLabel, ImageView* pTip);
	void doConfirm(unsigned int isAccept);

	Button* m_pSaveBtn;
	Button* m_pCancelBtn;
	Button* m_pSureBtn;

	Label* m_pAtkTxt;
	Label* m_pHpTxt;
	Label* m_pDefTxt;

	ImageView* m_pAtkTip;
	ImageView* m_pHpTip;
	ImageView* m_pDefTip;

};

#endif // GenStrengthenEffectPanel_h__
