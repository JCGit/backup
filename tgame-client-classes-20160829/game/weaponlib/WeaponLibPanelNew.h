#ifndef WeaponLibPanelNew_h__
#define WeaponLibPanelNew_h__

#include "BasePanel.h"
#include "BasicTypes.h"

#include "game/guide/GuideControl.h"

class CWeaponTypeItem;

class CWeaponLibPanelNew : public BasePanel, public CCTableViewDataSource, public CCTableViewDelegate, public GuideContainer
{
public:
    enum 
    {
        INIT_WEAPON_NUM = 20,
    };
	CWeaponLibPanelNew();
	virtual ~CWeaponLibPanelNew();

	CREATE_FUNC(CWeaponLibPanelNew);

    bool init();

    virtual void open();
    virtual void close();
    
	virtual CCNode* getGuideNodeByName(const std::string &name);

    void updateUI();

	void updateWeaponList();
        
	void updateWeaponByChange();			//换枪后的更新

	int getWeaponInBagInfoIdxByGUID(UINT64_t weaponGUID);

	void setTypeIdx(unsigned int idx);
	unsigned int getTypeIdx();

	unsigned int getWeaponIdx();

	void clear();

	virtual CCSize cellSizeForTable(CCTableView *table);
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {}
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
private:
	void initUI();
	void touchEventHandler(CCObject* pSender, TouchEventType type);
	void typeListViewEventHandler(CCObject *pSender, ListViewEventType type);
        
    UIListView* m_pBtnList;


	CCTableView* m_pTableView;
	UILayout* m_pWeaponDemo; //为了防止重复加载json
	unsigned int m_uCellCount;
    
private:
	int m_iSelectTypeIdx;			//当前选择的枪械类型下标
	int m_uSelectWeapon;		//当前选择的枪械

	int m_uTypeSize;			//枪械的类型数量

	int m_uPlayerWeaponIdx;		//玩家身上枪械的索引

	unsigned int m_uWeaponTotalSize;	//该类型的所有枪械，背包和配置的总和 
	unsigned int m_uWeaponInBag;		//背包中该类型枪械的数量
    
	CCArray* m_pTabBtnLst;
	CCTableViewCell* m_pSelectCell;
	CWeaponTypeItem* m_pTypeSelectItem;

	UIButton *m_pBackBtn;

};

#endif // WeaponLibPanelNew_h__
