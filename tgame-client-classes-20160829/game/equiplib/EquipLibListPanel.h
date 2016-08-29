#ifndef EquipLibListPanel_h__
#define EquipLibListPanel_h__

#include "basic/BasePanel.h"
#include "CSEquipItemMsg.pb.h"
#include "EquipLibObj.h"
#include "game/guide/GuideControl.h"

using namespace std;

class EquipSelItem;


class EquipLibListPanel: public BasePanel, public CCTableViewDataSource, public CCTableViewDelegate, public GuideContainer
{
public:	
    enum 
    {
        EQUIP_LIST_ITEM_TAG = 23,
        EQUIP_LIB_MAX_TAB   = 5, //5个tab按钮
        EQUIP_PART_ALL      = 0, //所有装备
    };
public:
    EquipLibListPanel();
	virtual ~EquipLibListPanel();

	virtual CCNode* getGuideNodeByName(const std::string &name);

    CREATE_FUNC(EquipLibListPanel);
    // override
    virtual bool init();
    virtual void open();
    virtual void close();


	virtual CCSize cellSizeForTable(CCTableView *table);
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {}
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
private:
    void addOneListItem(const vmsg::CSEquipInfo& stInfo, const bool bIsWear, const int iBagIdx, const int iEquipPart);
    void touchEventHandler(CCObject *pSender, TouchEventType type);

    void filterEquipByPart(const int iEquipPart); //所有装备用EQUIP_PART_ALL表示

    bool static compareEquipLv(const EquipLibOneInfo& stInfoA, const EquipLibOneInfo& stInfoB);	

    void showTab(UIButton* m_pTabBtn, const int iEquipPart);
private:
    UIButton *m_pCloseBtn;

    UIButton* m_pBtnTabAll;
    UIButton* m_pBtnTabRing;
    UIButton* m_pBtnTabArmor;
    UIButton* m_pBtnTabCloth;
    UIButton* m_pBtnTabShoe;

	CCTableView* m_pTableView;

    vector<EquipLibOneInfo> m_stAllEquipInfos;

    UIButton* m_stTabBtnList[EQUIP_LIB_MAX_TAB];
};

#endif // EquipLibListPanel_h__
