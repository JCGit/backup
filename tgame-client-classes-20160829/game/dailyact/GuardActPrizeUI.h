#ifndef GuardActPrizeUI_h__
#define GuardActPrizeUI_h__

#include "BaseLayout.h"


using namespace gui;
using namespace std;

class CGuardActPrizeUI : public BaseLayout
{
public:
	CGuardActPrizeUI();
	~CGuardActPrizeUI();

	bool init();

	CREATE_FUNC(CGuardActPrizeUI);
    
    void updateMyContrNum(const int iMyContrNum);

	void updateMyCoinNum(const int iCoin);

	void setParent(CCNode* parent);
protected:
    void addShadow(UILabel *pLabel);
private:
    UILabel* m_pTotalValTxt; //total_val_txt
    UILabel* m_pKillTxt; 


};

#endif // GuardActPrizeUI_h__
