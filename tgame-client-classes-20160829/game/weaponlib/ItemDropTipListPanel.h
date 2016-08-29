#ifndef ItemDropTipListPanel_h__
#define ItemDropTipListPanel_h__

#include "BasePanel.h"
#include "vlib/QEventDispatcher.h"
class IconCell;

typedef enum DropType
{
	DROP_WAR = 1,              //副本
	DROP_NIHILITY,			  //虚无之地
	DROP_LOTTERY,			 //夺宝奇兵
	DROP_BLACK_MARKET,  //黑市
	DROP_WEAPON_BOOK, //枪魂
	DROP_ARENA_SHOOP,  //特兰战地商店
}DROP_TYPE;

class ItemDropTipListItem : public BaseLayout {
public:
    ItemDropTipListItem();
    ~ItemDropTipListItem();

    static ItemDropTipListItem* create();

    // override start
    virtual bool init();
    virtual std::string getDescription() const;

    void setWarInfo(unsigned int idroptype, const int iWarID, const int iWarType, const int iThingType, const int iThingID);
    void updateUI();

    int getWarID() {
        return m_iWarID;
    }
    int getWarType() {
        return m_iWarType;
    }
	int getDropType(){
		return m_dropType;
	}

private:
    UIImageView* m_pMapImg;
    UILabel* m_pNameTxt;
    IconCell* m_IconCell;
	UILabel* m_pLvTxt;

    int m_iWarID;
    int m_iWarType;
    int m_iThingType;
    int m_iThingID;

	unsigned int m_dropType;
};

class ItemDropTipListPanel : public BasePanel, public CCTableViewDataSource, public CCTableViewDelegate, public CCTouchDelegate
{
public:
    enum {
        MAX_TIP_ITEM_NUM = 6,
		LIST_ITEM_TAG,
    };
	struct Drop_Info
	{
		unsigned int idroptype;
		int iWarID;
		int iWarType;
		int iThingType;
		int iThingID;
		Drop_Info(unsigned i1, int i2, int i3, int i4, int i5)
		{
			idroptype = i1;
			iWarID = i2;
			iWarType = i3;
			iThingType = i4;
			iThingID = i5;
		};
	};
	ItemDropTipListPanel();
	virtual ~ItemDropTipListPanel();

    bool init();


    void showItemInfo(const int iType, const int iCfgID) {
        m_iType = iType;
        m_iCfgID = iCfgID;
        open();
    }

    virtual void open();
    virtual void close();
    
    void updateUI();

	virtual CCSize cellSizeForTable(CCTableView *table);
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {}
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}

	 bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
private:
    void touchEventHandler(CCObject* pSender, TouchEventType type);

    void listViewEventHandler(CCObject *pSender, ListViewEventType type);

	UIButton* m_pCloseBtn;
	UIButton* m_pSureBtn;

	UILayout* m_pItemLayout;
	UILayout* m_pDropLayout;

	Label* m_pNameTxt;
	Label* m_pDescTxt;
	Label* m_pTipTxt;

	ImageView* m_pItemBg;
	IconCell* m_pItemCell;

    ListView* m_pTipList;
	CCTableView* m_pTipTable;
	vector<Drop_Info> m_pDropInfoVec;
    int m_iType;
    int m_iCfgID;
};

#endif // ItemDropTipListPanel_h__
