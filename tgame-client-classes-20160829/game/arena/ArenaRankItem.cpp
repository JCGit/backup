#include "ArenaRankItem.h"
#include "defines.h"

#include "ArenaLvCfgMaster.h"

CArenaRankItem::CArenaRankItem(){
	m_uRank = 0;
}

CArenaRankItem::~CArenaRankItem(){

}

CArenaRankItem* CArenaRankItem::create(UILayout *pRefLayout) {
	CArenaRankItem *pRet = new CArenaRankItem();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool CArenaRankItem::initWithLayout(UILayout *pRefLayout) {
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

bool CArenaRankItem::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/arena_rank_item_ui.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void CArenaRankItem::initUI(){
	m_pRankTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("rank_txt"));
	m_pNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("name_txt"));
	m_pLevTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("level_txt"));
	m_pLvTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("lv_txt"));
	m_pScoreTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("score_txt"));

	m_pItemBg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("item_bg"));

    m_pRankImg = UIImageView::create();
    m_pRankImg->setPosition(m_pRankTxt->getPosition());
    m_pRootLayout->addChild(m_pRankImg);
}

void CArenaRankItem::setInfo(const CSRankInfo& rankInfo, unsigned int rank){
	m_stInfo = rankInfo;
	m_uRank = rank;

	updateUI();
}

void CArenaRankItem::updateUI(){
	m_pNameTxt->setText(m_stInfo.nick());
	m_pLevTxt->setText("LV." + intToString(m_stInfo.level()));
	m_pLvTxt->setText(ARENA_LV_CFG_MASTER->GetCfgByScore(m_stInfo.value())->name());
	m_pScoreTxt->setText(intToString(m_stInfo.value()));

	m_pItemBg->setVisible(false);
	if((m_uRank + 1)%2 == 0){
		m_pItemBg->setVisible(true);
	}

    string strRankIcon("");
    switch((m_uRank+1)) {
    case 1:
        strRankIcon = "share/icon_first.png";
        break;
    case 2:
        strRankIcon = "share/icon_second.png";
        break;
    case 3:
        strRankIcon = "share/icon_third.png";
        break;
    }
    if(strRankIcon.size() > 0) {
        m_pRankImg->loadTexture(strRankIcon.c_str(), UI_TEX_TYPE_PLIST);
        m_pRankImg->setVisible(true);
        m_pRankTxt->setText("");
    } else {
        m_pRankImg->setVisible(false);
        m_pRankTxt->setText(intToString(m_uRank + 1));
    }
}