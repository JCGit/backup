#ifndef SignInListItem_h__
#define SignInListItem_h__

#include "basic/BaseLayout.h"

class IconCell;

class SignInListItem : public BaseLayout
{
public:
    enum 
    {
        SIGN_ITEM_STAT_DONE = 1, //�Ѿ�ǩ������
        SIGN_ITEM_STAT_IDLE = 2, //û��ǩ������

        MAX_AWARD_NUM   = 2, //���2������
    };
	SignInListItem();
	virtual ~SignInListItem();

	static SignInListItem* create();

	static SignInListItem* create(UILayout *pRefLayout);

	// override 
	virtual bool init();

	bool initWithLayout(UILayout *pRefLayout);

	void initUI();
    
    void onTouch(const CCPoint& stGlobalPt);

    void setInfo(const int iIdx, const int iStat); 
    void setTodayCanSign(bool bCanSign);

	UIButton* getSignBtn();
private:

    UILabel* m_pDayTxt;

    UIImageView* m_pCell0Bg;
    IconCell* m_pIconCell;

	UIImageView* m_pCell1Bg;
	IconCell* m_pSecondIconCell;

    UIButton* m_pSignBtn;
    

    int m_iIndex;
    int m_iStat;
};

#endif // SignInListItem_h__
