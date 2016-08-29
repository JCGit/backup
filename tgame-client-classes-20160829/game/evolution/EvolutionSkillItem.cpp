#include "EvolutionSkillItem.h"

#include "defines.h"
#include "Resources.h"

#include "CfgMaster/SkillCfgMaster.h"
#include "CfgMaster/EvolutionSkillGrowCfgMaster.h"

#include "basic/TextHelper.h"

EvolutionSkillItem::EvolutionSkillItem()
:m_bSelected(false)
,m_pBgImg(NULL)
,m_pSkillIconImg(NULL)
,m_pSkillNameTxt(NULL)
,m_pSkillLevelTxt(NULL)
{
}

EvolutionSkillItem::~EvolutionSkillItem()
{
}

EvolutionSkillItem* EvolutionSkillItem::create() {
	EvolutionSkillItem *pRet = new EvolutionSkillItem();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool EvolutionSkillItem::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

bool EvolutionSkillItem::isSelected() {
	return m_bSelected;
}

void EvolutionSkillItem::selected(bool bSelected) {
	m_bSelected = bSelected;

	if(m_bSelected) {
		m_pBgImg->loadTexture("share/bg_interior_2.png", UI_TEX_TYPE_PLIST);
	} else {
		m_pBgImg->loadTexture("share/bg_interior_1.png", UI_TEX_TYPE_PLIST);
	}
}

void EvolutionSkillItem::updateUI(const vmsg::CSEvolutionSkill& stSkill) {
	EvolutionSkillGrowCfg *pstSkillGrowCfg = EVOLUTION_SKILL_GROW_CFG_MASTER->getEvolutionSkillGrowCfgByID(stSkill.startskillid());

	unsigned int uSkillID = pstSkillGrowCfg->skillids(stSkill.level() - 1);
	const SkillCfg *pstSkillCfg = SKILL_CFG_MASTER->GetSkillCfgByID(uSkillID);

	std::string path = SKILLICON_RES_PATH + pstSkillCfg->skillicon() + ".png";
	m_pSkillIconImg->loadTexture(path.c_str());
	/*std::string level = SKILLICON_RES_PATH + "lv" + intToString(stSkill.level()) + ".png";
	m_pSkillLevelImg->loadTexture(level.c_str());*/
	m_pSkillNameTxt->setText(pstSkillCfg->name());
	m_pSkillLevelTxt->setText(std::string("Lv.") + intToString(stSkill.level()));

	/*if((int)stSkill.level() < pstSkillGrowCfg->skillids_size()) {
		unsigned int uNextSkillID = pstSkillGrowCfg->skillids(stSkill.level());
		const SkillCfg *pstNextSkillCfg = SKILL_CFG_MASTER->GetSkillCfgByID(uNextSkillID);

		m_pNextEffectValTxt->setText(pstNextSkillCfg->desc());
	} else {
		m_pNextEffectValTxt->setText(TextStr(TEXT_NOTHING));
	}*/
}

void EvolutionSkillItem::initUI() {
	initLayoutFromJsonFile("ui_v2/EvolutionSkillItemUI.ExportJson");
	addChild(m_pRootLayout);

	m_pBgImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_img"));

	m_pSkillIconImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("item_img"));

	m_pSkillNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("skill_name_txt"));

	m_pSkillLevelTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("skill_level_txt"));

}
