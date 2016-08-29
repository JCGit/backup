#ifndef BottomEntryLayout_h__
#define BottomEntryLayout_h__

#include "basic/BaseLayout.h"

#include "ModuleID.pb.h"

class BottomEntryLayout: public BaseLayout
{
public:
	BottomEntryLayout();
	virtual ~BottomEntryLayout();

	static BottomEntryLayout* create(UILayout *pRefLayout);

	bool initWithLayout(UILayout *pRefLayout);

	UIButton* getEntryBtn(unsigned int uEntryID); // 获取入口按钮

	void addEntryBtn(unsigned int uEntryID); // 添加一个入口按钮

	void removeEntryBtn(unsigned int uEntryID); // 移除一个入口按钮

	unsigned int getEntryID(unsigned int uModuleID);

	bool isEntryUnfold();

	void unfoldEntry(); // 展开入口

	void foldEntry(); // 折叠入口

	void updateEntry(); // 更新入口

	void moduleOpen(unsigned int uModuleID);

	CCPoint calcEntryPos(unsigned int uEntryID); // 计算入口的目标位置

	CCPoint getEntryOriginPos();

	void badgeEntryBtn(unsigned int uEntryID, unsigned int uBadgeNum); // BadgeNum大于0才显示

private:
	void playBtnEff(bool bIsFold);

	void handleBtnEffComplete(CCArmature * armature, MovementEventType type, const char * name);

	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void onUnfoldEntry(float dt); // 入口已展开

	void onFoldEntry(float dt); // 入口已折叠

	void onBtnEffPlayEvent(CCArmature* pArmature, MovementEventType type, const char *pszActionName);

	void onBtnEffFrameEvent(CCBone *pBone, const char *pszFrameName, int a, int b);

	void flyArrived(); // 功能开放特效飞到目的点

	std::string getModuleText(unsigned int uModuleID);

	void onBtnHide(CCNode *pNode);

private:
	UIButton *m_pUnfoldBtn; // 展开按钮

	UIButton *m_pFoldBtn; // 折叠按钮

	UILayout *m_pHContainerLayout; // 横向入口容器

	UILayout *m_pVContainerLayout; // 纵向入口容器

	bool m_bUnfolded; // 是否打开

	std::map<unsigned int, UIButton*> m_stBtnMap; // 所有入口按钮  <EntryID, EntryBtn*>

	std::vector<unsigned int> m_stHOrderList; // 所有横向入口顺序 <EntryID>

	std::vector<unsigned int> m_stVOrderList; // 所有纵向入口顺序 <EntryID>

	std::vector<unsigned int> m_stHRenderList; // 已开放横向入口 <EntryID>

	std::vector<unsigned int> m_stVRenderList; // 已开放纵向入口 <EntryID>

	UIButton *m_pFuncBtn; // 功能开放用的临时按钮

	UILabel *m_pFuncWordsTxt; // 功能开放的描述

	UILayout *m_pBgMask; // 功能开放用的遮罩

	CCArmature *m_pBtnEffArmature; // 功能开放用的特效

	unsigned int m_uNowModuleID; // 刚刚开放的ModuleID

};

#endif // BottomEntryLayout_h__