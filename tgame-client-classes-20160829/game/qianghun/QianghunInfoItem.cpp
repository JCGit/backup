#include "QianghunInfoItem.h"
#include "UserObj.h"
#include "TextHelper.h"
#include "ItemUtils.h"

CQianghunInfoItem::CQianghunInfoItem(){
	m_uLayer = 0;
	m_uItemHeight = 0;
}

CQianghunInfoItem::~CQianghunInfoItem(){

}

bool CQianghunInfoItem::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui/info_item_ui.json");
		addChild(m_pRootLayout);

		m_pDescTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("desc_txt"));
		m_pBgImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("bg_img"));

		bRet = true;
	} while (0);

	return bRet;
}

void CQianghunInfoItem::setLayer(unsigned int layer){
	m_uLayer = layer;
	updateUI();
}


void CQianghunInfoItem::updateUI(){
	char str[100];
	snprintf(str, 100,  TextStr(TEXT_QIANGHUN_GET_LEVEL) , m_uLayer);  
	m_pDescTxt->setText(str);

	unsigned int lev = P_USER_OBJ->getUserInfo()->level;
	const QiangHunCfg* stCfg = QIANGHUN_CFG_MASTER->GetCfgByLayerAndLev(m_uLayer, lev);
	if(stCfg == NULL){
		CV_ERROR(("%s, can not find layer = %u  and level = %u!", __PRETTY_FUNCTION__, m_uLayer, lev));
		return;
	}

	for(int i = 0; i < stCfg->getelmts_size(); i++){
		addOneItem(stCfg->getelmts(i), i);
	}

	m_pBgImg->setSize(CCSize(300, (stCfg->getelmts_size() - 1)*(m_uItemHeight + 10) + m_uItemHeight + 50));
	setSize(CCSize(300, 60 + m_pBgImg->getSize().height));
}

void CQianghunInfoItem::addOneItem(const QiangHunGetElement& stElmt, unsigned int idx){
	UILabel* pNameTxt = UILabel::create();
	pNameTxt->setColor(ccGREEN);
	pNameTxt->setFontName("fonts/tgamefont.ttf");
	pNameTxt->setFontSize(24);
	pNameTxt->setTextHorizontalAlignment(kCCTextAlignmentLeft); //ÓÒ¶ÔÆë
	pNameTxt->setAnchorPoint(ccp(0, 1.0f));
	m_pBgImg->addChild(pNameTxt);
	
	UILabel* pDescTxt = UILabel::create();
	pDescTxt->setColor(ccGREEN);
	pDescTxt->setFontName("fonts/tgamefont.ttf");
	pDescTxt->setFontSize(24);
	pDescTxt->setTextHorizontalAlignment(kCCTextAlignmentLeft); //ÓÒ¶ÔÆë
	pDescTxt->setAnchorPoint(ccp(0, 1.0f));
	m_pBgImg->addChild(pDescTxt);

	pNameTxt->setText(ItemUtils::getItemName(stElmt.type(), stElmt.cfgid()));
	pDescTxt->setText(getCountDesc(stElmt.countdesc()));

	float xx = 25;
	float yy = -25 - (pNameTxt->getSize().height + 10)*idx;
	pNameTxt->setPosition(ccp(xx, yy));
	pDescTxt->setPosition(pNameTxt->getPosition() + ccp(pNameTxt->getSize().width + 20, 0));
	m_uItemHeight = pNameTxt->getSize().height;
}

string CQianghunInfoItem::getCountDesc(unsigned int count){
	switch (count)
	{
	case 1:
		return TextStr(TEXT_QIANGHUN_FEW);
		break;
	case 2:
		return TextStr(TEXT_QIANGHUN_BIT);
		break;
	case 3:
		return TextStr(TEXT_QIANGHUN_MORE);
		break;
	case 4:
		return TextStr(TEXT_QIANGHUN_MASS);
		break;
	default:
		return TextStr(TEXT_QIANGHUN_FEW);
		break;
	}
}
