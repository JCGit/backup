#ifndef ActTabLayout_h__
#define ActTabLayout_h__

#include "BaseLayout.h"
#include "CSCouponMsg.pb.h"

using namespace vmsg;

class CActTabLayout : public BaseLayout
{
public:
	CActTabLayout();
	virtual ~CActTabLayout();

	CREATE_FUNC(CActTabLayout);

	static CActTabLayout* create(UILayout *pRefLayout);

	bool init();

	bool initWithLayout(UILayout *pRefLayout);

	void setSelected(bool var);
	bool getSelected();

	void setInfo(const CSCouponActStatusItem& stMsg);
	const CSCouponActStatusItem& getInfo(){
		return m_stInfo;
	}

	bool isFestive();

	void setFestive(bool bFestive);

	void updateUI();

private:
	void initUI();

	ImageView* m_pBgImg;
	ImageView* m_pIconImg;

	Label* m_pNameTxt;
	
	bool m_bIsSelected;

	bool m_bFestive;

	CSCouponActStatusItem m_stInfo;
};

#endif // ActTabLayout_h__
