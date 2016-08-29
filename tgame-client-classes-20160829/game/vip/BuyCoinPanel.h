#ifndef BuyCoinPanel_h__
#define BuyCoinPanel_h__

#include "basic/BasePanel.h"

class BuyCoinPanel: public BasePanel
{
public:
	BuyCoinPanel();
	virtual ~BuyCoinPanel();
	
	static BuyCoinPanel* create();

	// override start

	virtual bool init();

    virtual void open();
    virtual void close();

	// override end

	void updateUI();

	void showMultiEff(unsigned int uMulti);
    
private:
	void onEffDone(CCArmature* armature, MovementEventType type, const char* name);
	void touchEventHandler(CCObject *pSender, TouchEventType type);
    void doBuy();
    void onBuyCallback();
private:
    UILabel* m_pBuyNumTxt;
    UILabel* m_pGoldNumTxt;
    UILabel* m_pCoinNumTxt;

    UIButton* m_pCloseBtn;
    UIButton* m_pBuyBtn;

	ImageView* m_pIconImg;
};

#endif // BuyCoinPanel_h__