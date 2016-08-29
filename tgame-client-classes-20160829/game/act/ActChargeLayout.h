#ifndef ActChargeLayout_h__
#define ActChargeLayout_h__

#include "ActLayout.h"
#include "CSCouponMsg.pb.h"

using namespace vmsg;

class CActChargeLayout : public ActLayout
{
public:
	enum{
		MAX_ITEM_NUM = 4,
	};

	CActChargeLayout();
	virtual ~CActChargeLayout();

	CREATE_FUNC(CActChargeLayout);

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

	CCArray *m_pUICellArray;
	CCArray *m_pCellArray;

	Button* m_pGetBtn;
	ImageView* m_pBtnImg;

	unsigned int m_uPkgType;
	unsigned int m_uState;

};

#endif // ActChargeLayout_h__
