#ifndef FaceBookSDK_h__
#define FaceBookSDK_h__

#include <string>
using namespace std;

class FaceBookSDK
{
public:

	static void onSDKData(std::string stPlayerId, bool bIsGuest, std::string stSig);  //SDK�ص�
		
	static void onLogin(int iRetCode);		//����SDK��½

	static void onBind();					//����SDK��
	static void onShared(std::string name, std::string link, std::string pic, std::string caption, std::string desc);
	static void onFeedBack(std::string extraInfo);

	static void onLogOut();					//����SDK�ǳ��˺�(����Facebook�˺ŵ�½�����οͶ����Ե���)

	static void onFaceBookLogout();			//SDK�ص�

	static void onFaceBookLogin();			//faceBook�˺ŵ�½����
	static void onGuestLogin();				//�ο͵�½����
	static void onFaceBookShared(int iret);			//facebook����

	static void onFaceBookBlind(std::string stPlayerId, std::string stSig);					//���˺ŷ���(��������û�)

	static std::string genLoginJson();		//�������̨��½��֤��Json

	static bool isSessionValid();			//�Ƿ��Ѿ���½��

	static std::string getUserType();		//��ȡ��½����(0:�οͣ�2��FB)
	static std::string getPlatformName(){
		return "facebookchannel";
	}

	static bool getIsGuest(){
		return m_bIsGuest;
	}

	static void onPay(int iMoney);					////����SDK֧��

	static void onSDKPayBack(int iRetCode);	//SDK֧���ص�

	static void onSDKShareBack(int iRetCode);	// SDK����ص�

private:
	static std::string m_stPlayerId;		//���ID
	static bool m_bIsGuest;					//�Ƿ��ο͵�½
	static std::string m_stSig;				//�û���������������û���֤��ǩ��

	static bool m_bIsSessionValid;			//�Ƿ��Ѿ���½��
	static std::string m_stUserType;		//��½����
};

#endif // FaceBookSDK_h__
