#ifndef CreateRolePanel_h__
#define CreateRolePanel_h__

#include "basic/BasePanel.h"

class UFOCell;

class CreateRolePanel: public BasePanel
{
public:
	CreateRolePanel();
	virtual ~CreateRolePanel();

	static CreateRolePanel* create();

	// override start

	virtual bool init();

	virtual void open();

	virtual void close();

	// override end

	void initUI();

	void prevRole();

	void nextRole();

	void showTip();

	void rotateEllipseTrack(bool clockwise); // �����Ⱥ�ʱ�뷽����ת��Բ���

private:
	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void scrollViewEventHandler(CCObject *pSender, ScrollviewEventType type);

	void textFieldEventHandler(CCObject *pSender, TextFiledEventType type);

	void checkName(std::string nameStr);

	void pushAircraft(UFOCell *pAircraft); // m_stAircrafts

	UFOCell* popAircraft(); // m_stAircrafts

	void removeCellFromScroll(CCNode *pUFOCell);

	void onSelected(CCNode *pNode);

	void onUnSelected(CCNode *pNode);

    void doAfterMovieDone();

	void onRotated();

private:
	CCArmature *m_pBgArmture;

	CCArmature *m_pFrontBgArmture;

	UIImageView *m_pBgImg;

	UIImageView *m_pLightingMaskImg;

	UIImageView *m_pRoleLightingImg;

	UIScrollView *m_pRolesScrollView;

	UILabel *m_pRoleNameTxt;

	UILabel *m_pRoleIntroTxt;

	UITextField *m_pRoleNameInputTxt;

	UIButton *m_pDiceBtn;

	UIButton *m_pCreateBtn;

	UIButton *m_pLeftFlipBtn;

	UIButton *m_pRightFlipBtn;

	UILabel *m_pTipTxt;

	int m_nRoleSelectedIdx;

	int m_nScrollDirection;

	vector<UFOCell*> m_stAircrafts; // ��Ҫ�ķ�����

	CCParticleSystemQuad *m_pScreenParticleEff;

	CCParticleBatchNode *m_pParticleBatchNode;

	list<UFOCell*> m_stAircraftShowLst; // չʾ�ķ�����

};

#endif // CreateRolePanel_h__