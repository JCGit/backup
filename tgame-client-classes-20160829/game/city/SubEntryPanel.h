#ifndef SubEntryPanel_h__
#define SubEntryPanel_h__

#include "basic/BasePanel.h"
#include "game/guide/GuideControl.h"

class SubEntryPanel: public BasePanel, public GuideContainer
{
public:
	enum SubEntryType {
		SUB_ENTRY_WEAPON, // ǹе
		SUB_ENTRY_GROWTH, // �ɳ�
		SUB_ENTRY_SOCIAL, // �罻
		SUB_ENTRY_CHALLENGE, // ��ս
		SUB_ENTRY_BUSINESS, // ����
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

	UIButton* getEntryBtn(unsigned int uEntryID); // ��ȡ��ڰ�ť

	void addEntryBtn(unsigned int uEntryID); // ���һ����ڰ�ť

	void removeEntryBtn(unsigned int uEntryID); // �Ƴ�һ����ڰ�ť

	void openWithType(SubEntryType type);

	void badgeEntryBtn(unsigned int uEntryID, unsigned int uBadgeNum);

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

private:
	std::vector<unsigned int> m_stWeaponEntryGroup; // ǹе�����

	std::vector<unsigned int> m_stGrowthEntryGroup; // �ɳ������

	std::vector<unsigned int> m_stSocialEntryGroup; // �罻�����

	std::vector<unsigned int> m_stChallengeEntryGroup; // ��ս�����

	std::vector<unsigned int> m_stBusinessEntryGroup; // ���������

	std::map<unsigned int, UIButton*> m_stBtnMap; // ������ڰ�ť  <EntryID, EntryBtn*>

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