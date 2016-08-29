#pragma once

//auth:diandian
//date:2016.4.18


#include <unordered_map>
#include <list>
#include "DelaySortArray.h"
#include "SectionCount.h"


using namespace std;


typedef struct PlayerInfo
{
    PlayerInfo():curArr(NULL){};
    
    int     point;
    float   win_rate;
    int     uin;
    
    DelaySortArray * curArr;
}PlayerInfoDef;

class PvpRankManager
{
    public:
        PvpRankManager();
        ~PvpRankManager();
       
        unsigned int SearchRank(int uin);
        void OnInfoChange(PlayerInfoDef * info);
        void RemovePlayer(int uin);
        void FlushAll();
        unsigned int GetInfoCount(){ return player_infos.size(); };
        PlayerInfoDef * GetInfoByRank(unsigned int rank);
        
		static bool SortFun(void * a, void * b);
    private:
        unordered_map<int, PlayerInfoDef *> player_infos;
        SectionCountRoot * c_root;
		unordered_map<DelaySortArray *, char> sort_arr_list;
        
};

