
#include <lua.hpp>
#include <unordered_map>
#include <iostream>
#include "DelaySortArray.h"
#include "SectionCount.h"
#include "PvpRankManager.h"

extern "C"
{
    int luaopen_pvp_rank_mgr(lua_State *L);
}

PlayerInfoDef * GetPlayerInfoByState(lua_State * L, int pi);

int Lua_OnPlayerRankChange(lua_State * L);
int Lua_SearchPlayerRank(lua_State * L);
int Lua_RemovePlayer(lua_State * L);
int Lua_CreateHandle(lua_State * L);
int Lua_FlushAll(lua_State * L);
int Lua_GetInfoCount(lua_State * L);
int Lua_SearchRankPlayer(lua_State * L);


int luaopen_pvp_rank_mgr(lua_State *L)
{
    luaL_checkversion(L);
    luaL_Reg l[] = {
        {"on_player_rank_change", Lua_OnPlayerRankChange},
        {"search_player_rank", Lua_SearchPlayerRank},
        {"remove_player", Lua_RemovePlayer},
        {"create_handle", Lua_CreateHandle},
        {"flush_all", Lua_FlushAll},
        {"get_info_count", Lua_GetInfoCount},
        {"search_rank_player", Lua_SearchRankPlayer},
        {NULL, NULL},
    };
    luaL_newlib(L, l);
    return 1;
}

PlayerInfoDef * GetPlayerInfoByState(lua_State * L, int pi)
{
    PlayerInfoDef * info = new PlayerInfoDef();
    lua_getfield(L, pi, "point");
    info->point = luaL_checkinteger(L, -1);
    lua_getfield(L, pi, "win_rate");
    info->win_rate = luaL_checknumber(L, -1);
    lua_getfield(L, pi, "uin");
    info->uin = luaL_checkinteger(L, -1);
    return info;
}

int Lua_OnPlayerRankChange(lua_State * L)
{
    PvpRankManager * mgr = (PvpRankManager *)lua_touserdata(L, 1);
    PlayerInfoDef * info = GetPlayerInfoByState(L, 2);
    mgr->OnInfoChange(info);
    return 0;
}

int Lua_SearchPlayerRank(lua_State * L)
{
    PvpRankManager * mgr = (PvpRankManager *)lua_touserdata(L, 1);
    int uin = luaL_checkinteger(L, 2);
    unsigned int rank = mgr->SearchRank(uin);
    lua_pushinteger(L, rank);
    return 1;
}

int Lua_RemovePlayer(lua_State * L)
{
    PvpRankManager * mgr = (PvpRankManager *)lua_touserdata(L, 1);
    int uin = luaL_checkinteger(L, 2);
    mgr->RemovePlayer(uin);
    return 0;
}

int Lua_CreateHandle(lua_State * L)
{
    PvpRankManager * mgr = new PvpRankManager();
    lua_pushlightuserdata(L, mgr);
    return 1;
}

int Lua_FlushAll(lua_State * L)
{
    PvpRankManager * mgr = (PvpRankManager *)lua_touserdata(L, 1);
    mgr->FlushAll();
    return 0;
}

int Lua_GetInfoCount(lua_State * L)
{
    PvpRankManager * mgr = (PvpRankManager *)lua_touserdata(L, 1);
    unsigned int count = mgr->GetInfoCount();
    lua_pushinteger(L, count);
    return 1;
}

int Lua_SearchRankPlayer(lua_State * L)
{
    PvpRankManager * mgr = (PvpRankManager *)lua_touserdata(L, 1);
    unsigned int rank = luaL_checkinteger(L, 2);
    PlayerInfoDef * info = mgr->GetInfoByRank(rank);
    if (info)
    {
        lua_pushinteger(L, info->uin);
        return 1;
    }
    return 0;
    
}

