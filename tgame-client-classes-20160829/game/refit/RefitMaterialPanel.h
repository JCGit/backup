#ifndef RefitMaterialPanel_h__
#define RefitMaterialPanel_h__

#include "basic/BasePanel.h"
#include "game/guide/GuideControl.h"

class RefitMaterialPanel: public BasePanel, public GuideContainer
{
public:
	RefitMaterialPanel();
	virtual ~RefitMaterialPanel();

	static RefitMaterialPanel* create();

	// override start

	virtual bool init();

	virtual void open();

	virtual CCNode* getGuideNodeByName(const std::string &name);

	// override end

	void openWithItemID(unsigned int uPart, unsigned int uItemID);

	void updateUI();

	void showItem(unsigned int uItemID);

private:
	void initUI();

	void touchEventHandler(CCObject *pSender, TouchEventType type);

	void listViewEventHandler(CCObject *pSender, ListViewEventType type);

private:
	UIButton *m_pCloseBtn;

	UILayout *m_pItemInfoLayout; // 道具信息

	UILabel *m_pItemNameTxt;

	UIImageView *m_pItemImg;

	UILabel *m_pItemDescTxt;

	UILabel *m_pAttr1Txt;

	UILabel *m_pAttr1ValTxt;

	UILabel *m_pAttr2Txt;

	UILabel *m_pAttr2ValTxt;

	UIButton *m_pComposeOrMosaicBtn;

	UILayout *m_pComposeLayout; // 合成

	UIListView *m_pTreeListView;

	UILayout *m_pTreeInfoLayout; // 需要合成的道具

	UIImageView *m_pTreeItemImg;

	UIImageView *m_pTreeLineImg;

	UILayout *m_pNodesLayout;

	UILabel *m_pItemCntTxt;

	UIButton *m_pComposeBtn;

	UILayout *m_pDropInfoLayout; // 直接掉落的道具

	UIListView *m_pDropListView;

	UIButton *m_pBackBtn;

	unsigned int m_uPart;

	unsigned int m_uItemID;

};

#endif // RefitMaterialPanel_h__