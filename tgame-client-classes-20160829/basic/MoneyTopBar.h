#ifndef MoneyTopBar_h__
#define MoneyTopBar_h__

#include "BaseLayout.h"


class MoneyTopBar : public BaseLayout
{
public:
	MoneyTopBar();
	~MoneyTopBar();
    CREATE_FUNC(MoneyTopBar);

    bool init();
    virtual void setVisible(bool visible);
    void updateUI();
private:

    void touchEventHandler(CCObject *pSender, TouchEventType type);
    void buyManual();
    
private:
    UIButton* m_pBtnCoin;
    UIButton* m_pBtnGold;
    UIButton* m_pBtnMannual;

    UILabel* m_pTxtCoin;
    UILabel* m_pTxtGold;
    UILabel* m_pTxtMannual;

};


//#define MONEY_TOP_BAR CVLib::CVSingleton<MoneyTopBar>::Instance()

#endif // MoneyTopBar_h__
