#ifndef SubEntryPanel_h__
#define SubEntryPanel_h__

#include "basic/BasePanel.h"
#include "game/guide/GuideControl.h"

class SubEntryPanel: public BasePanel, public GuideContainer
{
public:
	enum SubEntryType {
		SUB_ENTRY_WEAPON, // 枪械
		SUB_ENTRY_GROWTH, // 成长
		SUB_ENTRY_SOCIAL, // 社交
		SUB_ENTRY_CHALLENGE, // 挑战
		SUB_ENTRY_BUSINESS, // 交易
	};

public:
	SubEntryPanel();
	virtual ~SubEntryPanel();

	static SubEntryPanel* create();

	// override start

	virtual bool init();

	// guide
	virtual CCNode* getGuideNodeByName(const std::string &name);

	// override end

	UIButton* getEntryBtn(unsigned int uEntryID); // 获取入口按钮

	void addEntryBtn(unsigned int uEntryID); // 添加一个入口按钮

	void removeEntryBtn(unsigned int uEntryID); // 移除一个入口按钮

	void openWithType(SubEntryType type);

	void badgeEntryBtn(unsigned int uEntryID, unsigned int uBadgeNum);

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	std::vector<unsigned int> m_stWeaponEntryGroup; // 枪械组入口

	std::vector<unsigned int> m_stGrowthEntryGroup; // 成长组入口

	std::vector<unsigned int> m_stSocialEntryGroup; // 社交组入口

	std::vector<unsigned int> m_stChallengeEntryGroup; // 挑战组入口

	std::vector<unsigned int> m_stBusinessEntryGroup; // 交易组入口

	std::map<unsigned int, UIButton*> m_stBtnMap; // 所有入口按钮  <EntryID, EntryBtn*>

	std::vector<unsigned int> m_stSubEntryLst;

	UILayout *m_pSubEntryLayout;

	UIImageView *m_pBgImg;

	UIImageView *m_pTitleWeaponImg;

	UIImageView *m_pTitleGrowthImg;

	UIImageView *m_pTitleSocialImg;

	UIImageView *m_pTitleChallengeImg;

	UIImageView *m_pTitleBusinessImg;

	map<unsigned int, unsigned int> m_stBadgeMap; // <EntryID, BadgeNum>

};

#endif // SubEntryPanel_h__