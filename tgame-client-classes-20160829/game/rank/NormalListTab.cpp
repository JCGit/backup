#include "NormalListTab.h"
#include "RankControl.h"
#include "NounHelper.h"
#include "defines.h"
#include "TimeEx.h"

#include "WarMapCfgMaster.h"

CNormalListTab::CNormalListTab()
:m_pTitleBg(NULL)
,m_pTitleNameTxt(NULL)
,m_pTitlePowerTxt(NULL)
,m_pTimeTxt(NULL)
,m_pMyRankBg(NULL)
,m_pMyPowerBg(NULL)
,m_pMyRankTxt(NULL)
,m_pMyPowerTxt(NULL)
,m_pMyPowerLabel(NULL)
{

}

CNormalListTab::~CNormalListTab(){

}

CNormalListTab* CNormalListTab::create(UILayout *pRefLayout) {
	CNormalListTab *pRet = new CNormalListTab();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool CNormalListTab::initWithLayout(UILayout *pRefLayout){
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutWithUI(pRefLayout);

		m_pTitleBg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("title_bg"));
		m_pTitleNameTxt = dynamic_cast<UILabel*>(m_pTitleBg->getChildByName("name_label"));
		m_pTitlePowerTxt = dynamic_cast<UILabel*>(m_pTitleBg->getChildByName("content_txt"));

		m_pAwardLayer = dynamic_cast<UIPanel*>(m_pRootLayout->getChildByName("award_layer"));
		m_pTimeTxt = dynamic_cast<UILabel*>(m_pAwardLayer->getChildByName("time_txt"));
		m_pCoinTxt = dynamic_cast<UILabel*>(m_pAwardLayer->getChildByName("coin_txt"));
		m_pArenaCoinTxt = dynamic_cast<UILabel*>(m_pAwardLayer->getChildByName("arean_coin_txt"));
		m_pTipTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("tip_txt"));
		m_pTipTxt->setVisible(false);

		m_pMyRankBg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("my_rank_bg"));
		m_pMyPowerBg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("my_power_bg"));
		m_pMyRankTxt = dynamic_cast<UILabel*>(m_pMyRankBg->getChildByName("my_rank_txt"));
		m_pMyPowerTxt = dynamic_cast<UILabel*>(m_pMyPowerBg->getChildByName("my_power_txt"));
		m_pMyPowerLabel = dynamic_cast<UILabel*>(m_pMyPowerBg->getChildByName("my_power_label"));

		m_pCoinTxt->setText("0");
		m_pArenaCoinTxt->setText("0");

		bRet = true;
	} while(0);

	return bRet;
}

void CNormalListTab::setRankType(unsigned int var){
	_rankType = var;

	switch (_rankType)
	{
	case RANK_TYPE_FIGHTPOWER:  //战力
		m_pTitlePowerTxt->setText(NounStr(POWER_TITLE));
		m_pMyPowerLabel->setText(NounStr(PLAYER_POWER_TXT) + ": ");
		break;
	case RANK_TYPE_WARMAP:		//关卡
		m_pTitlePowerTxt->setText(NounStr(WARMAP_TITLE));
		m_pMyPowerLabel->setText(NounStr(PLAYER_WARMAP_TXT) + ": ");
		break;
	case RANK_TYPE_NOTHINGLANDSCORE: //虚无之地
		m_pTitlePowerTxt->setText(NounStr(ENDLESS_TITLE));
		m_pMyPowerLabel->setText(NounStr(PLAYER_ENDLESS_TXT) + ": ");
		break;
	default:
		break;
	}
}

unsigned int CNormalListTab::getRankType(){
	return _rankType;
}

void CNormalListTab::updateUI(){

}

void CNormalListTab::updateMyRank(){
	const CSRankQueryMyRankRsp& stMyRank = P_RANK_CTL->getMyRankInfo();

	if(stMyRank.rank() < 0){
		m_pMyRankTxt->setText(NounStr(NO_RANK_TIP));
		m_pMyPowerTxt->setText("0");
		return;
	}

	m_pMyRankTxt->setText(to_string(stMyRank.rank() + 1));
	
	if(_rankType == RANK_TYPE_WARMAP){
		WarMapCfg* stCfg = WAR_MAP_CFG->GetWarMapCfgByID(stMyRank.score());
		if(stCfg != NULL){
			CCLOG("CNormalListTab::there is no war map cfg, cfg id == %d", stMyRank.score());
			m_pMyPowerTxt->setText(stCfg->name());
		}
	}else{
		m_pMyPowerTxt->setText(to_string(stMyRank.score()));
	}

	if(stMyRank.rank() > 1000){
		m_pTipTxt->setVisible(true);
		m_pAwardLayer->setVisible(false);
	}else{
		m_pTipTxt->setVisible(false);
		m_pAwardLayer->setVisible(true);
	}
}

void CNormalListTab::updateGift(){
	const CSAwardRankRsp& stAwardInfo = P_RANK_CTL->getRankAwardInfo();
	const google::protobuf::RepeatedPtrField<vmsg::CSAwardRankInfo>& stInfos = stAwardInfo.infos();
	for(int i = 0; i < stInfos.size(); i++){
		const vmsg::CSAwardRankInfo& tempInfo = stInfos.Get(i);
		if(_rankType == tempInfo.ranktype()){
			if(tempInfo.haslastaward()){
				m_pCoinTxt->setText(intToString(tempInfo.coin()));
				m_pArenaCoinTxt->setText(intToString(tempInfo.arenamoney()));
			}else{
				m_pCoinTxt->setText("0");
				m_pArenaCoinTxt->setText("0");
			}
			break;
		}
	}

	//得到倒计时，先加1
	_totalTime = stAwardInfo.remaintime();
	_totalTime++;
	m_pTimeTxt->setText(NounStr(TIME_COUNT_TXT) + " " + TimeEx::getStringTimeForInt(_totalTime));

	startCountDown();
}

void CNormalListTab::startCountDown(){
	schedule(schedule_selector(CNormalListTab::downTime), 1);
}

void CNormalListTab::clearCountDown(){
	unschedule(schedule_selector(CNormalListTab::downTime));
}

void CNormalListTab::downTime(float dt){
	_totalTime--;

	if(_totalTime <= 0){
		m_pTimeTxt->setText(NounStr(TIME_COUNT_TXT) + " " + TimeEx::getStringTimeForInt(0));
		unschedule(schedule_selector(CNormalListTab::downTime));
		P_RANK_CTL->sendQueryAwardRankRqst();	//时间到了再次查询
		return;
	}

	m_pTimeTxt->setText(NounStr(TIME_COUNT_TXT) + " " + TimeEx::getStringTimeForInt(_totalTime));
}

void CNormalListTab::touchEventHandler(CCObject* pSender, TouchEventType type){
	UIButton* pBtn = dynamic_cast<UIButton*>(pSender);
	const char* pName = pBtn->getName();
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		if(strcmp(pName, "get_btn") == 0){
			P_RANK_CTL->sendFetchRankAwardRqst(_rankType);
		}
		break;
	default:
		break;
	}
}