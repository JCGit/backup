#include "FightEndlessRankItem.h"
#include "defines.h"

CFightEndlessRankItem::CFightEndlessRankItem()
:m_pRankTxt(NULL)
,m_pNameTxt(NULL)
,m_pScoreTxt(NULL)
{

}

CFightEndlessRankItem::~CFightEndlessRankItem(){

}

CFightEndlessRankItem* CFightEndlessRankItem::create(UILayout *pRefLayout) {
	CFightEndlessRankItem *pRet = new CFightEndlessRankItem();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool CFightEndlessRankItem::initWithLayout(UILayout *pRefLayout) {
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

bool CFightEndlessRankItem::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/endless_rank_item_ui.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void CFightEndlessRankItem::initUI(){
	m_pRankTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("rank_txt"));
	m_pNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("name_txt"));
	m_pScoreTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("score_txt"));
	m_pRankImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("rank_img"));
	m_pBgImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("bg_img"));
}

void CFightEndlessRankItem::setObj(const vmsg::CSRankInfo &rankInfo){
	m_stRankInfo = rankInfo;
	updateUI();
}

const vmsg::CSRankInfo& CFightEndlessRankItem::getObj(){
	return m_stRankInfo;
}

void CFightEndlessRankItem::setRank(int rank){
	m_pRankTxt->setText(intToString(rank + 1));

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/share.plist");
	if(rank < 3){
		m_pRankImg->setVisible(true);
		m_pRankTxt->setVisible(false);

		if(rank == 0){
			m_pRankImg->loadTexture("share/icon_first.png", UI_TEX_TYPE_PLIST);
		}else if(rank == 1){
			m_pRankImg->loadTexture("share/icon_second.png", UI_TEX_TYPE_PLIST);
		}else if(rank == 2){
			m_pRankImg->loadTexture("share/icon_third.png", UI_TEX_TYPE_PLIST);
		}
	}else{
		m_pRankImg->setVisible(false);
		m_pRankTxt->setVisible(true);
	}

	if(rank%2 == 0){
		m_pBgImg->setVisible(false);
	}else{
		m_pBgImg->setVisible(true);
	}
}

void CFightEndlessRankItem::updateUI(){
	m_pNameTxt->setText(m_stRankInfo.nick());
	m_pScoreTxt->setText(intToString(m_stRankInfo.value()));
}