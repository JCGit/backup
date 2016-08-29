#ifndef IconFrameCell_h__
#define IconFrameCell_h__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace std;

class IconFrameCell: public UIWidget
{
public:
	IconFrameCell();
	virtual ~IconFrameCell();

	static IconFrameCell* create();

	// override start

	virtual bool init();

	// override end

	UIImageView* getIconImg();

	UIImageView* getFrameImg();

	unsigned int getType();

	unsigned int getCfgID();

	unsigned int getCnt();

	void setInfo(unsigned int uType, unsigned int uCfgID, unsigned int uCnt = 0);

	void clear();

private:
	UIImageView *m_pIconImg;

	UIImageView *m_pFrameImg;

	UIImageView *m_pFlagImg; // ÀÈ∆¨±Íº«

	UILabel *m_pCntTxt;

	unsigned int m_uType;

	unsigned int m_uCfgID;

	unsigned int m_uCnt;

};

#endif // IconFrameCell_h__