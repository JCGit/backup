#include "RankItem.h"
#include "defines.h"
#include "RankControl.h"
#include "NounHelper.h"

#include "WarMapCfgMaster.h"

CRankItem::CRankItem()
:m_pRankImg(NULL)
,m_pRankTxt(NULL)
,m_pLevelTxt(NULL)
,m_pNameTxt(NULL)
,m_pScoreTxt(NULL)
,m_pTitleTxt(NULL)
{
	
}

CRankItem::~CRankItem(){

}

CRankItem* CRankItem::create(UILayout *pRefLayout) {
	CRankItem *pRet = new CRankItem();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool CRankItem::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/RankItemUI.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

bool CRankItem::initWithLayout(UILayout *pRefLayout) {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutWithUICopy(pRefLayout);
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void CRankItem::initUI(){
	m_pRankImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("rank_img"));

	m_BgImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_img"));

	m_pRankTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("rank_txt"));

	m_pLevelTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("player_level_txt"));

	m_pNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("player_name_txt"));

	m_pScoreTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("rank_value_txt"));

	m_pTitleTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("rank_title_txt"));

	m_pRankTxt->setVisible(false);
}

void CRankItem::setObj(const CSRankInfo &rankInfo, unsigned int rankType){
	m_stRankInfo = rankInfo;

	m_pLevelTxt->setText("Lv." + intToString(m_stRankInfo.level()));
	m_pNameTxt->setText(m_stRankInfo.nick());

	if(rankType == RANK_TYPE_WARMAP){
		WarMapCfg* stCfg = WAR_MAP_CFG->GetWarMapCfgByID(m_stRankInfo.value());

		if(stCfg != NULL){
			m_pScoreTxt->setFontName("fonts/tgamefont.ttf");
			m_pScoreTxt->setText(stCfg->name());
		}else{
			CCLOG("CRankItem::there is no war map cfg, cfg id == %llu", m_stRankInfo.value());
		}
	}else{
		m_pScoreTxt->setFontName("fonts/04b_03b.ttf");
		m_pScoreTxt->setText(to_string(m_stRankInfo.value()));
	}

	setTitle(rankType);
}

const CSRankInfo& CRankItem::getObj(){
	return m_stRankInfo;
}

void CRankItem::setRank(unsigned int var){
	_rank = var;

	if(_rank < 3){
		m_pRankImg->setVisible(true);
		m_pRankTxt->setVisible(false);

		if(_rank == 0){
			m_pRankImg->loadTexture("share/icon_first.png", UI_TEX_TYPE_PLIST);
		}else if(_rank == 1){
			m_pRankImg->loadTexture("share/icon_second.png", UI_TEX_TYPE_PLIST);
		}else if(_rank == 2){
			m_pRankImg->loadTexture("share/icon_third.png", UI_TEX_TYPE_PLIST);
		}

		string str = "rank/item_bg_" + intToString(_rank) + ".png";
		m_BgImg->loadTexture(str.c_str(), UI_TEX_TYPE_PLIST);
		m_BgImg->setVisible(true);
	}else{
		m_pRankImg->setVisible(false);
		m_pRankTxt->setVisible(true);
		m_pRankTxt->setText(intToString(_rank + 1));

		string str = "rank/item_bg.png";
		m_BgImg->loadTexture(str.c_str(), UI_TEX_TYPE_PLIST);
		if((_rank - 3)%2 == 0){
			m_BgImg->setVisible(true);
		}else{
			m_BgImg->setVisible(false);
		}
	}

}

void CRankItem::setTitle(unsigned int uType){
	string str = "";
	switch (uType)
	{
	case RANK_TYPE_WARMAP:
		str = NounStr(WARMAP_TITLE);
		break;
	case RANK_TYPE_FIGHTPOWER:
		str = NounStr(POWER_TITLE);
		break;
	case RANK_TYPE_NOTHINGLANDSCORE:
		str = NounStr(ENDLESS_TITLE);
		break;
	case RANK_TYPE_PAY:
		str = NounStr(NOUN_GOLD);
		break;
	default:
		str = NounStr(POWER_TITLE);
	}

	m_pTitleTxt->setText(str);
}

unsigned int CRankItem::getRank(){
	return _rank;
}