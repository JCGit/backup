#include "SectionCount.h"
#include <cmath>
#include <iostream>

SectionCount::SectionCount(unsigned long long minPkv, unsigned long long maxPkv, unsigned int maxSize):
    _minPkv(minPkv),
    _maxPkv(maxPkv),
    _maxSize(maxSize),
    _count(0)
{
    if((maxPkv - minPkv) <= maxSize)
    {
        //叶子节点
        _type = 1;
    }
    else
    {
        _type = 2;
        unsigned long long middle = floor((maxPkv - minPkv) / 2);
        SectionCount * c1 = new SectionCount(minPkv, middle + minPkv, maxSize);
        _childrens.insert({1, c1});
        SectionCount * c2 = new SectionCount(middle + 1 + minPkv, maxPkv, maxSize);
        _childrens.insert({2, c2});
    }
}

SectionCount::~SectionCount()
{
    if(_type == 2)
    {
        auto iter = _childrens.begin();
        for(; iter != _childrens.end(); iter ++)
        {
             delete iter->second;
        }
    }
    else if(_type == 1)
    {
        auto iter = _map.begin();
        for(; iter != _map.end(); iter ++)
        {
             delete iter->second;
        }
    }
}


void SectionCount::Add(unsigned long long  pkv, void * obj)
{
     if(_type == 1)
     {
         if(map_have_key(_map, pkv))
         {
            _map[pkv]->count += 1;
            _map[pkv]->obj = obj;
         }
         else
         {
            CountDatDef * def = new CountDatDef();
            def->obj = obj;
            def->count = 1;
            _map.insert({pkv, def});
         }
     }
     else if(_type == 2)
     {
         if(pkv <= _childrens[1]->_maxPkv)
             _childrens[1]->Add(pkv, obj);
         else
             _childrens[2]->Add(pkv, obj);
     }
     _count ++;
}


bool SectionCount::Remove(unsigned long long pkv)
{
    if (_count < 1)
        return false;
    bool ret = false;
    if(_type == 1)
    {
        if(map_have_key(_map, pkv))
        {
             _map[pkv]->count --;
             ret = true;
        }
    }
    else
    {
        if(pkv <= _childrens[1]->_maxPkv)
            ret = _childrens[1]->Remove(pkv);
        else
            ret = _childrens[2]->Remove(pkv);
    }

    if (ret)
        _count --;
    return ret;
}

void * SectionCount::Get(unsigned long long pkv)
{
    if(_type == 1)
    {
        if(map_have_key(_map, pkv))
            return _map[pkv]->obj;
        return NULL;
    }
    else
    {
         if (pkv <= _childrens[1]->_maxPkv)
            return _childrens[1]->Get(pkv);
         else
            return _childrens[2]->Get(pkv);
    }
}

unsigned int SectionCount::FindIndex(unsigned long long pkv, bool & bReverse)
{
    if(_type == 1)
    {
        if(!map_have_key(_map, pkv))
            return 0;

        unsigned int index = 0;
        if(bReverse)
        {
            auto iter = _map.rbegin();
            for(; iter != _map.rend(); iter ++)
            {
                if(iter->first == pkv)
                    break;
                index += iter->second->count;
            }
        }
        else
        {
            auto iter = _map.begin();
            for(; iter != _map.end(); iter ++)
            {
                if(iter->first == pkv)
                    break;
                index += iter->second->count;
            }
        }
        return index;
    }
    else
    {
        unsigned int c_i = 0;
        if(pkv <= _childrens[1]->_maxPkv)
        {
            c_i = _childrens[1]->FindIndex(pkv, bReverse);
             if(bReverse)
                 return _childrens[2]->_count + c_i;
             else
                 return c_i;
        }
        else
        {
            c_i = _childrens[2]->FindIndex(pkv, bReverse);
            if(bReverse)
                return c_i;
            else
                return _childrens[1]->_count + c_i;
        }
    }
}

void * SectionCount::ObjByIndex(unsigned int index, unsigned int & front_index, bool & bReverse)
{
    if(index > _count)
        return NULL;
    if(_type == 1)
    {
        unsigned int add_index = 0;
        if(bReverse)
        {
            auto iter = _map.rbegin();
            for(; iter != _map.rend(); iter ++)
            {
                add_index += iter->second->count;
                if (add_index >= index)
                    return iter->second->obj;
                front_index += iter->second->count;
            }       
        }
        else
        {
            auto iter = _map.begin();
            for(; iter != _map.end(); iter ++)
            {
                add_index += iter->second->count;
                if (add_index >= index)
                    return iter->second->obj;
                front_index += iter->second->count;
            }
        }
        return NULL;
    }
    else
    {
        if(bReverse)
        {
            if(index <= _childrens[2]->_count)
                return _childrens[2]->ObjByIndex(index, front_index, bReverse);
            else
            {
                front_index += _childrens[2]->_count;
                return _childrens[1]->ObjByIndex(index - _childrens[2]->_count, front_index, bReverse);
            }
        }
        else
        {
            if(index <= _childrens[1]->_count)
                return _childrens[1]->ObjByIndex(index, front_index, bReverse);
            else
            {
                front_index += _childrens[1]->_count;
                return _childrens[2]->ObjByIndex(index - _childrens[1]->_count, front_index, bReverse);
            }
        }
    }
}




SectionCountRoot::SectionCountRoot(unsigned int blockSize, unsigned int maxSize, bool bReverse):
    _bReverse(bReverse),
    _blockSize(blockSize),
    _maxSize(maxSize),
    _count(0)
{

}

SectionCountRoot::~SectionCountRoot()
{
     auto iter = _map.begin();
     for(; iter != _map.end(); iter ++)
     {
         delete iter->second;
     }
     _map.clear();
}

SectionCount * SectionCountRoot::FindCount(unsigned long long pkv, bool bCreate)
{
    unsigned int block = floor(pkv / _blockSize) + 1;
    if(map_have_key(_map, block))
        return _map[block];
    else
    {
         if(bCreate)
         {
             SectionCount * c = new SectionCount(_blockSize * (block - 1),
                                                    _blockSize * block -1,
                                                    _maxSize);
             _map.insert({block, c});
             return c;
         }
         else
             return NULL;
    }
}

void SectionCountRoot::Add(unsigned long long pkv, void * obj)
{
    SectionCount * c = FindCount(pkv, true);
    _count ++;
    return c->Add(pkv, obj);
}

void SectionCountRoot::Remove(unsigned long long pkv)
{
    SectionCount * c = FindCount(pkv, false);
    if(c)
    {
        bool ret = c->Remove(pkv);
        if(ret)
            _count --;
    }
}

unsigned int SectionCountRoot::FindIndex(unsigned long long pkv)
{
    unsigned int block = floor(pkv / _blockSize) + 1;
    if(!map_have_key(_map, block))
        return 0;

    unsigned int index = _map[block]->FindIndex(pkv, _bReverse);
    if(_bReverse)
    {
        auto riter = _map.rbegin();
        for (; riter != _map.rend(); riter ++)
        {
            if (block == riter->first)
                break;
            index += riter->second->_count;
        }
    }
    else
    {
        auto iter = _map.begin();
        for (; iter != _map.end(); iter ++)
        {
            if (block == iter->first)
                break;
            index += iter->second->_count;
        }
    }
    return index;
}

void * SectionCountRoot::Get(unsigned long long pkv)
{
    SectionCount * c = FindCount(pkv, false);
    if(c)
        return c->Get(pkv);
    else
        return NULL;
}

void * SectionCountRoot::ObjByIndex(unsigned int index, unsigned int & front_index)
{
    if(index > _count)
        return NULL;
    
    unsigned int add_index = 0;
    front_index = 0;
    if(_bReverse)
    {
        auto riter = _map.rbegin();
        for (; riter != _map.rend(); riter ++)
        {
            add_index += riter->second->_count;
            if(add_index >= index)
                return riter->second->ObjByIndex(index - front_index, front_index, _bReverse);
            front_index += riter->second->_count;
        }
        return NULL;
    }
    else
    {
        auto iter = _map.begin();
        for (; iter != _map.end(); iter ++)
        {
            add_index += iter->second->_count;
            if(add_index >= index)
                return iter->second->ObjByIndex(index - front_index, front_index, _bReverse);
            front_index += iter->second->_count;
        }
        return NULL;
    }
}





