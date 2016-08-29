#ifndef FightEndlessRankItem_h__
#define FightEndlessRankItem_h__

#include "BaseLayout.h"
#include "CSRankMsg.pb.h"

class CFightEndlessRankItem : public BaseLayout
{
public:
	CFightEndlessRankItem();
	virtual ~CFightEndlessRankItem();

	CREATE_FUNC(CFightEndlessRankItem);

	static CFightEndlessRankItem* create(UILayout *pRefLayout);

	bool initWithLayout(UILayout *pRefLayout);

	bool init();

	void initUI();

	void setObj(const vmsg::CSRankInfo &rankInfo);
	const vmsg::CSRankInfo& getObj();

	void setRank(int rank);

	void updateUI();
protected:
private:
	vmsg::CSRankInfo m_stRankInfo;

	ImageView* m_pRankImg;
	ImageView* m_pBgImg;
	UILabel* m_pRankTxt;
	UILabel* m_pNameTxt;
	UILabel* m_pScoreTxt;
};

#endif // FightEndlessRankItem_h__
