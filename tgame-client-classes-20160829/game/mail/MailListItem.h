#ifndef MAIL_LIST_ITEM_h__
#define MAIL_LIST_ITEM_h__

#include "basic/BaseLayout.h"
#include "CSMailMsg.pb.h"

class MailListItem : public BaseLayout
{
public:
	MailListItem();
	virtual ~MailListItem();

	static MailListItem* create();

	// override 
	virtual bool init();

	virtual std::string getDescription() const;
	
    void updateMailInfo(const vmsg::CSMail& stMail);
    
    const vmsg::CSMail& getMailInfo() {return m_stMail;}

    void setSelect(bool isSel);


    void onTouched();
private:
    UILabel *m_pTitleTxt;
    UILabel *m_pSenderTxt;
    UILabel *m_pTimeTxt;
    //UILabel *m_pDescTxt;

    UIImageView *m_pMailOpenPic; //
    UIImageView *m_pAttachIconPic; //¸½¼þÍ¼±ê
    
    vmsg::CSMail m_stMail;

};

#endif // MAIL_LIST_ITEM_h__
