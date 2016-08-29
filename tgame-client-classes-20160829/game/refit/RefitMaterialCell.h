#ifndef RefitMaterialCell_h__
#define RefitMaterialCell_h__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace std;

class RefitMaterialCell: public Widget
{
public:
	enum CellStat{
		CELL_STAT_UNKNOWN, // 未知，显示为空
		CELL_STAT_EMPTY, // 缺失，灰度显示
		CELL_STAT_ADD, // 可镶嵌
		CELL_STAT_ADDED, // 已镶嵌
		CELL_STAT_SHOW, // 展示
	};

public:
	RefitMaterialCell();
	virtual ~RefitMaterialCell();

	static RefitMaterialCell* create();

	// override start

	virtual bool init();

	virtual void setSize(const CCSize &size);

	// override end

	unsigned int getItemID();

	void setItemID(unsigned int uDotaItemID);

	unsigned int getStat();

	void setStat(CellStat stat);

private:
	CellStat m_stat;

	unsigned int m_uItemID;

	UIImageView *m_pIconImg;

	UIImageView *m_pFrameImg;

	UIImageView *m_pIconAddImg;

};

#endif // RefitMaterialCell_h__