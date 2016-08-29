#include "MemberListItem.h"

#include "defines.h"
#include "basic/NounHelper.h"

MemberListItem::MemberListItem()
:m_pNameTxt(NULL)
,m_pLevelTxt(NULL)
,m_pJobTxt(NULL)
,m_pOnlineTxt(NULL)
,m_uin(0)
{
}

MemberListItem::~MemberListItem()
{
}

MemberListItem* MemberListItem::create() {
	MemberListItem *pRet = new MemberListItem();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool MemberListItem::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui/MemberListItemUI.json");
		addChild(m_pRootLayout);

		m_pNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("name_txt"));

		m_pLevelTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("level_txt"));

		m_pJobTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("job_txt"));

		m_pOnlineTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("online_txt"));

		bRet = true;
	} while(0);

	return bRet;
}

void MemberListItem::updateUI(const vmsg::CSGuildMember &stMember) {
	m_uin = stMember.brief().uin();
	m_pNameTxt->setText(stMember.brief().name());
	m_pLevelTxt->setText(intToString(stMember.brief().level()));
	
	if(stMember.brief().type() == vmsg::GUILD_PRIVI_CHAIRMAN) {
		m_pJobTxt->setText(NounStr(NOUN_CHAIRMAN));
	} else if(stMember.brief().type() == vmsg::GUILD_PRIVI_DEPUTY) {
		m_pJobTxt->setText(NounStr(NOUN_DEPUTY));
	} else if(stMember.brief().type() == vmsg::GUILD_PRIVI_MEMBER) {
		m_pJobTxt->setText(NounStr(NOUN_MEMBER));
	}

	if(stMember.isonline() == 0) {
		m_pOnlineTxt->setText(NounStr(NOUN_OFFLINE));
	} else {
		m_pOnlineTxt->setText(NounStr(NOUN_ONLINE));
	}

	
}

UIN_t MemberListItem::getMemberUIN() {
	return m_uin;
}
