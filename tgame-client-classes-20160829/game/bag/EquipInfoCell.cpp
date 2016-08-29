#include "EquipInfoCell.h"
#include "defines.h"
#include "NounHelper.h"
#include "ClientSvrMsg.pb.h"
#include "ItemUtils.h"
#include "equiplib/EquipLibControl.h"
#include "firearms/StarProgressBar.h"

#include "EquipmentCfgMaster.h"

EquipInfoCell::EquipInfoCell()
:m_pEquipImg(NULL)
,m_pEquipNameTxt(NULL)
,m_pEquipPartValTxt(NULL)
,m_pEquipLevelValTxt(NULL)
,m_pBaseAttrTxt(NULL)
,m_pBaseAttrValTxt(NULL)
,m_pBaseAttrAddValTxt(NULL)
,m_pEquipDescTxt(NULL)
,m_pSalePriceValTxt(NULL)
{
}

EquipInfoCell::~EquipInfoCell()
{
}

EquipInfoCell* EquipInfoCell::create(UILayout *pRefLayout) {
	EquipInfoCell *pRet = new EquipInfoCell();

	if(pRet != NULL && pRet->init(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool EquipInfoCell::init(UILayout *pRefLayout) {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutWithUICopy(pRefLayout);
		addChild(m_pRootLayout);

		m_pStatusImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("status_img"));

		m_pEquipImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("equip_img"));

		m_pBgImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_equip_img"));

		m_pArrowImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("arrow_img"));

		m_pEquipNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("equip_name_txt"));

		m_pEquipPartValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("equip_part_val_txt"));

		m_pEquipLevelValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("equip_level_val_txt"));

		m_pMaxLevTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("max_lev_txt"));

		m_pBaseAttrTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("base_attr_txt"));

		m_pBaseAttrValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("base_attr_val_txt"));

		m_pBaseAttrAddValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("base_attr_add_val_txt"));

		m_pEquipDescTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("equip_desc_txt"));

		m_pSalePriceValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("sale_price_val_txt"));

		m_pStarBar = CStarProgressBar::create("share/little_star_dark.png", "share/little_star_light.png", "ui_v2/share.png", 6);
		m_pStarBar->setAnchorPoint(ccp(0, 1.0f));
		float xx = m_pEquipNameTxt->getPositionX() - 10; 
		float yy = m_pEquipLevelValTxt->getPositionY() - 20;
		m_pStarBar->setPosition(ccp(xx, yy));
		m_pStarBar->setStarMargin(1);
		m_pStarBar->setStarNum(1, 6); //装备星级
		m_pRootLayout->addChild(m_pStarBar);


		bRet = true;
	} while(0);

	return bRet;
}

void EquipInfoCell::setStatus(unsigned int status) {
	if(status == STATUS_DRESSED) {
		m_pStatusImg->setVisible(true);
		m_pBaseAttrAddValTxt->setVisible(false);
		m_pArrowImg->setVisible(false);
	} else if(status == STATUS_SELECTED) {
		m_pStatusImg->setVisible(false);
	}

}

void EquipInfoCell::updateInfo(const vmsg::CSEquipInfo &stEquipInfo) {
	if(stEquipInfo.has_equipid()) {
		const EquipmentCfg *pEquipCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(stEquipInfo.equipid());

		std::string iconURL = std::string("item/") + pEquipCfg->icon() + ".png";
		m_pEquipImg->loadTexture(iconURL.c_str());

		m_pBgImg->loadTexture(ItemUtils::getColorBgImg(pEquipCfg->color()).c_str(), UI_TEX_TYPE_PLIST);

		m_pStarBar->setStarNum(pEquipCfg->initstar());

        m_pEquipNameTxt->setText(pEquipCfg->equipname());
		
		std::string typeName = "";

		if(pEquipCfg->part() == vmsg::EQUIP_POS_ARMOR) {
			typeName = NounStr(ARMOR);
		} else if(pEquipCfg->part() == vmsg::EQUIP_POS_RING) {
			typeName = NounStr(RING);
		} else if(pEquipCfg->part() == vmsg::EQUIP_POS_SHOE) {
			typeName = NounStr(SHOE);
		} else if(pEquipCfg->part() == vmsg::EQUIP_POS_CLOTH) {
			typeName = NounStr(CLOTH);
		}

		m_pEquipPartValTxt->setText(typeName);

		m_pEquipLevelValTxt->setText("Lv." + intToString(stEquipInfo.level()));

		const int iMaxLv = P_EQUIP_LIB_CTRL->getObj()->getEquipMaxLvUpLv(stEquipInfo.equipid());
		m_pMaxLevTxt->setText(intToString(iMaxLv));

		m_pEquipDescTxt->setText(pEquipCfg->equipdesc());

		m_pSalePriceValTxt->setText(intToString(pEquipCfg->coinprice()));

		std::string attrName = "";

		if(pEquipCfg->attr().attrid() == vmsg::ATTR_HP) {
			attrName = NounStr(NOUN_HP);
		} else if(pEquipCfg->attr().attrid() == vmsg::ATTR_MP) {
			attrName = NounStr(NOUN_MP);
		} else if(pEquipCfg->attr().attrid() == vmsg::ATTR_ATK) {
			attrName = NounStr(NOUN_ATTACK);
		} else if(pEquipCfg->attr().attrid() == vmsg::ATTR_DEF) {
			attrName = NounStr(NOUN_DEFENSE);
		} else if(pEquipCfg->attr().attrid() == vmsg::ATTR_MOVESPEED) {
			attrName = NounStr(NOUN_MOVE_SPEED);
		} else if(pEquipCfg->attr().attrid() == vmsg::ATTR_RUNSPEED) {
			attrName = NounStr(NOUN_RUN_SPEED);
        } else if(pEquipCfg->attr().attrid() == vmsg::ATTR_HIT) {
            attrName = NounStr(NOUN_ATTR_HIT);
        } else {
            CCLOG("[error]EquipInfoCell::updateInfo,invalid attr id %d", pEquipCfg->attr().attrid() );
        }

		m_pBaseAttrTxt->setText(attrName);
		int attrval = ItemUtils::getEquipAttrVal(stEquipInfo.equipid(), stEquipInfo.level(), stEquipInfo.quality());
		if(pEquipCfg->attr().attrid() == vmsg::ATTR_HIT) { // 命中
			m_pBaseAttrValTxt->setText(intToString((int)(floor(attrval * 2.3f + 0.5f))));
		} else {
			m_pBaseAttrValTxt->setText(intToString(attrval));
		}
		
        /*
		EquipRefineCostCfg stEquipRefineCostCfg;
		if(EQUIP_REFINE_COST_CFG_MASTER->GetCfg(stEquipInfo.refinelv(), pEquipCfg->part(), stEquipRefineCostCfg) == 0) {
			unsigned int color = pEquipCfg->color();
			m_pStrengthenAttrValTxt->setText(intToString(stEquipRefineCostCfg.colorattradd(color - 1)));
		}*/
        int iAttr = ItemUtils::getEquipShowAttrVal(stEquipInfo.equipid(), stEquipInfo.level(), stEquipInfo.quality() ); //强化加属性
        /*
        int iDeltaAttr = iAttr - pEquipCfg->attr().attrval();
        iDeltaAttr = iDeltaAttr >= 0 ? iDeltaAttr : 0;
		if(pEquipCfg->attr().attrid() == vmsg::ATTR_HIT) { // 命中
			m_pStrengthenAttrValTxt->setText(intToString((int)(floor(iAttr * 2.3f + 0.5f))));
		} else {
			m_pStrengthenAttrValTxt->setText(intToString(iDeltaAttr)); //显示增加的属性值
		}*/
		
	}
}

void EquipInfoCell::showAddVal(int baseAttrVal) {
	m_pBaseAttrAddValTxt->setText(intToString(abs(baseAttrVal)));

	ccColor3B baseAttrColor = baseAttrVal > 0 ? ccc3(0x00, 0xff, 0x00) : ccc3(0xff, 0x00, 0x00);
	m_pBaseAttrAddValTxt->setColor(baseAttrColor);

	m_pBaseAttrAddValTxt->setVisible(baseAttrVal != 0);
	m_pArrowImg->setVisible(baseAttrVal != 0);

	string str = "";
	if(baseAttrVal > 0){
		str = "share/arrow_up_img.png";
	}else{
		str = "share/down_arrow_img.png";
	}
	m_pArrowImg->loadTexture(str.c_str(), UI_TEX_TYPE_PLIST);
}
