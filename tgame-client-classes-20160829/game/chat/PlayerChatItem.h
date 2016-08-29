#ifndef PlayerChatItem_h__
#define PlayerChatItem_h__

#include "BaseLayout.h"
#include "basic/BasicTypes.h"

class PlayerChatItem: public BaseLayout
{
public:
	PlayerChatItem();
	virtual ~PlayerChatItem();
	
	static PlayerChatItem* create(UILayout *pRefLayout);

	// override start

	virtual bool init();

	// override end

	bool initWithLayout(UILayout *pRefLayout);

	bool isSelected();

	void selected(bool bSelected);

	void updateUI(UIN_t uin, const std::string &nick, unsigned int showID);

	UIN_t getPlayerUin();

private:
	UIImageView *m_pBgSelectedImg;

	UIImageView *m_pHeadImg;

	UILabel *m_pNameTxt;

	bool m_bSelected;

	UIN_t m_uin;

};

#endif // PlayerChatItem_h__