#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <vector>
#include <iostream>


using namespace std;

struct Vertex{
	float x;
	float y;
	float z;
	int r;
	int g;
	int b;
	Vertex(){}
	Vertex(float ptx, float pty, float ptz){
		x = ptx;
		y = pty;
		z = ptz;
	}
	void setColor(int cr, int cb, int cg){
		r = cr;
		g = cb;
		b = cg;
	}
	friend ostream& operator<<(ostream& os, const Vertex& v){
		os << "[" << const_cast<Vertex&>(v).x << "," << const_cast<Vertex&>(v).y << "," << const_cast<Vertex&>(v).z << "]";
		return os;	
	}
	
};

struct Triangle{
	//Vertex tpt1, tpt2, tpt3;
	vector<Vertex> tpts;
	Triangle(){}
	Triangle(Vertex v1, Vertex v2, Vertex v3){
		//tpt1 = v1;
		//tpt2 = v2;
		//tpt3 = v3;
		tpts.push_back(v1);
		tpts.push_back(v2);
		tpts.push_back(v3);
	}
	void setTriangle(Vertex v1, Vertex v2, Vertex v3){
		tpts.clear();
		tpts.push_back(v1);
		tpts.push_back(v2);
		tpts.push_back(v3);
	}
	friend ostream& operator<<(ostream& os, const Triangle& t){
		os << const_cast<Triangle&>(t).tpts[0] <<  const_cast<Triangle&>(t).tpts[1] <<  const_cast<Triangle&>(t).tpts[2];
		return os;	
	}
	
};
struct Boundbox2D{
	float xmin, xmax;
	float ymin, ymax;
};
struct Boundbox3D{
	float xmin, xmax;
	float ymin, ymax;
	float zmin, zmax;
	Boundbox3D(){};
	Boundbox3D(float bxmin, float bxmax, float bymin, float bymax, float bzmin, float bzmax){
		xmin = bxmin;
		xmax = bxmax;
		ymin = bymin;
		ymax = bymax;
		zmin = bzmin;
		zmax = bzmax;
	}
};

Boundbox3D computeBoundbox(Triangle t);
Boundbox3D computeBoundbox(vector<Triangle> ts);
void computeTransform(int width, int height, Boundbox3D box3d, float &s, float &tx, float &ty);

float areaTriangle(Triangle t);
void computeBaryCoor(Triangle t, float x, float y, float &u1, float &u2, float &u3);

#endif
