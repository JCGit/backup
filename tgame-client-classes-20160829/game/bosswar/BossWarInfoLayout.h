#ifndef BossWarInfoLayout_h__
#define BossWarInfoLayout_h__

#include "BaseLayout.h"
#include "CSBossChallengeMsg.pb.h"

class CBossWarInfoLayout : public BaseLayout
{
public:
	CBossWarInfoLayout();
	virtual ~CBossWarInfoLayout();

	CREATE_FUNC(CBossWarInfoLayout);

	bool init();

	void updateInfo(const vmsg::CSBossChallengeDamageNotification& stInfo);

protected:
private:
	void delPreContent(unsigned int uLen);

	Label* m_pInfoTxt;

	unsigned int m_uContentLen;		//�ı�����

	map<string, unsigned int> m_stContentMap;
};

#endif // BossWarInfoLayout_h__
