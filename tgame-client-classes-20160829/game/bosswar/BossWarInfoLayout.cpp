#include "BossWarInfoLayout.h"
#include "TextHelper.h"
#include "CCColorUtil.h"
#include "vlib/CVRand.h"

#include "SkillCfgMaster.h"

using namespace vmsg;

CBossWarInfoLayout::CBossWarInfoLayout(){
	m_uContentLen = 0;
}

CBossWarInfoLayout::~CBossWarInfoLayout(){

}

bool CBossWarInfoLayout::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/boss_war_info_ui.ExportJson");
		addChild(m_pRootLayout);

		m_pInfoTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("info_txt"));
		m_pInfoTxt->setText("");

		CCColorUtil::addStroke(m_pInfoTxt, 2.0f);
		CCColorUtil::addShadow(m_pInfoTxt);

		bRet = true;
	} while(0);

	return bRet;
}

void CBossWarInfoLayout::updateInfo(const vmsg::CSBossChallengeDamageNotification& stInfo){
	string descStr = "";

	//½øÈë
	CSBossChallengeEnterEvent stEntry;
	for(int i = 0; i < stInfo.playerentry_size(); i ++){
		stEntry = stInfo.playerentry(i);
		
		delPreContent(1);

		char str[50];
		snprintf(str, 50, TextStr(TEXT_WORLD_BOSS_ENTRY_TIP), stEntry.nick().c_str());
		descStr = str;
		descStr += "\n";
		m_stContentMap[descStr] = 1;
	}

	//ÉËº¦
	CSSingleAttendeeDamageEvent stDamage;
	for(int i = 0; i < stInfo.damages_size(); i ++){
		stDamage = stInfo.damages(i);

		delPreContent(2);

		unsigned int uRand = CRandom::GetRandHaveMax(10);
		unsigned int key = TEXT_WORLD_BOSS_DAMAGE_TIP;
		char str[150];
		if(stDamage.skillid() == 0){
			snprintf(str, 150, TextStr(TEXT_WORLD_BOSS_DAMAGE_TIP), stDamage.nick().c_str(), stDamage.damage() + uRand);
		}else{
			string skillName = SKILL_CFG_MASTER->GetSkillCfgByID(stDamage.skillid())->name();
			snprintf(str, 150, TextStr(TEXT_WORLD_BOSS_SKILL_DAMAGE_TIP), stDamage.nick().c_str(), skillName.c_str(), stDamage.damage() + uRand);
		}
		descStr = str;
		descStr += "\n";
		m_stContentMap[descStr] = 2;
	}

	string totalDesc = "";
	map<string, unsigned int>::iterator stIter;
	for(stIter = m_stContentMap.begin(); stIter != m_stContentMap.end(); stIter++){
		totalDesc += stIter->first;
	}

	m_pInfoTxt->setText(totalDesc);
}

void CBossWarInfoLayout::delPreContent(unsigned int uLen){
	unsigned int uCurTotalLen = 0;
	map<string, unsigned int>::iterator stIter;
	for(stIter = m_stContentMap.begin(); stIter != m_stContentMap.end(); stIter++){
		uCurTotalLen += stIter->second;
	}

	unsigned int nowLen = uLen + uCurTotalLen;
	if(nowLen <= 8){
		return;
	}

	if(m_stContentMap.size() <= 0){
		return;
	}

	while(nowLen > 8){
		unsigned int uCurLen = m_stContentMap.begin()->second;
		m_stContentMap.erase(m_stContentMap.begin());
		nowLen = nowLen - uCurLen;
	}
}
