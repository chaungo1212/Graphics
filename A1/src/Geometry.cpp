#include "Geometry.h"

Boundbox3D computeBoundbox(Triangle t){
	float bxmin, bymin, bzmin;
	bxmin = bymin = bzmin = 1000000;
	float bxmax, bymax, bzmax;
	bxmax= bymax= bzmax=-10000000;

	for(size_t i = 0; i < 3; i++){ // Find the boundbox range throug the iteration of three vertexs.
		float ix = t.tpts[i].x,
			  iy = t.tpts[i].y,
			  iz = t.tpts[i].z;
		if(ix < bxmin)
			bxmin = ix;
		if(ix > bxmax)
			bxmax = ix;
		if(iy < bymin)
			bymin = iy;
		if(iy > bymax)
			bymax = iy;
		if(iz < bzmin)
			bzmin = iz;
		if(iz > bzmax)
			bzmax = iz;
	}
	Boundbox3D b(bxmin, bxmax, bymin, bymax, bzmin, bzmax);
	return b;
}

Boundbox3D computeBoundbox(vector<Triangle> ts){
	float bxmin, bymin, bzmin;
	bxmin = bymin = bzmin = 1000000;
	float bxmax, bymax, bzmax;
	bxmax= bymax= bzmax=-1000000;
	for(size_t i = 0; i < ts.size(); i++){
		Boundbox3D ibox = computeBoundbox(ts[i]);
		if(ibox.xmin < bxmin)
			bxmin = ibox.xmin;
		if(ibox.xmax > bxmax)
			bxmax = ibox.xmax;
		if(ibox.ymin < bymin)
			bymin = ibox.ymin;
		if(ibox.ymax > bymax)
			bymax = ibox.ymax;
		if(ibox.zmin < bzmin)
			bzmin = ibox.zmin;
		if(ibox.zmax > bzmax)
			bzmax = ibox.zmax;
	}
	Boundbox3D b(bxmin, bxmax, bymin, bymax, bzmin, bzmax); 
	return b;
}

void computeTransform(int width, int height, Boundbox3D box3d, float &s, float &tx, float &ty){
	float bdx = box3d.xmax - box3d.xmin,
		  bdy = box3d.ymax - box3d.ymin; 
	float sx = width/bdx,
		  sy = height/bdy;
	s = sx < sy? sx:sy; // Compute the scale in the transformation.
	// xi = s*xw + tx
	tx = 0.5*width - s*0.5*(box3d.xmin+box3d.xmax);
	// yi = s*yw + ty
	ty = 0.5*height - s*0.5*(box3d.ymin+box3d.ymax);
}

float areaTriangle(Triangle t){
	// Triangle area by Shoelace formula.
	float Ax = t.tpts[0].x;
	float Ay = t.tpts[0].y;
	float Bx = t.tpts[1].x;
	float By = t.tpts[1].y;
	float Cx = t.tpts[2].x;
	float Cy = t.tpts[2].y;
	float product1 = Ax*By + Bx*Cy + Cx*Ay;
	float product2 = Ax*Cy + Cx*By + Bx*Ay;
	float areaT = 0.5*(product1 - product2);
	areaT = (Ax*(By - Cy) + Bx*(Cy - Ay) + Cx*(Ay - By)) / 2.0;
	return areaT;
	
}

void computeBaryCoor(Triangle t, float x, float y, float &u1, float &u2, float &u3){ // P = u1*A + u2*B + u3*C.
	float areaABC = areaTriangle(t); 
	
	Vertex P;
	P.x = x;
	P.y = y; 
	// Three triangles which connects to two of three points of the Triangle t.
	Vertex A = t.tpts[0], B = t.tpts[1], C = t.tpts[2];
	Triangle BCP, ACP, ABP;
	BCP.setTriangle(B, C, P);
	float areaBCP = areaTriangle(BCP); 

	ACP.setTriangle(A, P, C);
	float areaACP = areaTriangle(ACP);

	ABP.setTriangle(A, B, P);
	float areaABP = areaTriangle(ABP);

	//float areaABC = areaBCP + areaACP + areaABP; 
	//cout << areaBCP << " " << areaACP << " " << areaABP << endl;
	u1 = areaBCP/areaABC;
	u2 = areaACP/areaABC;
	u3 = areaABP/areaABC;

}

