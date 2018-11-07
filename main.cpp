#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <string>
#include <stdlib.h>
#include <time.h>
#include<GL/glut.h>
#define KEY_ESC 27

#include "Rtree.h"

using namespace std;

int xx,yy;
int m = 50;
vector<pair<int,int> > pointss;
vector<pair<int,int> > points_f;
Rtree * Rt;

void displayPoints()
{
	size_t s=pointss.size();
    glPointSize(2.5f);
    glBegin(GL_POINTS);
        glColor3d(1,0,0);
        for(size_t i = 0;i < s;i++){
            glVertex2i(pointss[i].first, pointss[i].second);
        }
	glEnd();

	size_t ss=points_f.size();
    glPointSize(3.5f);
    glBegin(GL_POINTS);
        glColor3f(0.12f,0.160f,0.243f);
        for(size_t i = 0;i < ss;i++){
            glVertex2i(points_f[i].first, points_f[i].second);
        }
	glEnd();
}
void displayGizmo()
{
	glBegin(GL_LINES);
        glColor3d(0,0,0);
        glVertex2d(-300, 0);
        glVertex2d(300, 0);
        glColor3d(0, 0, 0);
        glVertex2d(0,-300);
        glVertex2d(0, 300);
	glEnd();
}
const  double  pi2 = 6.28318530718;
void drawRectangle(int &x, int &y){
    static double radius = 50;
    const double delta_theta = pi2/25;
    double xcenter = x , ycenter = y;
    double xx, yy;
    double theta = 0.0;
    glColor3f(0.255f,0.204f,0.255f);
    glBegin(GL_LINES);
        double auX=xcenter + radius * sin(theta);
        double auY=ycenter + radius * cos(theta);
        while (theta <= pi2) {
            theta += delta_theta;
            xx = xcenter + radius * sin(theta);
            yy = ycenter + radius * cos(theta);
            glVertex2f(auX, auY);
            glVertex2f(xx, yy);
            auX=xx;
            auY=yy;
        }
    glEnd();
}
void OnMouseClick(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
    pair<float,float> pt(x,y);
    if (x>=300)
        x-=300;
    else
        x =(300-x)*-1;
    if (y<=300)
        y=300-y;
    else
        y=(y-300)*-1;
    pointss.push_back(make_pair(x,y));
    cout<<"punto "<<pointss.size()<<": ("<<x<<" "<<y<<")"<<endl;
    Rt->insert(make_pair(x,y));
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
        if (x>=300)
            x-=300;
        else
            x =(300-x)*-1;
        if (y<=300)
            y=300-y;
        else
            y=(y-300)*-1;

        xx=x;
        yy=y;
  }
}

void OnMouseMotion(int x, int y)
{

    if (x>=300)
        x-=300;
    else
        x =(300-x)*-1;
    if (y<=300)
        y=300-y;
    else
        y=(y-300)*-1;

    xx=x;
    yy=y;
}

void idle()
{
    glutPostRedisplay();
}

void glPaint(void) {
	glClear(GL_COLOR_BUFFER_BIT); ///CAMBIO
	glLoadIdentity();
	glOrtho(-300.0f, 300.0f,-300.0f, 300.0f, -1.0f, 1.0f);

    displayPoints();
    Rt->draw();
	glutSwapBuffers();
}

void init_GL(void) {
	glClearColor(0, 200, 250, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

GLvoid window_redraw(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

GLvoid window_key(unsigned char key, int x, int y) {
	switch (key) {
	case KEY_ESC:
		exit(0);
		break;
	default:
		break;
	}
}
void insertchicago(vector<double> &A,vector<double> &B){
    double ix,iy;
    size_t zz = A.size();
    for(int i=0;i<zz;++i){
        ix=B[i];
        iy=A[i];
        ix*=600.0;
        iy*=600.0;

        if (ix>=0.0)
            ix+=300.0;
        else
            ix =(300.0+ix);
        if (iy>=0.0)
            iy=300.0-iy;
        else
            iy=(iy*-1.0)+300.0;
        pair<float,float> pt;
        pt=make_pair((float)ix,(float)iy);
        if (ix>=300)
            ix-=300;
        else
            ix =(300-ix)*-1;
        if (iy<=300)
            iy=300-iy;
        else
            iy=(iy-300)*-1;
        pointss.push_back(make_pair(ix,iy));
        Rt->insert(make_pair(ix,iy));

    }
}
void insert_rando(int x){
    srand(time(NULL));
    float ix,iy;
    for(int i=0;i<x;i++){
        ix = rand() %600;
        iy = rand() %600;
        pair<float,float> pt(ix,iy);
        if (ix>=300)
            ix-=300;
        else
            ix =(300-ix)*-1;
        if (iy<=300)
            iy=300-iy;
        else
            iy=(iy-300)*-1;
        pointss.push_back(make_pair(ix,iy));
        Rt->insert(make_pair(ix,iy));
    }
}
int main(int argc, char** argv) {
	vector<double> A(50000);
	vector<double> B(50000);
	ifstream lat("D:\\juan1t0\\codeB\\Torrtree\\bin\\Debug\\posiciones_chicago.txt");//_capital.txt");
	int i=0;
	string::size_type sz;
	while(!lat.eof()){
        lat>>A[i]>>B[i];
        ++i;
    }
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("R-Tree : juan1t0");

	init_GL();

	glutDisplayFunc(glPaint);
	glutReshapeFunc(&window_redraw);
	glutKeyboardFunc(&window_key);
	glutMouseFunc(&OnMouseClick);
	glutIdleFunc(&idle);

    Rt = new Rtree(51,0,600,0,600);
    insertchicago(A,B);
   // insert_rando(500);
	glutMainLoop();

	return 0;
}
