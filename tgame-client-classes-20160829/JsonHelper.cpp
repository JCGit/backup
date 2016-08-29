#include "JsonHelper.h"


template <> 
int GetJsonValue<int>(rapidjson::Value & obj, const char * name) {
    rapidjson::Value & m = obj[name];
    if (!m.IsInt()) {
        throw JsonException(name, "not int");
    }
    return m.GetInt();
}

template <> 
const char * GetJsonValue<const char*>(rapidjson::Value & obj, const char * name) {
    rapidjson::Value &  m = obj[name];
    //CCLog("get name %s", name);
    if (!m.IsString()) {
        throw JsonException(name, "not string");
    }
    return m.GetString();
}

template <> 
rapidjson::Value & GetJsonValue<rapidjson::Value&>(rapidjson::Value & obj, const char * name) {
    rapidjson::Value &  m = obj[name];
    if (!m.IsObject() && !m.IsArray()) {
        throw JsonException(name, "not object");
    }
    return m;
}

template <> 
int GetJsonValueDefault<int>(rapidjson::Value & obj, const char * name, const int & d) {
    rapidjson::Value & m = obj[name];
    if (m.IsNull()) {
        return d;
    }
    if (!m.IsInt()) {
        throw JsonException(name, "not int");
    }
    return m.GetInt();
}

template <> 
const char * GetJsonValueDefault<const char*>(rapidjson::Value & obj, 
                                              const char * name,
                                              const char * const & d) {
    rapidjson::Value & m = obj[name];
    if (m.IsNull()) {
        return d;
    }
    if (!m.IsString()) {
        throw JsonException(name, "not string");
    }
    return m.GetString();
}

