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

	void initClose();		//��ʼ��״̬

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

	void openCard();		//����

	void closeCard();

	void openByInfo(const CSQiangHunElmt& stInfo, bool isOpened = false, bool isKeyTip = true);		//isOpened�Ƿ��Ѿ���
	const CSQiangHunElmt& getCardInfo(){
		return m_stInfo;
	}

	unsigned int getCost(){
		return m_uCost;
	}

private:
	void openKeyCom();					//��Կ�׵�����ʾ
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
	bool m_bIsFree;				//�Ƿ���ѷ���
	bool m_bIsOpen;				//�Ƿ��Ѿ�����
	bool m_bLocked;				//�Ƿ���ס
	bool m_bIsTip;				//�Ƿ���ʾ�鵽Կ��
};

#endif // QianghunCard_h__
