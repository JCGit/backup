
#include "ChannelInf.h"

void ChannelInf::Init()
{

}

void ChannelInf::Login()
{
}


std::string ChannelInf::GetChannelName()
{ 
    return ""; 
}

int ChannelInf::Buy(const std::string & orderId, 
        const std::string & uidInGame, 
        const std::string & userNameInGame, 
        const std::string & serverId, 
        const std::string & productName, 
        const std::string & productId,
        const std::string & payInfo,
        int productCount,
        int realPayMoney) 
{
    
    return 0;
}

std::string ChannelInf::GetPayToken() {

    return "";
}
std::string ChannelInf::GetSDKUid() {
    return "";
}



bool ChannelInf::submitPlayerInfo(const std::string & roleId, 
                      const std::string & roleName, 
                      const std::string & roleLevel, 
                      int zoneId,
                      const std::string & zoneName)
{
    return true;
}

void ChannelInf::Logout() {

}

void ChannelInf::onLoginRsp(const std::string & strInfo) {

}

void ChannelInf::showToolbar(bool isVisible) {

}

void ChannelInf::doRunProtocol(int id, const std::string & protocol, const std::string & message) {

}

bool ChannelInf::isSupportSwtichAccount(){
	return true;
}

int ChannelInf::switchAccount(int id){
	return 0;
}
