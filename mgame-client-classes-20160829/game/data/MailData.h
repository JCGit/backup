#pragma once

#include "Singleton.h"
#include "GameData.h"
#include "cocos2d.h"
#include "net/proto/MailData.pb.h"
#include "net/proto/ClientSvrMsg.pb.h"
#include "Constant.h"
USING_NS_CC;


class MailData
{
public:

	MailData();
	~MailData(); 
	
	void initMail();

	void saveToFile();

	void addNewMail(const fgame::CSGetMailResp&);//������ʼ�

	void delMail(std::vector<int>);

	//�Ƿ������ʼ�
	bool isNewMail();
	void setNewMail(bool);

	int getMailSize();

	const fgame::DataMailInfo& getMailInfo(int order);
	const fgame::DataMailInfo* getMailInfoById(int id);

	int getMailNo();
	void setMailNo(int no);

	std::map<int,int> getAward(std::string); //<type, number>

	fgame::DataMailUin& getDataMailUin();

	int getGoodsType(int id);

	std::map<std::string, int> mRealPorp;
protected:

	fgame::DataMail mDataMail;

private:
	void loadMailData(std::string dbPath);

	void initMailData();

	void saveMailData();

	void loadDataByPb();

	void removeOldMail();
};

#define sgMailData()		Singleton<MailData>::Get()
