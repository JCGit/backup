#ifndef __DAILY_ACT_LIST_ITEM_H__
#define __DAILY_ACT_LIST_ITEM_H__

#include "basic/BaseLayout.h"
#include "CSActMsg.pb.h"

using namespace vmsg;

class DailyActListItem: public BaseLayout
{
public:    
	DailyActListItem();
	virtual ~DailyActListItem();

	static DailyActListItem* create();

	static DailyActListItem* create(UILayout *pRefLayout);

	bool initWithLayout(UILayout *pRefLayout);

	// override start
	virtual bool init();
	virtual std::string getDescription() const;

	void initUI();

	void setActInfo(const CSActInfo& stMsg);

	void updateStat(unsigned int uStat);

	const CSActInfo& getActInfo(){
		return m_stActInfo;
	}

	void updateUI();

	void updateGuardAct();
	void updateMaualAct();
	void updateWorldBossAct();

	void updateWorldBossTishen(unsigned int uMark);
 
private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void btnClick();
	void btnSelect();
	void doSelect();
private:
	Layout* m_pInfoLayout;
	UIImageView* m_pTitleImg;
	UIImageView* m_pIconImg;

	UIImageView* m_pTishenImg;
	UIImageView* m_pSelectImg;
	UIImageView* m_pTishenLabelImg;

    UILabel *m_pActTimeTxt;
    UILabel *m_pActPrizeTxt;
    UILabel *m_pActStatTxt;

    UIButton *m_pDummyBtn;

	bool m_bIsSelect;

	CSActInfo m_stActInfo;
};

#endif // __DAILY_ACT_LIST_ITEM_H__
