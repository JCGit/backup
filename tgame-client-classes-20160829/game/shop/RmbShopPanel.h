#ifndef RmbShopPanel_h__
#define RmbShopPanel_h__

#include "basic/BasePanel.h"
#include "CSGoldMsg.pb.h"

class ImageNumber;

class RmbShopPanel: public BasePanel, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
    enum 
    {
        RMB_SHOP_ITEM_TAG = 102, 
    };
	enum ShopTab {
		SHOP_TAB_GOLD = 1,
		SHOP_TAB_ITEM = 2,
	};
	RmbShopPanel();
	virtual ~RmbShopPanel();
	
	static RmbShopPanel* create();

	// override start

    virtual void open();
    virtual void close();

	virtual bool init();

	// override end

	void updateUI(const vmsg::CSQueryRMBStoreRsp& stRsp);
    
	virtual CCSize cellSizeForTable(CCTableView *table);
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {}
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}

	void switchTab(ShopTab tab);

private:
    void touchEventHandler(CCObject *pSender, TouchEventType type);

	void showTabByIdx(unsigned int iIdx);

private:
    
	CCTableView* m_pTableView;

    UIButton* m_pCloseBtn;
    UIButton* m_pVipBtn;
	UIButton* m_pChargeBtn;
	UIButton* m_pItemBtn;

    UILabel* m_pRemainGoldTxt;
    UILabel* m_pGoldTipTxt;
    UILabel* m_pNextVipTxt;

    UILoadingBar* m_pGoldBar;
    UILabel* m_pGoldBarValTxt;
    ImageNumber* m_pVIPTxt;

    UILabel* m_pGoldTipLabel0;

    vmsg::CSQueryRMBStoreRsp m_stRmbShopRsp;
    std::set<int> m_stBoughtIDSet; //已经购买过的id
	std::vector<vmsg::CSRMBStoreProductInfo> m_stItemLst;

	unsigned int m_uIdx;
	unsigned int m_uCellCnt;
};

#endif // RmbShopPanel_h__