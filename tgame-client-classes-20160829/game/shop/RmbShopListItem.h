#ifndef RmbShopListItem_h__
#define RmbShopListItem_h__

#include "basic/BaseLayout.h"
#include "CSGoldMsg.pb.h"
using namespace vmsg;

class RmbShopListItemOne : public CCObject {
public:
    RmbShopListItemOne() {
        m_pRefLayout = NULL;

    }
    void init(UILayout* pRefImg);
    void setInfo(const CSRMBStoreProductInfo& stItemInfo, const bool bHasBought);
    void setEmpty();
    void onTouch();
    UILayout* getRefLayout() {
        return m_pRefLayout;
    }
private:

private:
    UILayout* m_pRefLayout;

    UIImageView* m_pCellBg;
    UIImageView* m_pCellImg;
	UIImageView* m_pDiscountImg;		//折扣图片
	UILabel* m_pAwardTxt;
    UILabel* m_pGoldNumTxt;
    UILabel* m_pRmbNumTxt;
    UILabel* m_pRmbNameTxt;
    UILabel* m_pDescTxt;
    
    CSRMBStoreProductInfo m_stItemInfo;
};

class RmbShopListItem: public BaseLayout
{
public:
	RmbShopListItem();
	virtual ~RmbShopListItem();
	
    static RmbShopListItem* create(UILayout *pRefLayout);
    static RmbShopListItem* create();

	bool initWithLayout(UILayout *pRefLayout);

	void updateUI(const CSRMBStoreProductInfo& stItemInfo, const bool bIsFirst, const bool bHasBought);
    void setTwoEmpty(); //隐藏第二个格子
    void onItemTouched(const CCPoint& stGlobalPt);
private:
    bool initLayout();
    void initUI();
	
private:
    RmbShopListItemOne m_stItemOne;
    RmbShopListItemOne m_stItemTwo;
};

#endif // RmbShopListItem_h__