#ifndef WarMapCell_h__
#define WarMapCell_h__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace std;

typedef enum {
	WAR_STAT_LOCKED, // 未解锁
	WAR_STAT_OPENED, // 已开放
	WAR_STAT_PASSED, // 已通关
} WarStat;

class WarMapCell: public UIWidget
{
public:
	WarMapCell();
	virtual ~WarMapCell();

	static WarMapCell* create();

	virtual bool init();

	unsigned int getWarMapID();

	void setWarMapID(unsigned int uWarMapID);

	unsigned int getWarStat();

	void setWarStat(WarStat stat);

	virtual void setStarNum(unsigned int num);

protected:
	virtual void changeWarStatLogic(WarStat stat);

protected:
	unsigned int m_uWarMapID;

	unsigned int m_uWarStat;

	UIImageView *m_pWarImg;

	UIImageView *m_pStar1Img;

	UIImageView *m_pStar2Img;

	UIImageView *m_pStar3Img;

};

class StageWarMapCell: public WarMapCell
{
public:
	StageWarMapCell();
	virtual ~StageWarMapCell();

	static StageWarMapCell* create();

	virtual bool init();

	void setStarNum(unsigned int num);

protected:
	virtual void changeWarStatLogic(WarStat stat);

private:
	
};

class EliteWarMapCell: public WarMapCell
{
public:
	EliteWarMapCell();
	virtual ~EliteWarMapCell();

	static EliteWarMapCell* create();

	virtual bool init();

protected:
	virtual void changeWarStatLogic(WarStat stat);

private:
	UIImageView* m_pElitePassImg;

};

#endif // WarMapCell_h__