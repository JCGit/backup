#ifndef SweepInfoItem_h__
#define SweepInfoItem_h__

#include "BaseLayout.h"
#include "CSWarMapMsg.pb.h"

using namespace std;
using namespace vmsg;

class CSweepInfoItem : public BaseLayout
{
public:
	CSweepInfoItem();
	virtual ~CSweepInfoItem();

	CREATE_FUNC(CSweepInfoItem);

	bool init();

	void setInfo(const CSSweepAward& stAwardInfo, unsigned int idx);
	const CSSweepAward& getInfo();

	void updateUI();
protected:
private:
	UILabel* m_pNumTxt;
	UILabel* m_pExpTxt;
	UILabel* m_pCoinTxt;

	UILabel *m_pAwardDescTxt;

	CSSweepAward m_stAwardInfo;

	unsigned int m_IDX;
};

#endif // SweepInfoItem_h__
