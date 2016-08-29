#ifndef EquipSelMatPanel_h__
#define EquipSelMatPanel_h__

#include "basic/BasePanel.h"
#include "CSEquipItemMsg.pb.h"
#include "EquipLibObj.h"

using namespace std;


class EquipSelMatPanel: public BasePanel, public CCTableViewDataSource, public CCTableViewDelegate
{
public:	
	enum {
        EQUIP_SEL_ITEM_TAG = 101,
	};
public:
    EquipSelMatPanel();
	virtual ~EquipSelMatPanel();


    CREATE_FUNC(EquipSelMatPanel);
    // override
    virtual bool init();
    virtual void open();
    virtual void close();
    const vector<EquipSelMatOneInfo>& findMat(); //�Զ�ѡ�������

	virtual CCSize cellSizeForTable(CCTableView *table);
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {}
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
private:
    void addOneEquip(const vmsg::CSEquipInfo& stInfo, const int bagIdx);
    void addSomeItem(const int itemCfgID, const int cnt, const int bagIdx); //��Ҫ���ѵ��ĵ��߲�ֿ�
    void touchEventHandler(CCObject *pSender, TouchEventType type);

    void updateUI();

    bool static compareMat(const EquipSelMatOneInfo& stInfoA, const EquipSelMatOneInfo& stInfoB);	
private:
    UIButton *m_pCloseBtn;

	CCTableView* m_pTableView;
	UILayout* m_pEquipDemo;

    vector<EquipSelMatOneInfo> m_stMatInfo; //��ǰ�б���ʾ�Ĳ���

    set<int> m_stNowSelMatIdx; //��ǰѡ��Ĳ���index
};

#endif // EquipSelMatPanel_h__
