#ifndef FragPanel_h__
#define FragPanel_h__

#include "basic/BasePanel.h"
#include "IconCell.h"


class FragPanel: public BasePanel, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
    enum {
        FRAG_LIST_ITEM_TAG = 87,

        TAB_WEAPON  = 1,
        TAB_EQUIP   = 2,
        TAB_AUTO_SEL = 3, //自动选择tab页（如果枪械tab空，且有装备碎片，则选择装备tab）
    };
	FragPanel();
	virtual ~FragPanel();
	
	static FragPanel* create();

    // override start
    virtual void open();
    virtual void close();

	virtual bool init();
	// override end

	void updateUI();

    void onSelBagIdx(const int iBagIdx);
    void onSelCfgID(const int iCfgID);
    
	virtual CCSize cellSizeForTable(CCTableView *table);
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {}
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
private:
    void touchEventHandler(CCObject* pSender, TouchEventType type);
    void showTab(UIButton* m_pBtn);

    void resetUI();
    void setSubPanelVisible(bool bIsVisible);
    void showFragInfo(const int iCfgID, const int iMyNum, const bool bShowMixBtn);
private:
    UIButton* m_pCloseBtn;
    UIImageView* m_pSubCloseBtnImg;

    UIButton* m_pTabBtnWeapoon;
    UIButton* m_pTabBtnEquip;
    int m_iNowTabID;

    UIImageView* m_pBgInfo;
    UIImageView* m_pSubPanelFrame;

    UILabel *m_pFromNameTxt;
    UIImageView* m_pFromCellBg;
    IconCell* m_pFromCell;
    UIImageView* m_pFragImg;

    UILabel *m_pToNameTxt;
    UIImageView* m_pToCellBg;
    IconCell* m_pToCell;

    UILabel* m_pCoinTxt;
    UIButton* m_pDoMixBtn;


	CCTableView* m_pTableView;
	
    unsigned int m_uMyCellCount; //我拥有的
    unsigned int m_uCfgCellCount; //配置中我没有的

    int m_iNowSelCfgID;
    int m_iNowCost;
    bool m_bIsEnough;

};

#endif // FragPanel_h__