#ifndef ExchangeInfoPanel_h__
#define ExchangeInfoPanel_h__

#include "BasePanel.h"
#include "IconCell.h"

class CExchangeInfoPanel : public BasePanel
{
public:
	CExchangeInfoPanel();
	virtual ~CExchangeInfoPanel();

	CREATE_FUNC(CExchangeInfoPanel);

	bool init();

	void setInfo(unsigned int cfgID, unsigned int cfgType, unsigned int cfgCnt, unsigned int idx, unsigned int coin);

	void updateUI();

	unsigned int getCfgID(){
		return m_uCfgID;
	}

	unsigned int getCfgType(){
		return m_uCfgType;
	}

	unsigned int getCfgCnt(){
		return m_uCfgCnt;
	}

	unsigned int getCoin(){
		return m_uCoin;
	}

	unsigned int getIdx(){
		return m_uIdx;
	}
protected:
private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UIButton* m_pCloseBtn;
	UIButton* m_pExchangeBtn;

	UILabel* m_pNameTxt;
    UILabel* m_pDescTxt;
    UILabel* m_pBagNumTxt;
    UILabel* m_pArenaCoinTxt;

	ImageView* m_pIconBg;

	IconCell* m_pItem;

	unsigned int m_uCfgID;
	unsigned int m_uCfgType;
	unsigned int m_uCfgCnt;
	unsigned int m_uCoin;
	unsigned int m_uIdx;
};

#endif // ExchangeInfoPanel_h__
