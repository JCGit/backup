#ifndef AttrStrengthenControl_h__
#define AttrStrengthenControl_h__

#include "vlib/base/CVSingleton.h"

#include "AttrStrengthenProxy.h"
#include "AttrStrengthenObj.h"

#include "AttrStrengthenInfoPanel.h"
#include "AttrStrengthenPanel.h"
#include "CoinTipPanel.h"

class AttrStrengthenControl 
{
public:
	AttrStrengthenControl();
	~AttrStrengthenControl();

	const vmsg::CSAttrStrengthenInfo& getAttrStrengthenInfo();

	void openAttrStrengthenInfoPanel();

	AttrStrengthenInfoPanel* getAttrStrengthenInfoPanel();

	AttrStrengthenPanel* getAttrStrengthenPanel();

	CoinTipPanel* getCoinTipPanel();

	void onRecvMsg(vmsg::VLogicSvr2CliMsg &stMsg);

	void sendQueryAttrStrengthenInfoRqst(); // 查询属性强化

	void sendAttrStrengthenRqst(unsigned int uType, unsigned int uMagicType); // 强化属性

private:
	AttrStrengthenProxy m_stAttrStrengthenProxy;

	AttrStrengthenObj m_stAttrStrengthenObj;

	AttrStrengthenInfoPanel *m_pAttrStrengthenInfoPanel;

	AttrStrengthenPanel *m_pAttrStrengthenPanel;

	CoinTipPanel *m_pCoinTipPanel;

};

#define P_ATTR_STRENGTHEN_CTRL CVLib::CVSingleton<AttrStrengthenControl>::Instance()

#endif // AttrStrengthenControl_h__