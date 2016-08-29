#include "GuildListTab.h"
#include "NounHelper.h"
#include "RankControl.h"
#include "defines.h"

CGuildListTab::CGuildListTab()
:m_pMyRankBg(NULL)
,m_pMyPowerBg(NULL)
,m_pMyRankTxt(NULL)
,m_pMyPowerTxt(NULL)
{
	
}

CGuildListTab::~CGuildListTab(){

}

CGuildListTab* CGuildListTab::create(UILayout *pRefLayout) {
	CGuildListTab *pRet = new CGuildListTab();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool CGuildListTab::initWithLayout(UILayout *pRefLayout){
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutWithUI(pRefLayout);

		m_pMyRankBg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("my_rank_bg"));
		m_pMyPowerBg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("my_power_bg"));

		m_pMyRankTxt = dynamic_cast<UILabel*>(m_pMyRankBg->getChildByName("my_rank_txt"));
		m_pMyPowerTxt = dynamic_cast<UILabel*>(m_pMyPowerBg->getChildByName("my_power_txt"));

		bRet = true;
	} while(0);

	return bRet;
}

void CGuildListTab::updateUI(){

}

void CGuildListTab::updateMyRank(){
	const CSRankQueryMyRankRsp& stMyRank = P_RANK_CTL->getMyRankInfo();

	if(stMyRank.rank() < 0){
		m_pMyRankTxt->setText(NounStr(NO_RANK_TIP));
		m_pMyPowerTxt->setText("0");
		return;
	}

	m_pMyRankTxt->setText(to_string(stMyRank.rank() + 1));
	m_pMyPowerTxt->setText(to_string(stMyRank.score()));
}