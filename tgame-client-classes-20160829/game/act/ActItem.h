#ifndef ActItem_h__
#define ActItem_h__

#include "basic/BaseLayout.h"
#include "CSCouponMsg.pb.h"

class ActItem: public BaseLayout
{
public:
	enum ActStyle {
		ACT_STYLE_GENERAL,
		ACT_STYLE_FESTIVE,
	};
public:
	ActItem();
	virtual ~ActItem();

	static ActItem* create();

	// override start

	virtual bool init();

	// override end

	unsigned int getActStyle();

	void setActStyle(ActStyle style);

	bool isSelected();

	void selected(bool bSelected);

	const vmsg::CSCouponActStatusItem& getInfo();

	void setInfo(const vmsg::CSCouponActStatusItem& info);

private:
	void initUI();

	ActStyle transStyle(unsigned int uStyle);

private:
	UIImageView *m_pBgImg;

	UIImageView *m_pIconImg;

	UILabel *m_pActNameTxt;

	ActStyle m_style;

	bool m_bSelected;

	vmsg::CSCouponActStatusItem m_stInfo;
	CCArmature* m_pstArmature;
};

#endif // ActItem_h__