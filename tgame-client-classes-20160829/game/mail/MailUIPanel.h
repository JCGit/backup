#ifndef __MAIL_UI_PANEL_H__
#define __MAIL_UI_PANEL_H__

#include "basic/BasePanel.h"
#include "BasicTypes.h"
#include <vector>

using namespace std;


class MailUIPanel: public BasePanel, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
    enum 
    {
        MAIL_LIST_ITEM_TAG = 101, //tag
    };

	MailUIPanel();
	virtual ~MailUIPanel();
    CREATE_FUNC(MailUIPanel);

	// override
	virtual bool init();

	virtual std::string getDescription() const;

    virtual void open();
    virtual void close();
    void updateUI();

    //overwrite
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {}
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}

private:
    void touchEventHandler(CCObject *pSender, TouchEventType type);
    void doGetAllAttach();
private:

    CCTableView* m_pTableView;

    UIButton *m_pCloseBtn;
    UIButton *m_pAutoDelBtn; //一键删除
    UIButton *m_pAddAllBtn; //一键领取
    UILabel *m_pNumTipTxt; //邮件数量

    vector<UNIT64_t> m_stMailDataLst;
};

#endif // __MAIL_UI_PANEL_H__
