#include "Nodo.h"

Nodo::Nodo(int M, vec_int Lx, vec_int Ly) {
	this->M = M;
	this->m = std::ceil((double)M / 2);
	this->LimiteX = Lx;
	this->LimiteY = Ly;
	this->isLeaf = this->isParent = false;
}
Nodo::Nodo() {
	this->M = 0;
	this->m = 0;
	this->isLeaf = this->isParent = false;
}
Nodo::~Nodo() {
	this->puntos.clear();
	this->hijos.clear();
}

void Nodo::changeIsLeaf(bool b) {
	this->isLeaf = b;
}
void Nodo::changeIsParent(bool b) {
	this->isParent = b;
}

void Nodo::changeLimite(std::size_t i, par_int ps) {
	std::size_t sizeLimite = this->LimiteX.size();
	if (i >= sizeLimite)
		return;
	this->LimiteX[i] = ps.first;
	this->LimiteY[i] = ps.second;
}

void Nodo::addLimite(par_int li) {
	this->LimiteX.push_back(li.first);
	this->LimiteY.push_back(li.second);
}
void Nodo::addPoint(par_int p) {
	this->puntos.push_back(p);
	this->Cant = this->Cant + 1;
	this->Ajustar();
}
void Nodo::addSon(Nodo*& x) {
	//x->setParent(*this);
	x->padre = this;
	this->hijos.push_back(x);
	this->Cant = this->Cant + 1;
	if (this->isTheRoot)return;
	this->Reajustar();
}
void Nodo::setParent(Nodo& pa) {
	*(this->padre) = pa;
}
void Nodo::doRoot() {
	this->isTheRoot = true;
}
void Nodo::dismiss() {
	this->isTheRoot = false;
}
int Nodo::getCantidad() {
	return this->Cant;
}
int Nodo::getM() {
	return this->M;
}
int Nodo::getm() {
	return this->m;
}
par_int Nodo::getPoint(int i) {
	if (i > this->Cant)
		return std::make_pair(INFINITO, INFINITO);
	return this->puntos[i];
}
Nodo* Nodo::getChild(int i) {
	if (i >= this->Cant)
		return 0;
	return this->hijos[i];
}
par_int Nodo::getLimite(std::size_t i) {
	std::size_t sizeLimite = this->LimiteX.size();
	if (i > sizeLimite)
		return std::make_pair(INFINITO, INFINITO);
	return std::make_pair(this->LimiteX[i], this->LimiteY[i]);
}
void Nodo::removePoint(int i) {
	if (i >= this->Cant)
		return;
	this->puntos.erase(this->puntos.begin() + i);
	this->Cant = this->Cant - 1;
}
void Nodo::removeChild(int i) {
	if (i >= this->Cant)
		return;
	this->hijos.erase(this->hijos.begin() + i);
	this->Cant = this->Cant - 1;
}
bool Nodo::knowIsLeaf() {
	return this->isLeaf;
}
bool Nodo::knowIsParent() {
	return this->isParent;
}
bool Nodo::knowIsTheRoot() {
	return this->isTheRoot;
}
vec_par_int Nodo::getAllPoints() {
	return this->puntos;
}
std::vector<Nodo *> Nodo::getAllChilds() {
	return this->hijos;
}
void Nodo::clean() {
	this->hijos.clear();
	this->puntos.clear();
	this->Cant = 0;
}
int Nodo::areaEnlarged(par_int punto) {
	int x = this->LimiteX[0], y = this->LimiteY[0];
	int X = this->LimiteX[1], Y = this->LimiteY[1];

	int areaR = std::abs(X - x) * std::abs(Y - y);

	if (punto.first >= X)
		X = punto.first;
	else if (punto.first <= x)
		x = punto.first;

	if (punto.second >= Y)
		Y = punto.second;
	else if (punto.second <= y)
		y = punto.second;

	int areaRp = std::abs(X - x) * std::abs(Y - y);
	return areaRp - areaR;
}
int Nodo::areaEnlarged(Nodo* nodin) {
	int x = this->LimiteX[0], y = this->LimiteY[0];
	int X = this->LimiteX[1], Y = this->LimiteY[1];

	par_int n = nodin->getLimite(0), N = nodin->getLimite(1);
	int areaR = std::abs(X - x) * std::abs(Y - y);

	if (N.first >= X)
		X = N.first;
	else if (n.first <= x)
		x = n.first;

	if (N.second >= Y)
		Y = N.second;
	else if (n.second <= y)
		y = n.second;

	int areaRp = std::abs(X - x) * std::abs(Y - y);
	return areaRp - areaR;
}
void Nodo::Reajustar() {
	if (this->isLeaf)return;
	par_int r, R, aux;
	r = this->hijos[0]->getLimite(0);
	R = this->hijos[0]->getLimite(1);
	for (int i = 1; i < this->Cant; ++i) {
		aux = this->hijos[i]->getLimite(0);
		if (aux.first < r.first)
			r.first = aux.first;
		if (aux.second < r.second)
			r.second = aux.second;
		aux = this->hijos[i]->getLimite(1);
		if (aux.first > R.first)
			R.first = aux.first;
		if (aux.second > R.second)
			R.second = aux.second;
	}
	this->LimiteX.clear();
	this->LimiteY.clear();
	this->addLimite(r);
	this->addLimite(R);
}
void Nodo::Ajustar() {
	if (this->isTheRoot || this->isParent) return;
	int x = INFINITO, X = -1000;
	int y = INFINITO, Y = -1000;
	int px, py;
	for (int i = 0; i < this->Cant; ++i) {
		px = this->puntos[i].first;
		py = this->puntos[i].second;
		if (px < x) x = px;
		if (px > X) X = px;
		if (py < y) y = py;
		if (py > Y) Y = py;
	}
	this->LimiteX.clear();
	this->LimiteY.clear();
	this->addLimite(std::make_pair(x, y));
	this->addLimite(std::make_pair(X, Y));
}
void Nodo::changeM(int m) {
	this->M = m;
	this->m = std::ceil((double)m / 2);
}
Nodo* Nodo::getParent() {
	return this->padre;
}
void Nodo::draw(){
    if(isLeaf){
    glBegin(GL_LINES);
        glColor3d(0,0,0);
        glVertex2d(LimiteX[0],LimiteY[0]);
        glVertex2d(LimiteX[0],LimiteY[1]);
        glColor3d(0, 0, 0);
        glVertex2d(LimiteX[0],LimiteY[0]);
        glVertex2d(LimiteX[1],LimiteY[0]);
        glColor3d(0,0,0);
        glVertex2d(LimiteX[0],LimiteY[1]);
        glVertex2d(LimiteX[1],LimiteY[1]);
        glColor3d(0,0,0);
        glVertex2d(LimiteX[1],LimiteY[1]);
        glVertex2d(LimiteX[1],LimiteY[0]);
    glEnd();
    }
    if(isParent){
        for(int i=0;i<this->Cant;++i)
            this->hijos[i]->draw();
    }
}
