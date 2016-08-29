// 
//  LuaInf.h 
//  CppLua 
// 
//  Created by Himi on 13-4-17. 
// 
// 
  
#ifndef __lua_inf__ 
#define __lua_inf__ 
  
#include "cocos2d.h" 
using namespace cocos2d; 
using namespace std; 
  
extern "C" { 
#include "lua.h" 
#include "lualib.h" 
#include "lauxlib.h" 
}; 
  
class LuaInf{ 
public: 
    static LuaInf* sharedHD(); 
  
        //------------  c++ -> lua ------------// 
  
    /* 
        getLuaVarString : ����luaȫ��string 
  
        luaFileName  = lua�ļ��� 
        varName = ��ҪȡLua�еı����� 
     */ 
    const char* getLuaVarString(const char* luaFileName,const char* varName); 
  
    /* 
     getLuaVarOneOfTable : ����luaȫ��table�е�һ��Ԫ�� 
  
     luaFileName  = lua�ļ��� 
     varName = ��ҪȡLua�е�table������ 
     keyName = ��ҪȡLua�е�table��ĳһ��Ԫ�ص�Key 
     */ 
    const char* getLuaVarOneOfTable(const char* luaFileName,const char* varName,const char* keyName); 
  
    /* 
     getLuaVarTable : ����luaȫ��table 
  
     luaFileName  = lua�ļ��� 
     varName = ��Ҫȡ��table������ 
  
     ��ע�����ص������е����ݣ�ͯЬ�ǿ����Լ�ʹ��Map�ȴ��� 
     */ 
    const char* getLuaVarTable(const char* luaFileName,const char* varName); 
  
    /* 
     callLuaFunction : ����lua���� 
  
     luaFileName  = lua�ļ��� 
     functionName = ��Ҫ����Lua�еĵĺ����� 
     */ 
    const char* callLuaFunction(const char* luaFileName,const char* functionName); 
  
      //------------  lua -> c++ ------------// 
  
    void callCppFunction(const char* luaFileName); 
  
private: 
    static int cppFunction(lua_State* ls); 
  
    static bool _isFirst; 
    static LuaInf* _shared; 
    const char* getFileFullPath(const char* fileName); 
    ~LuaInf(); 
}; 
  
#endif /* defined(__CppLua__LuaInf__) */ 