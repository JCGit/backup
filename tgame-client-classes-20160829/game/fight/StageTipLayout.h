#ifndef StageTipLayout_h__
#define StageTipLayout_h__

#include "BaseLayout.h"

class CStageTipLayout : public BaseLayout
{
public:
	CStageTipLayout();
	virtual ~CStageTipLayout();

	CREATE_FUNC(CStageTipLayout);

	bool init();

	void showStageTip(unsigned int warId, unsigned int stageIdx);
protected:
private:
	void showEnd();

	ImageView* m_pBgImg;
	UILabel* m_pTxt;

	unsigned int m_uWarID;
	unsigned int m_uStageIdx;
};

#endif // StageTipLayout_h__
