#ifndef SvrActListItem_h__
#define SvrActListItem_h__

#include "BaseLayout.h"

class CSvrActListItem : public BaseLayout
{
public:
	CSvrActListItem();
	virtual ~CSvrActListItem();

	CREATE_FUNC(CSvrActListItem);

	static CSvrActListItem* create(UILayout *pRefLayout);

	bool initWithLayout(UILayout *pRefLayout);

	virtual bool init();

	void initUI();

	void setActID(unsigned int uID);
	unsigned int getActID(){
		return m_uActID;
	}

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	unsigned int m_uActID;

	ImageView* m_pIconImg;
	ImageView* m_pTitleImg;

	Label *m_pActTimeTxt;
	Label *m_pActPrizeTxt;

	Button *m_pDummyBtn;
};

#endif // SvrActListItem_h__
