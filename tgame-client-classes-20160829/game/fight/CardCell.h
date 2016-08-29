#ifndef CardCell_h__
#define CardCell_h__

#include "BaseLayout.h"
#include "vlib/QEventDispatcher.h"

class IconCell;

class CCardCell : public BaseLayout, public CQEventDispatcher
{
public:
	enum {
		FREE_CARD,
		COST_CARD
	};

	static const string OPEN_OTHER_CARDS_EVENT;

public:
	CCardCell();
	virtual ~CCardCell();

	CREATE_FUNC(CCardCell);

	virtual bool init();

	void updateUI();

	void openCard(bool isGeted = true);		//翻牌

	void setItemInfo(unsigned int itemID, unsigned int itemType, unsigned int itemCnt);

	void lock();
	void unLock();

	bool getLocked(){
		return m_bLocked;
	}

private:
	void handleEffComplete(CCArmature * armature, MovementEventType type, const char * name);

	ImageView* m_pCardImg;
	ImageView* m_pIconBgImg;
	IconCell* m_pCell;
	ImageView* m_pGoldImg;
	Label* m_pCostTxt;


private:
	CC_SYNTHESIZE(unsigned int, _itemID, ItemID);		//物件id
	CC_SYNTHESIZE(unsigned int, _itemType, ItemType);	//类型
	CC_SYNTHESIZE(unsigned int, _itemCnt, ItemCnt);		//数量
	CC_PROPERTY(unsigned int, _costType, CostType);		//花费类型，免费 OR 花费
	CC_SYNTHESIZE(bool, _isOpen, IsOpen);				//是否打开
	CC_SYNTHESIZE(bool, _isGeted, IsGeted);				//是否得到该物品

	bool m_bLocked;			//是否锁住

};

#endif // CardCell_h__
