#ifndef MemberListItem_h__
#define MemberListItem_h__

#include "basic/BaseLayout.h"
#include "CSGuildMsg.pb.h"
#include "basic/BasicTypes.h"

class MemberListItem: public BaseLayout
{
public:
	MemberListItem();
	virtual ~MemberListItem();
	
	static MemberListItem* create();

	// override start

	virtual bool init();

	// override end

	void updateUI(const vmsg::CSGuildMember &stMember);

	UIN_t getMemberUIN();

private:
	UILabel *m_pNameTxt;

	UILabel *m_pLevelTxt;

	UILabel *m_pJobTxt;

	UILabel *m_pOnlineTxt;

	UIN_t m_uin;

};

#endif // MemberListItem_h__