#ifndef RelivePanel_h__
#define RelivePanel_h__

#include "BasePanel.h"
#include "vlib/QEventDispatcher.h"

class CRelivePanel : public BasePanel, public CQEventDispatcher
{
public:
	static const string HERO_RELIVE_EVENT;				//����ɹ��¼�
	static const string HERO_CANCEL_RELIVE_EVENT;		//ȡ�������¼�
public:
	CRelivePanel();
	virtual ~CRelivePanel();

	CREATE_FUNC(CRelivePanel);

	bool init();

	void updateUI();

	void open();
	void close();

	void openWithCallFun(CCObject* target, SEL_CallFunc callbackFun);

	void doReliveDone();			//����ɹ�
protected:
private:
	void initUI();
	void touchEventHandler(CCObject *pSender, TouchEventType type);
	void doRelive();
	void reliveFun();

	Button* m_pCloseBtn;
	Button* m_pReliveBtn;
	Button* m_pCancelBtn;

	ImageView* m_pBtnGoldImg;
	ImageView* m_pBtnLabelImg;
	ImageView* m_pBtnReliveIcon;

	Layout* m_pCostItemLayout;
	Layout* m_pCostGoldLayout;

	Label* m_pBtnCostTxt;
	Label* m_pCostItemRightTxt;
	Label* m_pCostGoldNumTxt;
	Label* m_pCostGoldRightTxt;

	CCObject*    m_pTarget;
	SEL_CallFunc m_pEventSelector;

	bool m_bIsCostGold;				//�Ƿ���������
	unsigned int m_uCostGold;		//������������
};

#endif // RelivePanel_h__
