#ifndef  _QENV_SET_
#define  _QENV_SET_

#include <string>

#define TGAME_DEV_TEST 1        //打开全部功能入口
#define TGAME_SHOW_ANYENTRY 1   //打开AnyEntry 按钮
//#define TGAME_IN_APP_SDK 1    //打开sdk
//#define TGAME_USE_AUTO_UPDATE 1 //打开在线更新
//#define TGAME_SHOW_FPS  1       //显示帧率

static std::string gGlobalOpenID("test0111"); //  1011

class QEnvSet
{
public:
    static bool neil(){return false;}
    static bool taddy(){return false;}
    static bool tod(){return false;}
    static bool beck(){return false;} //
    static bool linc(){return false;}
	static bool jacey(){ return true; }


     //neil
    //static const char* GetOpenID() {return m_stOpenID.c_str();}
    static const char* GetIP(){return "192.168.2.34";} 

    //static const char* GetDirURL() {return "118.192.76.46:8080";} //外网

	//static const char* GetDirURL() {return "118.192.89.18:8080";} //新外网
    
    //static const char* GetDirURL() {return "192.168.2.32:8080";} //内网

	static const char* GetResUrl(){ return "http://192.168.2.36:8081/"; }

    static int GetPort(){return 9000;}
    static int GetZoneID() {return 2;}

    
    /*
    static const char* GetOpenID() {return "111";}
    static const char* GetIP(){return "192.168.2.32";}
    static const char* GetDirURL() {return "192.168.2.32:8080";}
    static int GetPort(){return 9000;}
    static int GetZoneID() {return 1;}
    */
public:
};


#endif // _QENV_SET_



