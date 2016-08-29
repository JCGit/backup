#ifndef QianghunInfoItem_h__
#define QianghunInfoItem_h__

#include "BaseLayout.h"
#include "QiangHunCfgMaster.h"

using namespace std;

class CQianghunInfoItem : public BaseLayout
{
public:
	CQianghunInfoItem();
	virtual ~CQianghunInfoItem();

	CREATE_FUNC(CQianghunInfoItem);

	bool init();

	void setLayer(unsigned int layer);
	unsigned int getLayer(){
		return m_uLayer;
	}

	void updateUI();

	void addOneItem(const QiangHunGetElement& stElmt, unsigned int idx);

	string getCountDesc(unsigned int count);

protected:
private:
	UILabel* m_pDescTxt;
	ImageView* m_pBgImg;

	unsigned int m_uLayer;
	unsigned int m_uItemHeight;
};

#endif // QianghunInfoItem_h__
