#ifndef BagCell_h__
#define BagCell_h__

#include "basic/BaseLayout.h"
#include "CSBagMsg.pb.h"

class BagCell: public BaseLayout
{
public:
	BagCell();
	virtual ~BagCell();

	static BagCell* create(UILayout *pRefLayout);

	// override start

	virtual std::string getDescription() const;

	// override end

	virtual bool initWithLayout(UILayout *pRefLayout);

	bool isSelected();

	void selected(bool bSelected);

	const vmsg::CSBagCellInfo& getInfo();

	void setInfo(const vmsg::CSBagCellInfo &stBagCellInfo);
    
	void clearInfo();

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	UIImageView *m_pBgItemImg;

	UIImageView *m_pItemImg;

	UIImageView *m_pItemFrameImg;

	UILabel *m_pNumTxt;

	bool m_bSelected; //  «∑Ò—°÷–

	vmsg::CSBagCellInfo m_stBagCellInfo;

};

#endif // BagCell_h__
