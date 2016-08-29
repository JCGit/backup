#ifndef FillBulletPanel_h__
#define FillBulletPanel_h__

#include "BasePanel.h"
#include "vlib/QEventDispatcher.h"

class CFillBulletPanel : public BasePanel, public CQEventDispatcher
{
public:
	static const string FILL_BULLET_SUCCESS_EVENT;				//���䵯ҩ�ɹ��¼�
	static const string CANCEL_FILL_BULLET_EVENT;				//ȡ�����䵯ҩ�¼�
public:
	CFillBulletPanel();
	virtual ~CFillBulletPanel();

	CREATE_FUNC(CFillBulletPanel);

	bool init();

	void updateUI();

	void open();
	void close();

	void openWithCallFun(CCObject* target, SEL_CallFunc callbackFun);

	void doFillBulletDone();

protected:
private:
	void initUI();
	void touchEventHandler(CCObject *pSender, TouchEventType type);
	void doBuyBullet();
	void buyBulletFun();

	Button* m_pCloseBtn;
	Button* m_pFillBtn;
	Button* m_pCancelBtn;

	ImageView* m_pBtnGoldImg;
	ImageView* m_pBtnLabelImg;
	ImageView* m_pBtnFillIcon;

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

#endif // FillBulletPanel_h__
