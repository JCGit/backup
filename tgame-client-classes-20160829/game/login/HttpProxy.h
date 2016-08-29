#ifndef HttpProxy_h__
#define HttpProxy_h__

#include "vlib/base/CVSingleton.h"
#include <string>

class HttpProxy
{
public:
	HttpProxy();
	~HttpProxy();
    
	void sendGetDirInfoRqst(const std::string &strURL);

    //source code: http://blog.csdn.net/huyiyang2010/article/details/7664201
    /** 
    * @brief HTTP GET请求 
    * @param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com 
    * @param strResponse 输出参数,返回的内容 
    * @return 返回是否Post成功 
    */  
    int HttpGet(const std::string & strUrl, std::string & strResponse);

    /** 
    * @brief HTTP POST请求 
    * @param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com 
    * @param strPost 输入参数,使用如下格式para1=val1=val2&… 
    * @param strResponse 输出参数,返回的内容 
    * @return 返回是否Post成功 
    */  
    int HttpPost(const std::string & strUrl, const std::string & strPost, std::string & strResponse);
    bool bIsDoCurlOK(const int iRetCode);
private:
    void parseDirInfoRsp(const std::string& strRspJson);
    bool m_bIsCurlGlobalInited;
};

#define P_HTTP_PROXY CVLib::CVSingleton<HttpProxy>::Instance()

#endif // HttpProxy_h__
