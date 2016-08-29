#pragma once

//auth:diandian
//date:2016.4.18
//区间计数封装

#include <map>
#include <iostream>
#include "DelaySortArray.h"

using namespace std;

class SectionCountRoot;
class SectionCount
{
    typedef struct CountDat
    {
        void * obj;
        unsigned int count;
    }CountDatDef;

    public:
        SectionCount(unsigned long long minPkv, unsigned long long maxPkv, unsigned int maxSize);
        ~SectionCount();

        void Add(unsigned long long pkv, void * obj);
        bool Remove(unsigned long long pkv);
        void * Get(unsigned long long pkv);
        unsigned int FindIndex(unsigned long long pkv, bool & bReverse);
        void * ObjByIndex(unsigned int index, unsigned int & front_index, bool & bReverse);

        friend class SectionCountRoot;
    protected:
        map<unsigned long long, CountDatDef *>    _map;
        map<char, SectionCount *>             _childrens;
        unsigned int                          _minPkv;
        unsigned int                          _maxPkv;
        unsigned int                          _maxSize;
        unsigned long long                    _count;
        char                                  _type;
};

class SectionCountRoot
{
    public:
        SectionCountRoot(unsigned int blockSize, unsigned int maxSize, bool bReverse = false);
        ~SectionCountRoot();

        static SectionCount * CreateCount(int index);
        SectionCount * FindCount(unsigned long long pkv, bool bCreate);
        void Add(unsigned long long pkv, void * obj);
        void Remove(unsigned long long pkv);
        unsigned int FindIndex(unsigned long long pkv);
        void * Get(unsigned long long pkv);
        void * ObjByIndex(unsigned int index, unsigned int & front_index);

    protected:
        map<unsigned int, SectionCount *> _map;
        bool _bReverse;
        unsigned int _blockSize;
        unsigned int _maxSize;

    public:
        unsigned int _count;
};