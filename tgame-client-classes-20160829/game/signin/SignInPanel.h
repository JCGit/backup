#ifndef SignInPanel_h__
#define SignInPanel_h__

#include "basic/BasePanel.h"
#include "guide/GuideContainer.h"

class SignInPanel: public BasePanel, public CCTableViewDataSource, public CCTableViewDelegate, public GuideContainer
{
public:
    enum {
        LIST_ITEM_TAG   = 33, 
        LIST_MAX_NUM    = 25, //×î¶à25¸öitem

        TAG_TODAY_ICON = 111, 
    };

	SignInPanel();
	virtual ~SignInPanel();
	
	static SignInPanel* create();

	// override start

	virtual bool init();

    virtual void open();
    virtual void close();

	virtual bool isMeetAutoOpen();

	// override end

	void updateUI();
    

    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {}
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}

    void popAwardTip();
	virtual CCNode* getGuideNodeByName(const std::string &name);
private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);
    void updateDayInfo();

private:

    CCTableView* m_pTableView;

    CCSpriteBatchNode* m_pDaySigninNode;

    UILabel* m_stDayTxtLst[LIST_MAX_NUM];
    UILabel* m_pTimeTipTxt;

    UIImageView* m_pBgLeft;

    int m_iRegCnt;
    int m_iCanReg;
};

#endif // SignInPanel_h__