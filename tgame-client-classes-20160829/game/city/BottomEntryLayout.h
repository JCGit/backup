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

	UIButton* getEntryBtn(unsigned int uEntryID); // ��ȡ��ڰ�ť

	void addEntryBtn(unsigned int uEntryID); // ���һ����ڰ�ť

	void removeEntryBtn(unsigned int uEntryID); // �Ƴ�һ����ڰ�ť

	unsigned int getEntryID(unsigned int uModuleID);

	bool isEntryUnfold();

	void unfoldEntry(); // չ�����

	void foldEntry(); // �۵����

	void updateEntry(); // �������

	void moduleOpen(unsigned int uModuleID);

	CCPoint calcEntryPos(unsigned int uEntryID); // ������ڵ�Ŀ��λ��

	CCPoint getEntryOriginPos();

	void badgeEntryBtn(unsigned int uEntryID, unsigned int uBadgeNum); // BadgeNum����0����ʾ

private:
	void playBtnEff(bool bIsFold);

	void handleBtnEffComplete(CCArmature * armature, MovementEventType type, const char * name);

	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void onUnfoldEntry(float dt); // �����չ��

	void onFoldEntry(float dt); // ������۵�

	void onBtnEffPlayEvent(CCArmature* pArmature, MovementEventType type, const char *pszActionName);

	void onBtnEffFrameEvent(CCBone *pBone, const char *pszFrameName, int a, int b);

	void flyArrived(); // ���ܿ�����Ч�ɵ�Ŀ�ĵ�

	std::string getModuleText(unsigned int uModuleID);

	void onBtnHide(CCNode *pNode);

private:
	UIButton *m_pUnfoldBtn; // չ����ť

	UIButton *m_pFoldBtn; // �۵���ť

	UILayout *m_pHContainerLayout; // �����������

	UILayout *m_pVContainerLayout; // �����������

	bool m_bUnfolded; // �Ƿ��

	std::map<unsigned int, UIButton*> m_stBtnMap; // ������ڰ�ť  <EntryID, EntryBtn*>

	std::vector<unsigned int> m_stHOrderList; // ���к������˳�� <EntryID>

	std::vector<unsigned int> m_stVOrderList; // �����������˳�� <EntryID>

	std::vector<unsigned int> m_stHRenderList; // �ѿ��ź������ <EntryID>

	std::vector<unsigned int> m_stVRenderList; // �ѿ���������� <EntryID>

	UIButton *m_pFuncBtn; // ���ܿ����õ���ʱ��ť

	UILabel *m_pFuncWordsTxt; // ���ܿ��ŵ�����

	UILayout *m_pBgMask; // ���ܿ����õ�����

	CCArmature *m_pBtnEffArmature; // ���ܿ����õ���Ч

	unsigned int m_uNowModuleID; // �ոտ��ŵ�ModuleID

};

#endif // BottomEntryLayout_h__