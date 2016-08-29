#ifndef EquipInfoCell_h__
#define EquipInfoCell_h__

#include "BaseLayout.h"
#include "CSEquipItemMsg.pb.h"

class CStarProgressBar;

class EquipInfoCell: public BaseLayout
{
public:
	enum CellStatus {
		STATUS_DRESSED = 0, // 身上的
		STATUS_SELECTED, // 选中的
	};

public:
	EquipInfoCell();
	virtual ~EquipInfoCell();
	
	static EquipInfoCell* create(UILayout *pRefLayout);

	bool init(UILayout *pRefLayout);

	void setStatus(unsigned int status);

	void updateInfo(const vmsg::CSEquipInfo &stEquipInfo);

	void showAddVal(int baseAttrVal);

private:
	ImageView *m_pStatusImg;

	ImageView *m_pEquipImg;

	ImageView* m_pBgImg;

	ImageView* m_pArrowImg;

	UILabel *m_pEquipNameTxt;

	UILabel *m_pEquipPartValTxt;

	UILabel *m_pEquipLevelValTxt;

	UILabel* m_pMaxLevTxt;

	UILabel *m_pBaseAttrTxt;

	UILabel *m_pBaseAttrValTxt;

	UILabel *m_pBaseAttrAddValTxt;

	UILabel *m_pEquipDescTxt;

	UILabel *m_pSalePriceValTxt;

	CStarProgressBar* m_pStarBar;

};

#endif // EquipInfoCell_h__