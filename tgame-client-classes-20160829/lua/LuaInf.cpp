// 
//  LuaInf.cpp 
//  CppLua 
// 
//  Created by Himi on 13-4-17. 
// 
// 
  
#include "LuaInf.h" 
#include "CCLuaEngine.h" 
  
bool LuaInf::_isFirst; 
LuaInf* LuaInf::_shared; 
  
LuaInf* LuaInf::sharedHD(){ 
    if(!_isFirst){ 
        _shared = new LuaInf(); 
    } 
    return _shared; 
} 
  
const char* LuaInf::getLuaVarString(const char* luaFileName,const char* varName){ 
  
    lua_State*  ls = CCLuaEngine::defaultEngine()->getLuaStack()->getLuaState(); 
  
    int isOpen = luaL_dofile(ls, getFileFullPath(luaFileName)); 
    if(isOpen!=0){ 
        CCLOG("Open Lua Error: %i", isOpen); 
        return NULL; 
    } 
  
    lua_settop(ls, 0); 
    lua_getglobal(ls, varName); 
  
    int statesCode = lua_isstring(ls, 1); 
    if(statesCode!=1){ 
        CCLOG("Open Lua Error: %i", statesCode); 
        return NULL; 
    } 
  
    const char* str = lua_tostring(ls, 1); 
    lua_pop(ls, 1); 
  
    return str; 
} 
  
const char* LuaInf::getLuaVarOneOfTable(const char* luaFileName,const char* varName,const char* keyName){ 
  
    lua_State*  ls = CCLuaEngine::defaultEngine()->getLuaStack()->getLuaState(); 
  
    int isOpen = luaL_dofile(ls, getFileFullPath(luaFileName)); 
    if(isOpen!=0){ 
        CCLOG("Open Lua Error: %i", isOpen); 
        return NULL; 
    } 
  
    lua_getglobal(ls, varName); 
  
    int statesCode = lua_istable(ls, -1); 
    if(statesCode!=1){ 
        CCLOG("Open Lua Error: %i", statesCode); 
        return NULL; 
    } 
  
    lua_pushstring(ls, keyName); 
    lua_gettable(ls, -2); 
    const char* valueString = lua_tostring(ls, -1); 
  
    lua_pop(ls, -1); 
  
    return valueString; 
} 
  
const char* LuaInf::getLuaVarTable(const char* luaFileName,const char* varName){ 
    lua_State*  ls = CCLuaEngine::defaultEngine()->getLuaStack()->getLuaState(); 
  
    int isOpen = luaL_dofile(ls, getFileFullPath(luaFileName)); 
    if(isOpen!=0){ 
        CCLOG("Open Lua Error: %i", isOpen); 
        return NULL; 
    } 
  
    lua_getglobal(ls, varName); 
  
    int it = lua_gettop(ls); 
    lua_pushnil(ls); 
  
    string result=""; 
  
    while(lua_next(ls, it)) 
    { 
        string key = lua_tostring(ls, -2); 
        string value = lua_tostring(ls, -1); 
  
        result=result+key+":"+value+"\t"; 
  
        lua_pop(ls, 1); 
    } 
    lua_pop(ls, 1); 
  
    return result.c_str(); 
} 
  
const char* LuaInf::callLuaFunction(const char* luaFileName,const char* functionName){ 
    lua_State*  ls = CCLuaEngine::defaultEngine()->getLuaStack()->getLuaState(); 
  
    int isOpen = luaL_dofile(ls, getFileFullPath(luaFileName)); 
    if(isOpen!=0){ 
        CCLOG("Open Lua Error: %i", isOpen); 
        return NULL; 
    } 
  
    lua_getglobal(ls, functionName); 
  
    lua_pushstring(ls, "Himi"); 
    lua_pushnumber(ls, 23); 
    lua_pushboolean(ls, true); 
  
    /* 
     lua_call 
     第一个参数:函数的参数个数 
     第二个参数:函数返回值个数 
     */ 
    lua_call(ls, 3, 1); 
  
    const char* iResult = lua_tostring(ls, -1); 
  
    return iResult; 
} 
  
void  LuaInf::callCppFunction(const char* luaFileName){ 
  
    lua_State*  ls = CCLuaEngine::defaultEngine()->getLuaStack()->getLuaState(); 
  
    /* 
     Lua调用的C++的函数必须是静态的 
     */ 
    lua_register(ls, "cppFunction", cppFunction); 
  
    int isOpen = luaL_dofile(ls, getFileFullPath(luaFileName)); 
    if(isOpen!=0){ 
        CCLOG("Open Lua Error: %i", isOpen); 
        return; 
    } 
} 
  
int LuaInf::cppFunction(lua_State* ls){ 
    int luaNum = (int)lua_tonumber(ls, 1); 
    int luaStr = (int)lua_tostring(ls, 2); 
    CCLOG("Lua调用cpp函数时传来的两个参数： %d  %d",luaNum,luaStr); 
  
    /* 
     返给Lua的值 
     */ 
    lua_pushnumber(ls, 321); 
    lua_pushstring(ls, "Himi"); 
  
    /* 
     返给Lua值个数 
     */ 
    return 2; 
} 
  
const char* LuaInf::getFileFullPath(const char* fileName){ 
    return CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName).c_str(); 
} 
  
LuaInf::~LuaInf(){ 
  
    CC_SAFE_DELETE(_shared); 
    _shared=NULL; 
} 