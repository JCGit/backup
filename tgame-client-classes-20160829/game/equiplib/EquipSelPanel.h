#ifndef EquipSelPanel_h__
#define EquipSelPanel_h__

#include "basic/BasePanel.h"
#include "CSEquipItemMsg.pb.h"
#include "EquipLibObj.h"
#include "game/guide/GuideControl.h"

using namespace std;

class EquipSelItem;


class EquipSelPanel: public BasePanel, public CCTableViewDataSource, public CCTableViewDelegate, public GuideContainer
{
public:	
	enum {
        EQUIP_SEL_ITEM_TAG = 101,
	};
public:
    EquipSelPanel();
	virtual ~EquipSelPanel();

	virtual CCNode* getGuideNodeByName(const std::string &name);

    CREATE_FUNC(EquipSelPanel);
    // override
    virtual bool init();
    virtual void open();
    virtual void close();

    /*
    //设置排序规则,bWearEquipFirst true,使用中的装备排在前面
    void setSortRule(bool bWearEquipFirst, int iFromEquipPart = 0) { 
        m_bWearEquipFirst = bWearEquipFirst;
        m_iFromEquipPart = iFromEquipPart;
    }
    */
	virtual CCSize cellSizeForTable(CCTableView *table);
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {}
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
private:
    void addOneListItem(const vmsg::CSEquipInfo& stInfo, const bool bIsWear, const int iBagIdx);
    void touchEventHandler(CCObject *pSender, TouchEventType type);

    void updateUI();

    bool static compareEquipLv(const EquipLibOneInfo& stInfoA, const EquipLibOneInfo& stInfoB);	
private:
    UIButton *m_pCloseBtn;

	CCTableView* m_pTableView;
	UILayout* m_pEquipDemo;

    EquipSelItem* m_pNowSelItem; //当前选择的Item

    vector<EquipLibOneInfo> m_stAllEquipInfos;

};

#endif // EquipSelPanel_h__
