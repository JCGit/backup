#ifndef EvolutionControl_h__
#define EvolutionControl_h__

#include "vlib/base/CVSingleton.h"
#include "EvolutionProxy.h"
#include "EvolutionObj.h"
#include "EvolutionPanel.h"

class EvolutionControl
{
public:
	EvolutionControl();
	~EvolutionControl();

	void openEvolutionPanel();

    bool isEvolutionPanelOpen();
	EvolutionPanel* getEvolutionPanel();

	void evolve();

	unsigned int getEvolutionID();

	const std::vector<vmsg::CSEvolutionSkill>& getSkills();

	unsigned int getUsedSkillIdx();

	void notifyEvolution();

	void onRecvMsg(vmsg::VLogicSvr2CliMsg& stMsg);

	void sendEvolutionSkillRqst();

	void sendChangeEvolutionSkillRqst(unsigned int uSkillID);

	void sendQueryEvolutionInfoRqst();

	void sendQueryEvolutionSkillInfoRqst();

    void finalizePanel();

private:
	EvolutionProxy m_stEvolutionProxy;

	EvolutionObj m_stEvolutionObj;

	EvolutionPanel *m_pEvolutionPanel;

};

#define P_EVOLUTION_CTRL CVLib::CVSingleton<EvolutionControl>::Instance()

#endif // EvolutionControl_h__