#ifndef MilestoneItem_h__
#define MilestoneItem_h__

#include "basic/BaseLayout.h"
#include "CSTaskMsg.pb.h"

class MilestoneItem: public BaseLayout
{
public:
	MilestoneItem();
	virtual ~MilestoneItem();

	static MilestoneItem* create();

	// override start

	virtual bool init();

	// override end

	void updateUI(const vmsg::CSTask& stTask);

	bool isTaskFinished();

	unsigned int getTaskID();

	unsigned int getTaskStat();

    void onTouch();
private:
private:
	UIImageView *m_pIconImg;

	UILabel *m_pTaskNameTxt;

	UILabel *m_pTaskDescTxt;

	UILabel *m_pTaskIntroTxt;

	UILabel *m_pGoldValTxt;

	UILabel *m_pCoinValTxt;

	UILoadingBar *m_pMilestoneProgress;

	UILabel *m_pProgressValTxt;

	UIImageView *m_pStampGetImg;

	unsigned int m_uTaskID;

	unsigned int m_uTaskStat;

	unsigned int m_uItemType;

};

#endif // MilestoneItem_h__