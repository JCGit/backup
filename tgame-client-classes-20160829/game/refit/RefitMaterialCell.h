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
		CELL_STAT_UNKNOWN, // δ֪����ʾΪ��
		CELL_STAT_EMPTY, // ȱʧ���Ҷ���ʾ
		CELL_STAT_ADD, // ����Ƕ
		CELL_STAT_ADDED, // ����Ƕ
		CELL_STAT_SHOW, // չʾ
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