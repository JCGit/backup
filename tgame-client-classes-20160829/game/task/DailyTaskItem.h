#ifndef DailyTaskItem_h__
#define DailyTaskItem_h__

#include "basic/BaseLayout.h"
#include "CSTaskMsg.pb.h"

class DailyTaskItem: public BaseLayout
{
public:
	DailyTaskItem();
	virtual ~DailyTaskItem();

	static DailyTaskItem* create();

	// override start

	virtual bool init();

	// override end

	void updateUI(const vmsg::CSTask& stTask);

	void updateUI(); // month card

	bool isTaskFinished();

	unsigned int getTaskID();

	unsigned int getTaskStat();

	Widget* getGoToButton();
private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void buyManual();

	void onBuyManualConfirm();

private:
	UIImageView *m_pIconImg;

	UILabel *m_pTaskNameTxt;

	UILabel *m_pTaskDescTxt;

	UILabel *m_pTaskIntroTxt;

	//UIImageView *m_pAwardTypeImg;

	UIImageView *m_pIconAwardImg;

	UILabel *m_pAwardValTxt;

	UILabel *m_pProgressValTxt;

	UIButton *m_pGotoBtn;

	UIImageView *m_pStampGetImg;

	unsigned int m_uTaskID;

	unsigned int m_uTaskStat;

	bool m_bIsMonthCard; // ÊÇ·ñÊÇÔÂ¿¨

};

#endif // DailyTaskItem_h__