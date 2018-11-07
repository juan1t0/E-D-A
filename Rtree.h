#ifndef RTREE_H
#define RTREE_H

#include "Nodo.h"

typedef std::pair<int,int> par_int;
typedef std::vector<int> vec_int;
typedef std::vector<par_int> vec_par_int;
typedef std::vector<Nodo *> vec_pnodo;

class Rtree
{
    Nodo *root;

    Nodo* Choose(Nodo *&R , par_int &punto);
    Nodo* SplitLeaf(Nodo *&R);
    Nodo* SplitNode(Nodo *&R);
    Nodo* AdjusTree(Nodo *&R,Nodo *&RR);

    void PickSeed_toPoints(vec_par_int &Puntos,Nodo *& GroupA, Nodo *&GroupB);
    par_int PickNext_toPoints(vec_par_int &Puntos,Nodo *& GroupA, Nodo *&GroupB);

    void PickSeed_toRegion(vec_pnodo &Regiones,Nodo *& GroupA, Nodo *&GroupB);
    Nodo* PickNext_toRegion(vec_pnodo &Regiones,Nodo *& GroupA, Nodo *&GroupB);

public:
    Rtree();
    Rtree(int M, int x,int X, int y, int Y);
    ~Rtree();

    void insert(par_int x);
    void search(par_int x);
    void draw();
};
#endif // RTREE_H
