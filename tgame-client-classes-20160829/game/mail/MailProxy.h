#ifndef __MAIL_PROXY__
#define __MAIL_PROXY__

#include "QBaseProxy.h"
#include "BasicTypes.h"

class MailProxy: public QBaseProxy
{
public:
	MailProxy();
	virtual ~MailProxy();

	// override rsp handler
	virtual void DoRsp(vmsg::VLogicSvr2CliMsg &stMsg);


    void sendQueryMail();
    void sendReadMail(const UNIT64_t uMailID);
    void sendDelMail(const UNIT64_t uMailID);
    void sendGetAttachOne(const UNIT64_t uMailID);
    void sendGetAttachAll(const std::vector<UNIT64_t>& stMailLst);
    void sendAutoDelRqst(); //һ��ɾ��

private:
    UNIT64_t m_uLastGetAttachMailID; //��¼�ϴ���ȡ�������ʼ����ذ�ʱƮ��
};

#endif // __MAIL_PROXY__
