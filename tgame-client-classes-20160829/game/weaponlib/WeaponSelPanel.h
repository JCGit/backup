#ifndef WeaponSelPanel_h__
#define WeaponSelPanel_h__

#include "basic/BasePanel.h"
#include "BasicTypes.h"
#include "CSWeaponMsg.pb.h"
#include "game/guide/GuideContainer.h"

using namespace gui;
using namespace std;

class WeaponSelPanel : public BasePanel, public CCTableViewDataSource, public CCTableViewDelegate, public GuideContainer
{
public:
	enum {
		MAIN_WEAPON_TYPE = 1,		//Ö÷ÎäÆ÷
		DEPUTY_WEAPON_TYPE = 2,		//¸±ÎäÆ÷
	};
public:
    WeaponSelPanel();
    virtual ~WeaponSelPanel();

    CREATE_FUNC(WeaponSelPanel);

    virtual bool init();

	virtual CCNode* getGuideNodeByName(const std::string &name);

    void updateUI();

    void open();
    void close();

    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {}
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}

private:
    void touchEventHandler(CCObject* pSender, TouchEventType type);
private:
    void initUI();
    CCTableView* m_pTableView;
    UILayout* m_pWeaponDemo;
    Button* m_pCloseBtn;

    unsigned int m_uCellCount;

	vector<vmsg::CSWeapon> m_stSelWeaponLst;

	CC_SYNTHESIZE(unsigned int, m_uSelType, SelType);
};

#endif // WeaponSelPanel_h__
