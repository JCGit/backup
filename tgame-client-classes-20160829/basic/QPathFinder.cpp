#include "QPathFinder.h"
#include <cmath>

QPathFinder::QPathFinder():m_pMicroPather(NULL), m_pBlockIdxSet(NULL)
{
    m_iWidth = -1;
    m_iHeight = -1;
}

QPathFinder::~QPathFinder()
{
    if(m_pMicroPather) {
        m_pMicroPather->Reset();
        delete m_pMicroPather;
    }
}

void QPathFinder::Init(int iWidth, int iHeight,  set<int>& stBlocksIdxLst){
    m_iWidth = iWidth;
    m_iHeight = iHeight;
    m_pBlockIdxSet = &(stBlocksIdxLst);

    if(NULL == m_pMicroPather) {
        m_pMicroPather = new MicroPather(this, 1000); //��ʱ��1000��
    }
    m_pMicroPather->Reset();
}
void QPathFinder::NodeToXY( void* node, int* x, int* y ) 
{
    long index = (long)(node);
    *y = index / m_iWidth;
    *x = index - *y * m_iWidth;
}

void* QPathFinder::XYToNode( int x, int y )
{
    return (void*) ( y*m_iWidth + x );
}

//Return the least possible cost between 2 states.
//see class Graph for detail.
float QPathFinder::LeastCostEstimate( void* nodeStart, void* nodeEnd ) {

    int xStart, yStart, xEnd, yEnd;
    NodeToXY( nodeStart, &xStart, &yStart );
    NodeToXY( nodeEnd, &xEnd, &yEnd );

    int dx = xStart - xEnd;
    int dy = yStart - yEnd;
    return (float) sqrt( (double)(dx*dx) + (double)(dy*dy) );
}

//Return the exact cost from the given state to all its neighboring states.
//see class Graph for detail.
void QPathFinder::AdjacentCost( void* node, vector< micropather::StateCost > *neighbors ) {
    int x, y;
    static const int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
    static const int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
    static const float cost[8] = { 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f };

    NodeToXY( node, &x, &y );

    for( int i=0; i < 8; ++i ) {
        int nx = x + dx[i];
        int ny = y + dy[i];

        int pass = Passable( nx, ny );
        if ( pass > 0 ) {
            StateCost nodeCost = { XYToNode( nx, ny ), cost[i] };
            neighbors->push_back( nodeCost );
        }//end if
    } //end for
}

//see class Graph for detail.
void QPathFinder::PrintStateInfo( void* state ) {

}

//����true�������ߣ�����false ��������
bool QPathFinder::Passable( int iPosIdx){
    if(m_pBlockIdxSet) {
        return (m_pBlockIdxSet->count(iPosIdx) == 0);
    }
    return false;
}

//�����߷���1
//�����߷���0
int QPathFinder::Passable( int nx, int ny ) 
{
    //nx <= m_iWidth ��ʱ��<=
    if (nx >= 0 && nx <= m_iWidth 
        && ny >= 0 && ny <= m_iHeight )
    {
        int index = ny*m_iWidth+nx;
        return Passable(index);
    }		
    return 0;
}

/*
Ѱ·
@param iPlayerX ���X�±�
@param iPlayerY ���Y�±�
@param iDestX   �յ�X�±�
@param iDestY   �յ�Y�±�
@param path     ·�����±��б�
@return trueѰ·�ɹ�,falseѰ·ʧ��
*/
bool QPathFinder::FindPath(int iPlayerX, int iPlayerY, int iDestX, int iDestY, vector<void*>& path){
    if ( !Passable( iDestX, iDestY ) ) {
        return false;
    }
    float totalCost = 0;
    int result = m_pMicroPather->Solve( XYToNode( iPlayerX, iPlayerY ), XYToNode( iDestX, iDestY ), &path, &totalCost );

    if ( result == MicroPather::SOLVED ) {

        return true;
    }

    return false;
}


