#ifndef __MAIL_OBJ__
#define __MAIL_OBJ__

#include <map>
#include "CSMailMsg.pb.h"
#include "BasicTypes.h"

using namespace vmsg;
using namespace std;

class MailObj
{
public:
	MailObj();
	~MailObj();
    
    void updateMailData(const CSQueryMailRsp& stRsp);
    const map<UNIT64_t, CSMail>& getMailData() {
        return m_stMailMap;
    }
    map<UNIT64_t, CSMail>& getRefMailData() {
        return m_stMailMap;
    }
    const CSMail* getMail(const UNIT64_t uiMailID) {
        if(m_stMailMap.count(uiMailID)) {
            return &(m_stMailMap[uiMailID]);
        } else {
            return NULL;
        }
    }

    const UNIT64_t getGrpID(const UNIT64_t uiMailID) {
        if(m_stGrpMap.count(uiMailID)) {
            return m_stGrpMap[uiMailID];
        } else {
            return 0;
        }
    }

    void delMailID(const UNIT64_t uiMailID);

    void updateMailContent(const CSReadMailRsp& stMailRsp);
    const CSReadMailRsp* getMailContent(const UNIT64_t uiMailID);

	void updateMailState(const UNIT64_t uiMailID);		//�����ʼ�Ϊ�Ѷ�״̬

    void resetData() {
        m_stMailMap.clear();
        m_stGrpMap.clear();
    }

    void setAttachGeted(const UNIT64_t uiMailID);

	unsigned int getUnReadMailNum();			//δ���ʼ�����
private:
    map<UNIT64_t, CSMail> m_stMailMap; //<MailID, CSMail> ���ر�ʱ����
    map<UNIT64_t, UNIT64_t> m_stGrpMap; //<MailID, GrpID> ���ر�ʱ����

    map<UNIT64_t, CSReadMailRsp> m_stMailDataMap; //<MailID, CSReadMailRsp>,�����ʼ����ݣ������

};

#endif // __MAIL_OBJ__
