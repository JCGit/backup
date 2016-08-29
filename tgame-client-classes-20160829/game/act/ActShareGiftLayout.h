#ifndef ActShareGiftLayout_h__
#define ActShareGiftLayout_h__

#include "ActLayout.h"
#include "CSCouponMsg.pb.h"

using namespace vmsg;

class ActShareGiftLayout : public ActLayout
{
public:
	ActShareGiftLayout();
	virtual ~ActShareGiftLayout();

	CREATE_FUNC(ActShareGiftLayout);

	virtual bool init();
	void initLayout(UILayout* pRefLayout);

	virtual void updateInfo(const vmsg::CSCouponActStatusItem& info);

	void setInfo(const CSCouponActStatusItem& stMsg);
	unsigned int getPkgType(){
		return m_uPkgType;
	}

	void setLayoutEnabel(bool bIsEnabel);

private:
	void initUI();

	void doGet();
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	Layout* m_pRefLayout;

	Label* m_pTimeTxt;
	Label* m_pContentTxt;

	Button* m_pGetBtn;
	ImageView* m_pBtnImg;

	unsigned int m_uPkgType;
	unsigned int m_uState;
};

#endif // ActShareGiftLayout_h__
