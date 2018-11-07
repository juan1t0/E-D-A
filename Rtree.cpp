#include "Rtree.h"

Rtree::Rtree() {
	this->root = 0;
}
Rtree::Rtree(int M, int x, int X, int y, int Y) {
	vec_int Xs = { x , X };
	vec_int Ys = { y , Y };
	this->root = new Nodo(M, Xs, Ys);
	this->root->doRoot();
	this->root->changeIsLeaf(1);
}
Rtree::~Rtree() {
	this->root->~Nodo();
}
void Rtree::insert(par_int x) {
	par_int l = this->root->getLimite(0);
	par_int L = this->root->getLimite(1);
	Nodo* Rptr = this->root;
	Nodo* Leaff = Choose(Rptr, x);
	par_int li = Leaff->getLimite(0);
	par_int Li = Leaff->getLimite(1);
	Nodo* LL = 0;
	Leaff->addPoint(x);
	if (Leaff->getCantidad() > Leaff->getM()) {
		LL = SplitLeaf(Leaff);
		vec_par_int ll = LL->getAllPoints();
		vec_par_int le = Leaff->getAllPoints();
	}
	Nodo* RR = AdjusTree(Leaff, LL);
	if (RR) {
		Nodo *newroot = new Nodo();
		newroot->doRoot();
		newroot->addLimite(l);
		newroot->addLimite(L);
		newroot->changeM(this->root->getM());
		newroot->changeIsParent(1);
		newroot->addSon(this->root);
		this->root->dismiss();
		this->root->Reajustar();
		newroot->addSon(RR);
		this->root = newroot;
	}
}

Nodo* Rtree::Choose(Nodo *&R, par_int &punto) {
	Nodo* Raux = R;
	while (!Raux->knowIsLeaf()) {
		int n = Raux->getCantidad(), iesimo, major = 361201;
		for (int i = 0; i < n; ++i) {
			int area = Raux->getChild(i)->areaEnlarged(punto);
			if (major >= area) {
				iesimo = i;
				major = area;
			}
		}
		Raux = Raux->getChild(iesimo);
	}
	par_int a=Raux->getLimite(0), b=Raux->getLimite(1);
	return Raux;
}
Nodo* Rtree::SplitLeaf(Nodo *&R) {
	Nodo* RR = new Nodo();
	RR->changeM(R->getM());
	vec_par_int puntos(R->getAllPoints());
	R->clean();
	R->changeIsLeaf(1);
	RR->changeIsLeaf(1);
	PickSeed_toPoints(puntos, R, RR);
	while (!puntos.empty()) {
		par_int next = PickNext_toPoints(puntos, R, RR);
		int areaA = R->areaEnlarged(next);
		int areaB = RR->areaEnlarged(next);
		(areaA < areaB) ? R->addPoint(next) : RR->addPoint(next);
	}
	while (RR->getCantidad() < RR->getm()) {
		int aux = R->getCantidad() - 1;
		par_int temp = R->getPoint(aux);
		RR->addPoint(temp);
		R->removePoint(aux);
	}
	while (R->getCantidad() < R->getm()) {
		int aux = RR->getCantidad() - 1;
		par_int temp = RR->getPoint(aux);
		R->addPoint(temp);
		RR->removePoint(aux);
	}
	R->Ajustar();
	RR->Ajustar();
	return RR;
}
Nodo* Rtree::SplitNode(Nodo *&R) {
	Nodo* RR = new Nodo();
	RR->changeM(R->getM());
	vec_pnodo sons(R->getAllChilds());
	R->clean();
	R->changeIsParent(1);
	RR->changeIsParent(1);
	PickSeed_toRegion(sons, R, RR);
	while (!sons.empty()) {
		Nodo* next = PickNext_toRegion(sons, R, RR);
		par_int a = next->getLimite(0), b = next->getLimite(1);
		int areaA = R->areaEnlarged(next);
		int areaB = RR->areaEnlarged(next);
		(areaA < areaB) ? R->addSon(next) : RR->addSon(next);
	}
	while (RR->getCantidad() < RR->getm()) {
		int aux = R->getCantidad() - 1;
		Nodo* temp = R->getChild(aux);
		RR->addSon(temp);
		R->removeChild(aux);
	}
	while (R->getCantidad() < R->getm()) {
		int aux = RR->getCantidad() - 1;
		Nodo* temp = RR->getChild(aux);
		R->addSon(temp);
		RR->removeChild(aux);
	}
	return RR;
}

Nodo* Rtree::AdjusTree(Nodo *&R, Nodo *&RR) {
	while (R) {
		Nodo* papa = R->getParent();
		if (papa == 0)break;
		if (RR) {
			papa->addSon(RR);
			if (papa->getCantidad() > papa->getM())
				RR = SplitNode(papa);
			else
				RR = 0;
		}
		R = papa;
	}
	return RR;
}

void Rtree::PickSeed_toPoints(vec_par_int &Puntos, Nodo *& GroupA, Nodo *&GroupB) {
	std::size_t n = Puntos.size();
	int i = 0, j = 0, d = -1000;
	for (std::size_t ii = 0; ii < n - 1; ++ii) {
		for (std::size_t jj = ii + 1; jj < n; ++jj) {
			int temp = abs(Puntos[ii].first - Puntos[jj].first) * abs(Puntos[ii].second - Puntos[jj].second);
			if (d < temp) {
				d = temp;
				i = ii;
				j = jj;
			}
		}
	}
	GroupA->addPoint(Puntos[i]);
	GroupB->addPoint(Puntos[j]);
	Puntos.erase(Puntos.begin() + i);
	Puntos.erase(Puntos.begin() + (j-1));
}
par_int Rtree::PickNext_toPoints(vec_par_int &Puntos, Nodo *& GroupA, Nodo *& GroupB) {
	std::size_t n = Puntos.size();
	int areaA, areaB, d, D = -1000, ii;
	for (std::size_t i = 0; i < n; ++i) {
		areaA = GroupA->areaEnlarged(Puntos[i]);
		areaB = GroupB->areaEnlarged(Puntos[i]);
		d = std::abs(areaA - areaB);
		if (D < d) {
			D = d;
			ii = i;
		}
	}
	par_int entr = Puntos[ii];
	Puntos.erase(Puntos.begin() + ii);
	return entr;
}
void Rtree::PickSeed_toRegion(vec_pnodo &Regiones, Nodo *& GroupA, Nodo *&GroupB) {
	std::size_t n = Regiones.size();
	int i = 0, j = 0, d = -1000;

	for (std::size_t ii = 0; ii < n - 1; ++ii) {
		par_int r1 = Regiones[ii]->getLimite(0), R1 = Regiones[ii]->getLimite(1);
		int a1 = abs(R1.first - r1.first) * abs(R1.second - r1.second);
		for (std::size_t jj = ii + 1; jj < n; ++jj) {
			par_int r2 = Regiones[jj]->getLimite(0), R2 = Regiones[jj]->getLimite(1);
			int a2 = abs(R2.first - r2.first) * abs(R2.second - r2.second);
			int x, y, X, Y;
			(r1.first < r2.first)? x=r1.first : x = r2.first;
			(R1.first > R2.first)? X = r1.first : X = r2.first;
			(r1.second < r2.second)? y = r1.second : y = r2.second;
			(R1.second > R2.second)? Y = r1.second : Y = r2.second;

			int temp = abs(X - x) * abs(Y - y);
			temp = temp - (a1 + a2);
			if (d < temp) {
				d = temp;
				i = ii;
				j = jj;
			}
		}
	}
	GroupA->addSon(Regiones[i]);
	GroupB->addSon(Regiones[j]);
	Regiones.erase(Regiones.begin() + i);
	Regiones.erase(Regiones.begin() + (j-1));
}
Nodo* Rtree::PickNext_toRegion(vec_pnodo &Regiones, Nodo *& GroupA, Nodo *&GroupB) {
	std::size_t n = Regiones.size();
	int areaA, areaB, d, D = -1000, ii;
	for (std::size_t i = 0; i < n; ++i) {
		areaA = GroupA->areaEnlarged(Regiones[i]);
		areaB = GroupB->areaEnlarged(Regiones[i]);
		d = std::abs(areaA - areaB);
		if (D < d) {
			D = d;
			ii = i;
		}
	}
	Nodo* entr = Regiones[ii];
	Regiones.erase(Regiones.begin() + ii);
	return entr;
}
void Rtree::draw(){
    if(this->root->knowIsParent()){
        this->root->draw();
    }
}
