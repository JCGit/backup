#ifndef SvrCell_h__
#define SvrCell_h__

#include "basic/BaseLayout.h"

#include "LoginObj.h"

class SvrCell: public BaseLayout
{
public:
	SvrCell();
	virtual ~SvrCell();

	static SvrCell* create();

	// override start

	virtual bool init();

	virtual std::string getDescription() const;

	// override end

	void setInfo(const SvrInfo &stSvrInfo);

	const SvrInfo& getInfo();

private:
	UIImageView *m_pBgImg;

	UILabel *m_pSvrNameTxt;

	SvrInfo m_stSvrInfo;
};

#endif // SvrCell_h__
