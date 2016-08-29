#ifndef QPathFinder_h__
#define QPathFinder_h__

#include "micropather.h"
#include <set>
using namespace micropather;
using namespace std;



class QPathFinder   : public Graph
{
public:
	QPathFinder();
	~QPathFinder();

    void Init(int iWidth, int iHeight, set<int>& stBlocksIdxLst);

    virtual float LeastCostEstimate( void* nodeStart, void* nodeEnd );
	virtual void AdjacentCost( void* node, vector< micropather::StateCost > *neighbors );
    virtual void  PrintStateInfo( void* state );

    bool FindPath(int iPlayerX, int iPlayerY, int iDestX, int iDestY, std::vector<void*>& path);
private:
    bool Passable( int iPosIdx);
    int Passable( int nx, int ny );
    void NodeToXY( void* node, int* x, int* y );

    void* XYToNode( int x, int y );
private:
    MicroPather *m_pMicroPather;
    set<int> *m_pBlockIdxSet;

    int m_iWidth;
    int m_iHeight;
};

#endif // QPathFinder_h__
