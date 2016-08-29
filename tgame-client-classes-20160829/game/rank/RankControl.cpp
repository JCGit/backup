#include "RankControl.h"
#include "fightendless/FightEndlessControl.h"
#include "arena/ArenaControl.h"
#include "nihility/NihilityControl.h"

CRankControl::CRankControl(){

}

CRankControl::~CRankControl(){

}

CRankPanel* CRankControl::getRankPanel(){
	if( !(m_stRankPanel.isInited()) ) {
		m_stRankPanel.init();
	}

	return &m_stRankPanel;
}

void CRankControl::sendRankQueryRqst(unsigned int rankType, int startPos, int endPos){
	m_stProxy.sendRankQueryRqst(rankType, startPos, endPos);
}

void CRankControl::sendRankQueryMyRankRqst(unsigned int rankType){
	m_stProxy.sendRankQueryMyRankRqst(rankType);
}

void CRankControl::sendQueryAwardRankRqst(){
	m_stProxy.sendQueryAwardRankRqst();
}

void CRankControl::sendFetchRankAwardRqst(unsigned int rankType){
	m_stProxy.sendFetchRankAwardRqst(rankType);
}

void CRankControl::doRankQueryRsp(const CSRankQueryRsp* pstRsp){
	m_stRankInfo = *pstRsp;

	if(pstRsp->ranktype() == RANK_TYPE_NOTHINGLANDSCORE){  //虚无之地排行榜

		if(_isRankPanel){
			if(m_stRankPanel.isInited() && m_stRankPanel.isOpen()){
				m_stRankPanel.updateUI();
			}
		}else{
			P_ENDLESS_CTRL->getEndlessRankPanel()->updateUI();
		}

	}else if(pstRsp->ranktype() == RANK_TYPE_ARENA){  //竞技场排名

	}else{
		if(m_stRankPanel.isInited() && m_stRankPanel.isOpen()){
			m_stRankPanel.updateUI();
		}
	}
}

void CRankControl::doRankQueryMyRankRsp(const CSRankQueryMyRankRsp* pstRsp){
	m_stMyRankInfo = *pstRsp;

	if(pstRsp->ranktype() == RANK_TYPE_NOTHINGLANDSCORE){  //虚无知地排行榜
		if(_isRankPanel){
			if(m_stRankPanel.isInited() && m_stRankPanel.isOpen()){
				m_stRankPanel.updateMyRank();
			}
		}else{
			//P_ENDLESS_CTRL->getEndlessRankPanel()->updateMyRank(pstRsp->rank());
			P_NIHILITY_CTRL->getNihilityPanel()->updateRank(pstRsp->rank() + 1);
		}

	}else if(pstRsp->ranktype() == RANK_TYPE_ARENA){  //竞技场

	}else{  //战力、关卡、部落排行榜
		if(m_stRankPanel.isInited() && m_stRankPanel.isOpen()){
			m_stRankPanel.updateMyRank();
		}
	}
}

void CRankControl::doAwardRankRsp(const CSAwardRankRsp* pstRsp){
	m_stRankAwardInfo = *pstRsp;

	if(m_stRankPanel.isInited() && m_stRankPanel.isOpen()){
		m_stRankPanel.updateGift();
	}
}