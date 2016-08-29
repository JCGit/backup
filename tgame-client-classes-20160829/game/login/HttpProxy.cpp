#include "HttpProxy.h"

#include "LoginObj.h"
#include "LoginControl.h"
#include "defines.h"
#include "GameInf.h"
#include "evtlog/EvtLog.h"
#include "curl/curl.h"


static size_t OnWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid)  
{  
    std::string* str = dynamic_cast<std::string*>((std::string *)lpVoid);  
    if( NULL == str || NULL == buffer )  
    {  
        return -1;  
    }  

    char* pData = (char*)buffer;  
    str->append(pData, size * nmemb);  
    return nmemb;  
} 

HttpProxy::HttpProxy()
{
    m_bIsCurlGlobalInited = false;
}

HttpProxy::~HttpProxy()
{
}

void HttpProxy::parseDirInfoRsp(const std::string& strRspJson) {

    rapidjson::Document jsonDoc;
    jsonDoc.Parse<0>(strRspJson.c_str());
    if(jsonDoc.HasParseError()) {
        P_EVT_LOG->log(EVT_LOG_GET_SVR_LIST, "JsonError");
        CCLOG("HttpProxy::parseSvrListRsp,json parse error [%s]", jsonDoc.GetParseError());
        return;
    }

    P_EVT_LOG->log(EVT_LOG_GET_SVR_LIST, "success");
    rapidjson::Value &jsonElmts = jsonDoc["zones"];
    if(!(jsonElmts.IsArray() && jsonElmts.Size() > 0)) {
        CCLOG("HttpProxy::parseSvrListRsp, invalid json format");
        return;
    }
    std::vector<SvrInfo> svrInfos;

    for(unsigned int idx = 0; idx < jsonElmts.Size(); idx++) {
        rapidjson::Value &jsonElmt = jsonElmts[idx];
        SvrInfo stSvrInfo;

        if(jsonElmt.HasMember("zoneid") && jsonElmt["zoneid"].IsString()) {
            stSvrInfo.zoneid = std::atoi(jsonElmt["zoneid"].GetString());
        }

        if(jsonElmt.HasMember("name") && jsonElmt["name"].IsString()) {
            stSvrInfo.name = jsonElmt["name"].GetString();
        }

        if(jsonElmt.HasMember("ip") && jsonElmt["ip"].IsString()) {
            stSvrInfo.ip = jsonElmt["ip"].GetString();
        }

        if(jsonElmt.HasMember("status") && jsonElmt["status"].IsString()) {
            stSvrInfo.status = jsonElmt["status"].GetString();
        }

		stSvrInfo.iFlag = std::atoi(stSvrInfo.status.substr(0, 1).c_str());


// 		if (jsonElmt.HasMember("flag") && jsonElmt["flag"].IsString()) {
// 			stSvrInfo.iFlag = std::atoi(jsonElmt["flag"].GetString());
// 		}
// 		else{
// 			stSvrInfo.iFlag = 1;
// 		}

		if (jsonElmt.HasMember("date") && jsonElmt["date"].IsString()) {
			stSvrInfo.szTime = jsonElmt["date"].GetString();

			std::string::size_type pos(0);
			if ((pos = stSvrInfo.szTime.find("T")) != std::string::npos)
				stSvrInfo.szTime.replace(pos, 1, " ");
			stSvrInfo.szTime += ":00";
		}
		else{
			stSvrInfo.szTime = "";
		}

        svrInfos.push_back(stSvrInfo);
    }

    std::vector<SvrInfo> &svrInfoList = P_LOGIN_OBJ->getSvrInfoList();
    svrInfoList.clear();
    std::vector<SvrInfo>::const_reverse_iterator iter;

    for(iter = svrInfos.rbegin(); iter != svrInfos.rend(); iter++) {
        svrInfoList.push_back(*iter);
    }

	// other dir info
	DirSvrInfo stDirSvrInfo;

	if(jsonDoc.HasMember("cdn") && jsonDoc["cdn"].IsString()) {
		stDirSvrInfo.cdn = jsonDoc["cdn"].GetString();
	} else {
		stDirSvrInfo.cdn = "";
	}

	if(jsonDoc.HasMember("notification") && jsonDoc["notification"].IsString()) {
		stDirSvrInfo.notification = jsonDoc["notification"].GetString();
	} else {
		stDirSvrInfo.notification = "";
	}

	if(jsonDoc.HasMember("isstop") && jsonDoc["isstop"].IsString()) {
		stDirSvrInfo.isstop = std::atoi(jsonDoc["isstop"].GetString()) == 1;
	} else {
		stDirSvrInfo.isstop = false;
	}

	if(jsonDoc.HasMember("whitelist") && jsonDoc["whitelist"].IsString()) {
		stDirSvrInfo.whitelist = jsonDoc["whitelist"].GetString();
	} else {
		stDirSvrInfo.whitelist = "";
	}

	P_LOGIN_OBJ->setDirSvrInfo(stDirSvrInfo);

    P_LOGIN_CTRL->getLoginScreenView()->updateUI();
    P_LOGIN_CTRL->getChooseSvrPanel()->updateSvrList();
}

void HttpProxy::sendGetDirInfoRqst(const std::string &strURL) {
    CCLOG("[trace]HttpProxy::sendGetSvrListRqst,get svr list url %s", strURL.c_str());

    std::string strRsp;
    int iRet = HttpGet(strURL, strRsp);
    if (iRet != CURLM_OK) {
        CCLOG("[trace]HttpProxy::sendGetSvrListRqst, fail to do curl,url %s, ret %d", strURL.c_str(), iRet);
        //连接失败
        P_GAME_INF->showHttpFail();

        string strError;
        strError += "_fail" + intToString(iRet);
        P_EVT_LOG->log(EVT_LOG_GET_SVR_LIST, strError);
        return;
    }
    parseDirInfoRsp(strRsp);
}

bool HttpProxy::bIsDoCurlOK(const int iRetCode) {
    return (iRetCode == CURLM_OK);
}

int HttpProxy::HttpGet(const std::string & strUrl, std::string & strResponse)
{
    CURLcode res;
    if(!m_bIsCurlGlobalInited) {
        curl_global_init(CURL_GLOBAL_ALL);   //global init just once,neilwu comment
        m_bIsCurlGlobalInited = true;
    }
    
	CURL* curl = curl_easy_init();
	if(NULL == curl)
	{
		return CURLE_FAILED_INIT;
	}

    curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
	/**
	* 当多个线程都使用超时处理的时候，同时主线程中有sleep或是wait等操作。
	* 如果不设置这个选项，libcurl将会发信号打断这个wait从而导致程序退出。
	*/
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10); //10s超时
	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	return res;
}

int HttpProxy::HttpPost(const std::string & strUrl, const std::string & strPost, std::string & strResponse)
{
    CURLcode res;
    if(!m_bIsCurlGlobalInited) {
        curl_global_init(CURL_GLOBAL_ALL);   //global init just once,neilwu comment
        m_bIsCurlGlobalInited = true;
    }
    CURL* curl = curl_easy_init();
    if(NULL == curl)
    {
        return CURLE_FAILED_INIT;
    }

    curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPost.c_str());
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);//10s超时
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return res;
}