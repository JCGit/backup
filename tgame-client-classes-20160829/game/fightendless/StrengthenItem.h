#ifndef StrengthenItem_h__
#define StrengthenItem_h__

#include "BaseLayout.h"
#include "IconCell.h"
#include "ClientSvrMsg.pb.h"
class CStrengthenItem : public BaseLayout
{
public:
	CStrengthenItem();
	virtual ~CStrengthenItem();

	CREATE_FUNC(CStrengthenItem);

	static CStrengthenItem* create(UILayout *pRefLayout);

	bool initWithLayout(UILayout *pRefLayout);

	bool init();

	void initUI();

	void setObj(unsigned int idx, const vmsg::CSGen& info);

	const vmsg::CSGen& getObj();

	unsigned int getIDX();
protected:
private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);
private:
	vmsg::CSGen m_stInfo;
	unsigned int m_uIDX;

	IconCell* m_pCell;
	UILabel* m_pDesTxt;
	UILabel* m_pBtnTxt;
	UIButton* m_pBtn;
};

#endif // StrengthenItem_h__
