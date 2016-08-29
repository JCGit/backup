#ifndef ExchangeItem_h__
#define ExchangeItem_h__

#include "BaseLayout.h"
#include "IconCell.h"
#include "CSArenaMsg.pb.h"

using namespace vmsg;

class CExchangeItem : public BaseLayout
{
public:
	CExchangeItem();
	virtual ~CExchangeItem();

	CREATE_FUNC(CExchangeItem);

	bool init();

	void setInfo(const CSArenaStroeItem& stInfo, unsigned int idx);
	const CSArenaStroeItem& getInfo(){
		return m_stInfo;
	}

	unsigned int getIdx(){
		return m_uIdx;
	}

	void updateUI();
protected:
private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	ImageView* m_pItemBg;
	ImageView* m_pGetImg;
	UILabel* m_pCoinTxt;
    UILabel* m_pNameTxt;
	IconCell* m_pItem;

	CSArenaStroeItem m_stInfo;
	unsigned int m_uIdx;
};

#endif // ExchangeItem_h__
