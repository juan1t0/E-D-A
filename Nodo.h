#ifndef NODO_H
#define NODO_H
#define INFINITO 1024

#include <iostream>
#include <vector>
#include <utility>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <windows.h>
#include<GL/glut.h>

typedef std::pair<int,int> par_int;
typedef std::vector<int> vec_int;
typedef std::vector<par_int> vec_par_int;

using namespace std;
class Nodo
{
    vec_par_int puntos;
    std::vector<Nodo *> hijos;


    int M , m , Cant = 0;
    vec_int LimiteX;
    vec_int LimiteY;

    bool isLeaf;
    bool isParent;
    bool isTheRoot = false;
public:
    Nodo* padre = 0;//nullptr;
    Nodo(int M, vec_int Lx, vec_int Ly);
    Nodo();
    ~Nodo();

    void changeIsLeaf(bool);
    void changeM(int m);
    void changeIsParent(bool);
    void changeLimite(std::size_t i,par_int ps);

    void addLimite(par_int li);
    void addPoint(par_int p);
    void addSon(Nodo*& x);

    void setParent(Nodo& pa);
    void doRoot();
    void dismiss();

    int getCantidad();
    int getM();
    int getm();
    par_int getPoint(int i);
    Nodo* getChild(int i);
    Nodo* getParent();
    par_int getLimite(std::size_t i);
    vec_par_int getAllPoints();
    std::vector<Nodo *> getAllChilds();

    int areaEnlarged(par_int p);
    int areaEnlarged(Nodo* n);

    void removePoint(int i);
    void removeChild(int i);
    void clean();

    bool knowIsLeaf();
    bool knowIsParent();
    bool knowIsTheRoot();

    void Reajustar();
    void Ajustar();

    void draw();
};

#endif // NODO_H
