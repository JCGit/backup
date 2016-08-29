#ifndef TUITableViewCell_h__
#define TUITableViewCell_h__

#include "CocoStudio/GUI/BaseClasses/UIWidget.h"
#include "GUI/CCScrollView/CCSorting.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace gui;

class TUITableViewCell : public Widget, public CCSortableObject
{
public:
	TUITableViewCell() {}

    unsigned int getIdx();
    void setIdx(unsigned int uIdx);
  
    void reset();

    void setObjectID(unsigned int uIdx);
    unsigned int getObjectID();

private:
    unsigned int m_uIdx;
};

#endif // TUITableViewCell_h__
