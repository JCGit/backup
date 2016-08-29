#ifndef RankItem_h__
#define RankItem_h__

#include "BaseLayout.h"
#include "CSRankMsg.pb.h"

using namespace vmsg;

class CRankItem : public BaseLayout
{
public:
	CRankItem();
	virtual ~CRankItem();

	CREATE_FUNC(CRankItem);

	static CRankItem* create(UILayout *pRefLayout);

	bool init();

	bool initWithLayout(UILayout *pRefLayout);

	void initUI();

	void setObj(const CSRankInfo &rankInfo, unsigned int rankType);
	const CSRankInfo& getObj();

	void setTitle(unsigned int uType);

private:
	CSRankInfo m_stRankInfo;
	CC_PROPERTY(unsigned int, _rank, Rank);

	UIImageView* m_BgImg;

	UIImageView* m_pRankImg;

	UILabel* m_pRankTxt;

	UILabel* m_pLevelTxt;

	UILabel* m_pNameTxt;

	UILabel* m_pTitleTxt;

	UILabel* m_pScoreTxt;

};

#endif // RankItem_h__
