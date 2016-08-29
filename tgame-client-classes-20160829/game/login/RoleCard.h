#ifndef RoleCard_h__
#define RoleCard_h__

#include "basic/BaseLayout.h"
#include "CSPlayerMsg.pb.h"

class RoleCard: public BaseLayout
{
public:
	enum CellState {
		CELL_NONE, 
		CELL_NORMAL, 
		CELL_SELECTED
	};
public:
	RoleCard();
	virtual ~RoleCard();

	static RoleCard* create();

	// override start

	virtual bool init();

	virtual std::string getDescription() const;

	// override end

	unsigned int getState();

	void setState(unsigned int state);

	const vmsg::CSPlayerRoleInfo& getRoleInfo();

	void setRoleInfo(const vmsg::CSPlayerRoleInfo &stRoleInfo);

private:
	UIImageView *m_pBgNormalImg;

	UIImageView *m_pBgSelectedImg;

	UIImageView *m_pAddRoleImg;

	UIImageView *m_pRoleHeadImg;

	UILabel *m_pRoleNameTxt;

	UILabel *m_pRoleLevelTxt;

	unsigned int m_uState;

	vmsg::CSPlayerRoleInfo m_stRoleInfo;

};

#endif // RoleCard_h__