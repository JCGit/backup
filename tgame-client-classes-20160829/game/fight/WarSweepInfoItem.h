#ifndef WarSweepInfoItem_h__
#define WarSweepInfoItem_h__

#include "basic/BaseLayout.h"
#include "CSWarMapMsg.pb.h"

class WarSweepInfoItem: public BaseLayout
{
public:
	WarSweepInfoItem();
	virtual ~WarSweepInfoItem();

	static WarSweepInfoItem* create();

	// override start

	virtual bool init();

	// override end

	void updateUI(const vmsg::CSEliteAward &stAwardInfo);

private:
	UILabel *m_pWarMapNameTxt;

	UILabel *m_pAwardDescTxt;

};

#endif // WarSweepInfoItem_h__