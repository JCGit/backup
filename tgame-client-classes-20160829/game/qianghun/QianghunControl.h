#ifndef QianghunControl_h__
#define QianghunControl_h__

#include "QianghunProxy.h"
#include "QianghunPanel.h"

using namespace vmsg;

class CQianghunControl 
{
public:
	CQianghunControl();
	~CQianghunControl();

	void sendQiangHunQueryRqst();
	void sendQiangHunLayerQueryRqst(unsigned int layer);
	void sendQiangHunFlopRqst(unsigned int pos, unsigned int layer);

	void doQianghunInfoRsp(const CSQiangHunInfo& stMsg);
	void doFlopRsp(const CSQiangHunInfo& stMsg);

	const CSQiangHunInfo& getQianghunInfo(){
		return m_stInfo;
	}

	const CSQiangHunElmt& getOpenCardInfo(){
		return m_stOpenInfo;
	}

	CQianghunPanel* getQianghunPanel();

	bool hasFreeTimes();

	void notifyWeaponBook();

protected:
private:
	CQianghunPanel m_stQianghunPanel;
	CQianghunProxy m_stProxy;

	CSQiangHunInfo m_stInfo;
	CSQiangHunElmt m_stOpenInfo;		//翻开牌的信息
};

#define P_QIANGHUN_CTRL CVLib::CVSingleton<CQianghunControl>::Instance()

#endif // QianghunControl_h__
