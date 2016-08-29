#ifndef RestrictShopPanel_h__
#define RestrictShopPanel_h__

#include "BasePanel.h"
#include "CSStoreMsg.pb.h"

using namespace vmsg;

class CRestrictShopItem;

class CRestrictShopPanel : public BasePanel
{
public:
	enum 
	{
		MAX_ITEM_NUM = 8, //最多8个物品
	};

	CRestrictShopPanel();
	virtual ~CRestrictShopPanel();

	CREATE_FUNC(CRestrictShopPanel);

	bool init();

	void updateUI(const CSPanicBuyingStoreQueryRsp& stInfo, bool bIsUpdateTime = true);

	void open();
	void close();

private:
	void initUI();
	void timeCountDown(float dt);
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	CRestrictShopItem* m_stItemLst[MAX_ITEM_NUM];

	Label* m_pTimeTxt;
	Button* m_pCloseBtn;

	unsigned int m_uTimeCount;
};

#endif // RestrictShopPanel_h__
