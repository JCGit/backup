#ifndef GenStrengthenControl_h__
#define GenStrengthenControl_h__

#include "GenStrengthenProxy.h"
#include "GenStrengthenPanel.h"
#include "GenStrengthenEffectPanel.h"

class CGenStrengthenControl
{
public:
	CGenStrengthenControl();
	~CGenStrengthenControl();

	CGenStrengthenPanel* getGenPanel();
	CGenStrengthenEffectPanel* getGenEffPanel();

	//��ѯ����������
	void sendQueryGenStrengthenRqst();	

	//��������
	void sendGenStrengthenRqst(unsigned int uType);

	//��������������ȷ����Ϣ(1��ʾ����)
	void sendGenStrengthenConfirmRqst(unsigned int uIsAccept);


	void doQueryRsp(const vmsg::CSPlayerEnforceMyselfQueryRsp& stRsp);

	void doGenStrengthenRsp(const vmsg::CSPlayerEnforceMyselfRsp& stRsp);

	void doConfirmRsp(const vmsg::CSPlayerEnforceMyselfConfirmRsp& stRsp);

	unsigned int getCostByType(unsigned int uType);

protected:
private:
	CGenStrengthenProxy m_stProxy;

	CGenStrengthenPanel m_stGenPanel;
	CGenStrengthenEffectPanel m_stGenEffPanel;

	CC_SYNTHESIZE(int, m_uAtkAdd, AtkAdd);
	CC_SYNTHESIZE(int, m_uHpAdd, HpAdd);
	CC_SYNTHESIZE(int, m_uDefAdd, DefAdd);

	CC_SYNTHESIZE(unsigned int, m_uGenType, GenType);
	CC_SYNTHESIZE(unsigned int, m_uIsAccept, IsAccept);		//�Ƿ񱣴�
};

#define P_GEN_CTRL CVLib::CVSingleton<CGenStrengthenControl>::Instance()

#endif // GenStrengthenControl_h__
