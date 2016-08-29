
#include "PvpRankManager.h"
#include <iostream>

PvpRankManager::PvpRankManager()
{
    player_infos.clear();
    sort_arr_list.clear();
    c_root = new SectionCountRoot(1000, 50, true);
}

PvpRankManager::~PvpRankManager()
{
    auto iter = player_infos.begin();
    for(; iter != player_infos.end(); iter++)
        delete iter->second;
    player_infos.clear();
    
    auto sort_arr_iter = sort_arr_list.begin();
    for(; sort_arr_iter != sort_arr_list.end(); sort_arr_iter ++)
        delete sort_arr_iter->first;
    sort_arr_list.clear();
    
    if(c_root)
        delete c_root;
}

unsigned int PvpRankManager::SearchRank(int uin)
{
    if(!map_have_key(player_infos, uin))
        return 0;
    PlayerInfoDef * info = player_infos[uin];
    unsigned int point_rank = c_root->FindIndex(info->point);
    unsigned int cr = info->curArr->Index(info);
    if(cr ==0)
        return cr;
    return point_rank + cr;
}

void PvpRankManager::OnInfoChange(PlayerInfoDef * info)
{
    PlayerInfoDef * od_info = NULL;
	if (map_have_key(player_infos, info->uin))
	{
		od_info = player_infos[info->uin];
		if (od_info->point == info->point)
		{
			od_info->win_rate = info->win_rate;
			sort_arr_list.insert({ od_info->curArr, 'a' });
			delete info;
			return;
		}
		else
		{
			c_root->Remove(od_info->point);
			od_info->curArr->Remove(od_info);
			sort_arr_list.insert({ od_info->curArr, 'a' });
			od_info->point = info->point;
			od_info->win_rate = info->win_rate;
			delete info;
		}
	}
	else
	{
		player_infos.insert({ info->uin, info });
		od_info = info;
	}
    
    DelaySortArray * arr = (DelaySortArray *)c_root->Get(od_info->point);
    if(!arr)
    {
        arr = new DelaySortArray();
        arr->sort_fun = PvpRankManager::SortFun;
    }

	sort_arr_list.insert({ arr, 'a' });
    arr->Add(od_info);
	od_info->curArr = arr;
    c_root->Add(od_info->point, arr);
}
 
void PvpRankManager::RemovePlayer(int uin)
{
    auto iter = player_infos.find(uin);
    if (iter == player_infos.end())
        return;
    
    PlayerInfoDef * info = iter->second;
    player_infos.erase(iter);
    info->curArr->Remove(info);
    c_root->Remove(info->point);
    delete info;
}
 
void PvpRankManager::FlushAll()
{
    auto sort_arr_iter = sort_arr_list.begin();
    for(; sort_arr_iter != sort_arr_list.end(); sort_arr_iter ++)
    {
		sort_arr_iter->first->Flush();
    }
	
	sort_arr_list.clear();
}
 
 bool PvpRankManager::SortFun(void * a, void * b)
{
    PlayerInfoDef * pa = (PlayerInfoDef *)a;
    PlayerInfoDef * pb = (PlayerInfoDef *)b;
    
    if(pa->point == pb->point)
    {
        if(pa->win_rate == pb->win_rate)
            return pa->uin < pb->uin;
        else
            return pa->win_rate > pb->win_rate;
    }
    else
        return pa->point > pb->point;
}

PlayerInfoDef * PvpRankManager::GetInfoByRank(unsigned int rank)
{
	if (rank < 1 || rank > c_root->_count)
		return NULL;
    unsigned int front_index = 0;
    DelaySortArray * arr = (DelaySortArray *)c_root->ObjByIndex(rank, front_index);
    if(arr)
    {
        unsigned int section_rank = rank - front_index;
        return (PlayerInfoDef *)arr->ObjByIndex(section_rank);
    }
    return NULL;
}







 