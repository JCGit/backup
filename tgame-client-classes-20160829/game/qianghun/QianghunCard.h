#ifndef QianghunCard_h__
#define QianghunCard_h__

#include "BaseLayout.h"
#include "IconCell.h"
#include "CSQiangHunMsg.pb.h"

using namespace vmsg;

class CQianghunCard : public BaseLayout
{
public:
	CQianghunCard();
	virtual ~CQianghunCard();

	CREATE_FUNC(CQianghunCard);

	bool init();

	void initClose();		//初始化状态

	void setIsFree(bool isFree);
	bool getIsFree(){
		return m_bIsFree;
	}

	void updateCost(unsigned int uCost);

	bool getIsOpen(){
		return m_bIsOpen;
	}

	void lock();
	void unLock();

	bool getLocked(){
		return m_bLocked;
	}

	void updateUI();

	void openCard();		//翻牌

	void closeCard();

	void openByInfo(const CSQiangHunElmt& stInfo, bool isOpened = false, bool isKeyTip = true);		//isOpened是否已经打开
	const CSQiangHunElmt& getCardInfo(){
		return m_stInfo;
	}

	unsigned int getCost(){
		return m_uCost;
	}

private:
	void openKeyCom();					//打开钥匙弹出提示
	void touchEventHandler(CCObject* pSender, TouchEventType type);

	ImageView* m_pBackImg;
	ImageView* m_pFrontImg;
	ImageView* m_pCostLightImg;

	UILabel* m_pCostTxt;

	ImageView* m_pItemBg;
	IconCell* m_pItem;

	CCActionInterval* m_openAnimIn;  
	CCActionInterval* m_openAnimOut;  

private:
	CSQiangHunElmt m_stInfo;

	unsigned int m_uCost;		
	bool m_bIsFree;				//是否免费翻牌
	bool m_bIsOpen;				//是否已经翻牌
	bool m_bLocked;				//是否锁住
	bool m_bIsTip;				//是否提示抽到钥匙
};

#endif // QianghunCard_h__
