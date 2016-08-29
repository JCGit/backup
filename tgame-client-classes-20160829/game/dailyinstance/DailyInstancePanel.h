#ifndef DailyInstancePanel_h__
#define DailyInstancePanel_h__

#include "basic/BasePanel.h"

class ImageNumber;
class DailyInstanceListItem;

class DailyInstancePanel: public BasePanel, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
    enum 
    {
        INSTANCE_LIST_ITEM_TAG = 103,
        MAX_LV_NUM = 4, //四个难度
    };
	DailyInstancePanel();
	virtual ~DailyInstancePanel();
	
	static DailyInstancePanel* create();

	// override start

	virtual bool init();

    virtual void open();
    virtual void close();

	// override end

	void updateUI();

    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {}
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);
    void updateVipTipTxt();
    void updateSelInsance();
    void showLvTabByIdx(const int idx);
    void doStartFight();
    int getNowVipFightNum(); //获取当前VIP等级可以挑战的次数

	void buyManualTip();
	void buyManual();
private:
    CCTableView* m_pTableView;

    UIButton* m_pCloseBtn;

    UIImageView* m_pWarBg;
    UIImageView* m_pWarBgMask;

    UIButton* m_pLvTabBtn[MAX_LV_NUM];

    UIButton* m_pBtnFight;
    UIButton* m_pBtnCharge;

    UILabel* m_pTodayNumTxt;

    UILabel* m_pNowVipLvTxt;
    UILabel* m_pNextTipTxt0;
    UILabel* m_pNextVipLvTxt;
    UILabel* m_pNextTipTxt1;

    std::vector<int> m_stInstCfgKeyList; //副本显示列表,key
    DailyInstanceListItem* m_pNowSelItem;
    int m_iSelLvIdx; //当前选择的难度，index

    UIImageView* m_pCoinTipImg; //根据奖励类型来加载不同的提示图片
    ImageNumber* m_pCoinTipImgNum; //通关获得coin
    ImageNumber* m_pFpTipImgNum; //推荐战力

    ImageNumber* m_pDebrisStarNum; //碎片星级
};

#endif // DailyInstancePanel_h__