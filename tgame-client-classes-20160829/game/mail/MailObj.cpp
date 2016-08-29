#include "MailObj.h"


MailObj::MailObj()
{
    m_stMailMap.clear();
}

MailObj::~MailObj()
{
}


void MailObj::updateMailData(const CSQueryMailRsp& stRsp){
    
    m_stMailMap.clear();
    m_stGrpMap.clear();

    for(int i = 0; i < stRsp.selfmaillst_size(); ++i) {
        m_stMailMap[stRsp.selfmaillst(i).mailid()] = stRsp.selfmaillst(i);
    }

    for(int i = 0; i < stRsp.mailbox_size(); ++i) {
        const CSMailBox& stMailBox = stRsp.mailbox(i);

        const UNIT64_t uiGrpID = stMailBox.groupid();

        for(int iIdx = 0; iIdx < stMailBox.mailsep_size(); ++iIdx) {
            const CSMailSep& stMailSep = stMailBox.mailsep(iIdx);

            CSMail stMail;
            stMail.Clear();
            stMail.set_mailid(stMailSep.mailid());
            stMail.set_hadread(stMailSep.hadread());
            stMail.set_hadgetattach(stMailSep.hadgetattach());
            
            m_stMailMap[stMailSep.mailid()] = stMail;
            
            m_stGrpMap[stMailSep.mailid()] = uiGrpID; //set group id
        } //end for iIdx

        for(int iIdx = 0; iIdx < stMailBox.grpmaillst_size(); ++iIdx) {
            const CSGrpMailBreif& stMailBreif = stMailBox.grpmaillst(iIdx);
            if(m_stMailMap.count(stMailBreif.mailid()) > 0) {
                CSMail& stMail = m_stMailMap[stMailBreif.mailid()];
                stMail.set_sender(stMailBreif.sender());
                stMail.set_mailhead(stMailBreif.mailhead());
                stMail.set_remaintime(stMailBreif.remaintime());
                stMail.set_hasattach(stMailBreif.hasattach());
                stMail.set_timestamp(stMailBreif.timestamp());

            }

        } //end for iIdx

    } //end for i

}


void MailObj::delMailID(const UNIT64_t uiMailID){
    m_stMailMap.erase(uiMailID);
    m_stGrpMap.erase(uiMailID);
    m_stMailDataMap.erase(uiMailID);    
}


void MailObj::updateMailContent(const CSReadMailRsp& stMailRsp){
    m_stMailDataMap[stMailRsp.mailid()] = stMailRsp;

	updateMailState(stMailRsp.mailid());
}

void MailObj::updateMailState(const UNIT64_t uiMailID){
	if(m_stMailMap.count(uiMailID)) {
		m_stMailMap[uiMailID].set_hadread(1);
	}
}

const CSReadMailRsp* MailObj::getMailContent(const UNIT64_t uiMailID){
    if(m_stMailDataMap.count(uiMailID)) {
        return &(m_stMailDataMap[uiMailID]);
    }
    return NULL;
}



void MailObj::setAttachGeted(const UNIT64_t uiMailID){
    if(m_stMailMap.count(uiMailID)) {
        m_stMailMap[uiMailID].set_hadgetattach(true);
    }
}

unsigned int MailObj::getUnReadMailNum(){
	unsigned int count = 0;

	map<UNIT64_t, CSMail>::iterator stIter;
	for(stIter = m_stMailMap.begin(); stIter != m_stMailMap.end(); stIter++){
		if(0 == stIter->second.hadread() && stIter->second.hadgetattach() == 0 ){   
            //Œ¥∂¡” º˛£¨«“Œ¥¡Ï»°
			count++;
		}
	}

	return count;
}

