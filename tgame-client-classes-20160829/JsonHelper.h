#ifndef __JsonHelper_H_
#define __JsonHelper_H_

#include "cocos2d.h"
#include "cocos-ext.h"

template <typename T> 
T GetJsonValue(rapidjson::Value & obj, const char * name);

template <typename T> 
T GetJsonValueDefault(rapidjson::Value & obj, const char * name, const T & d);

class JsonException {
public:
    JsonException(const char * name, const char * reason) {
        mErrMsg.reserve(1024);
        mErrMsg += "Fail to handle field ";
        mErrMsg += name;
        mErrMsg += ", ";
        mErrMsg += reason;
    }
    std::string mErrMsg;
};


#endif //__JsonHelper_H_



