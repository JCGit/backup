#ifndef FaceBookSDK_h__
#define FaceBookSDK_h__

#include <string>
using namespace std;

class FaceBookSDK
{
public:

	static void onSDKData(std::string stPlayerId, bool bIsGuest, std::string stSig);  //SDK回调
		
	static void onLogin(int iRetCode);		//调用SDK登陆

	static void onBind();					//调用SDK绑定
	static void onShared(std::string name, std::string link, std::string pic, std::string caption, std::string desc);
	static void onFeedBack(std::string extraInfo);

	static void onLogOut();					//调用SDK登出账号(不管Facebook账号登陆还是游客都可以调用)

	static void onFaceBookLogout();			//SDK回调

	static void onFaceBookLogin();			//faceBook账号登陆返回
	static void onGuestLogin();				//游客登陆返回
	static void onFaceBookShared(int iret);			//facebook分享

	static void onFaceBookBlind(std::string stPlayerId, std::string stSig);					//绑定账号返回(针对试玩用户)

	static std::string genLoginJson();		//生成与后台登陆验证的Json

	static bool isSessionValid();			//是否已经登陆过

	static std::string getUserType();		//获取登陆类型(0:游客，2：FB)
	static std::string getPlatformName(){
		return "facebookchannel";
	}

	static bool getIsGuest(){
		return m_bIsGuest;
	}

	static void onPay(int iMoney);					////调用SDK支付

	static void onSDKPayBack(int iRetCode);	//SDK支付回调

	static void onSDKShareBack(int iRetCode);	// SDK分享回调

private:
	static std::string m_stPlayerId;		//玩家ID
	static bool m_bIsGuest;					//是否游客登陆
	static std::string m_stSig;				//用户用于与服务器上用户验证的签名

	static bool m_bIsSessionValid;			//是否已经登陆过
	static std::string m_stUserType;		//登陆类型
};

#endif // FaceBookSDK_h__
