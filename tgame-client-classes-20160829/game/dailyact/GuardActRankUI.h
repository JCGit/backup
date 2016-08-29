#ifndef GuardActRankUI_h__
#define GuardActRankUI_h__

#include "BaseLayout.h"
#include "CSCVZMsg.pb.h"

using namespace gui;
using namespace std;

class CGuardActRankUI : public BaseLayout
{
public:
	CGuardActRankUI();
	~CGuardActRankUI();

	bool init();

	CREATE_FUNC(CGuardActRankUI);
    
    void updateRankInfo(const vmsg::CSCVZRankInfo& stRankInfo);
    void updateTimeTip(const int iTime);

	void setParent(CCNode* parent);
    void updateTime(float dt);
protected:
    void addShadow(UILabel *pLabel);

    void showTimeTip(const int iTime);
private:
    UILabel *m_pTimeTipTxt; //µ¹¼ÆÊ±
    UILabel* m_pTimeAtlasTxt;

    int m_iTime;
};

#endif // GuardActRankUI_h__
