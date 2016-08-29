#ifndef ArenaRankItem_h__
#define ArenaRankItem_h__

#include "BaseLayout.h"
#include "CSRankMsg.pb.h"

using namespace vmsg;

class CArenaRankItem : public BaseLayout
{
public:
	CArenaRankItem();
	virtual ~CArenaRankItem();

	CREATE_FUNC(CArenaRankItem);

	static CArenaRankItem* create(UILayout *pRefLayout);

	bool init();

	bool initWithLayout(UILayout *pRefLayout);

	void initUI();

	void setInfo(const CSRankInfo& rankInfo, unsigned int rank);
	const CSRankInfo& getInfo(){
		return m_stInfo;
	}

	unsigned int getRank(){
		return m_uRank;
	}

	void updateUI();
protected:
private:
	UILabel* m_pRankTxt;
	UILabel* m_pNameTxt;
	UILabel* m_pLevTxt;
	UILabel* m_pLvTxt;
	UILabel* m_pScoreTxt;

	ImageView* m_pItemBg;

    UIImageView* m_pRankImg; //前三名显示图片

	unsigned int m_uRank;
	CSRankInfo m_stInfo;
};

#endif // ArenaRankItem_h__
