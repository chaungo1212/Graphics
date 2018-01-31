#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <time.h>
#include <stdlib.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

//#include "Image.h" // inlude in Utilis.h
//#include "Geometry.h" // include in Utilis.h
#include "Utilis.h"

// This allows you to skip the `std::` in front of C++ standard library
// functions. You can also say `using std::cout` to be more selective.
// You should never do this in a header file.
using namespace std;

int main(int argc, char **argv)
{
	if(argc < 5) {
		cout << "Usage: A1 meshfile outputFile width height colorMode" << endl;
		return 0;
	}
	string meshName(argv[1]);
	string outFile(argv[2]);
	// Width of image
	int width = atoi(argv[3]);
	if (width < 0 ){
		cout << "Invalid width! Please enter a positive number greater than 0!" <<endl;
	}
	// Height of image
	int height = atoi(argv[4]);
	if (height < 0 ){
		cout << "Invalid height! Please enter a positive number greater than 0!" <<endl;
	}
	// Create the image. We're using a shared_ptr, a C++11 feature.
	//auto image = make_shared<Image>(width, height);
	int colorMode = atoi(argv[5]);
	if (colorMode < 0 && colorMode > 2){
		cout << "Invalid color mode! Please enter 0, 1, or 2" << endl;
	}
	
	srand (time(NULL));	
	
	// Load geometry
	vector<float> posBuf; // list of vertex positions
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	string errStr;
	bool rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &errStr, meshName.c_str());
	if(!rc) {
		cerr << errStr << endl;
	} else {
		// Some OBJ files have different indices for vertex positions, normals,
		// and texture coordinates. For example, a cube corner vertex may have
		// three different normals. Here, we are going to duplicate all such
		// vertices.
		// Loop over shapes
		for(size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces (polygons)
			size_t index_offset = 0;
			for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				size_t fv = shapes[s].mesh.num_face_vertices[f];
				// Loop over vertices in the face.
				for(size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					posBuf.push_back(attrib.vertices[3*idx.vertex_index+0]);
					posBuf.push_back(attrib.vertices[3*idx.vertex_index+1]);
					posBuf.push_back(attrib.vertices[3*idx.vertex_index+2]);
				}
				index_offset += fv;
				// per-face material (IGNORE)
				shapes[s].mesh.material_ids[f];
			}
		}
	}
	//cout << "Number of vertices: " << posBuf.size()/3 << endl;
	//int ntriangle = (int)posBuf.size()/(3*3);
	vector<Triangle> triangles;
	for(size_t i = 0; i < posBuf.size(); i=i+9){
		// ith triangle
		int r, g, b;
		// Vertex 1
		Vertex v1(posBuf[i], posBuf[i+1], posBuf[i+2]);
		r = rand()%256;
		g = rand()%256;
		b = rand()%256;
		v1.setColor(r, g, b); 
		//cout << v1.r << " " << v1.g << " " << v1.b << " ";
		// Vertex 2
		Vertex v2(posBuf[i+3], posBuf[i+4], posBuf[i+5]);
		r = rand()%256;
		g = rand()%256;
		b = rand()%256;
		v2.setColor(r,g,b);
		//cout << v2.r << " " << v2.g << " " << v2.b << " ";
		// Vertex 3
		Vertex v3(posBuf[i+6], posBuf[i+7], posBuf[i+8]);
		r = rand()%256;
		g = rand()%256;
		b = rand()%256;
		v3.setColor(r,g,b);
		Triangle itriangle(v1, v2, v3);
		//cout << v3.r << " " << v3.g << " " << v3.b << endl;
		triangles.push_back(itriangle);
		
	}
	
	Boundbox3D box3d = computeBoundbox(triangles);
	// Transformation
	float s, tx, ty;
	computeTransform(width, height, box3d, s, tx, ty);
	cout << "Transformation: s("<< s << ") tx(" << tx << ") ty(" << ty << ")" << endl;
	
	// Mode 0
	perVertexColoring(width, height, triangles, s, tx, ty, outFile);
	
	return 0;
}
