#include "Utilis.h"

void perVertexColoring(int width, int height, vector<Triangle> ts, float s, float tx, float ty, string outFile){
 	auto image = make_shared<Image>(width, height); 

	for(size_t i = 0; i < ts.size(); i++){ // iterate through every triangle.
		// Scale the ith triangle.
		Triangle it = ts[i]; 
		//cout << it << " -> ";
		vector<Vertex> ivs(3);
		for(size_t j = 0; j < 3; j++){
			Vertex jv;
			jv.x = s*it.tpts[j].x + tx;	
			jv.y = s*it.tpts[j].y + ty;
			jv.z = it.tpts[j].z; // Only scale in x- and y- axis.
			jv.setColor(it.tpts[j].r, it.tpts[j].g, it.tpts[j].b);
			ivs[j] = jv;
		}
		ts[i].tpts.clear();
		ts[i].tpts = ivs;
 
		//cout << ts[i] << endl;
	} 
	// Start to draw the triangle.
	// By iterating all pts from the boundbox of ith triangle.
	// Use barycentric coordinate to determine whether the pts are within the ith triangle.
	// Only set the rgb color for the pts within the ith triangle.
	for(size_t i = 0; i < ts.size(); i++){
		Boundbox3D ibox = computeBoundbox(ts[i]);
		for(size_t x = ibox.xmin; x < ibox.xmax; x++){
			for(size_t y = ibox.ymin; y < ibox.ymax; y++){
				//cout << x << " " << y << endl;
				float u1, u2, u3;
				computeBaryCoor(ts[i], static_cast<float>(x), static_cast<float>(y), u1, u2, u3);
				//cout << u1 << " " << u2 << " " << u3 << endl;
				if(u1 >= 0 && u2 >= 0 && u3 >= 0){ // The pt within the ith triangle.
					unsigned char r = u1 * ts[i].tpts[0].r + u2 * ts[i].tpts[1].r + u3 * ts[i].tpts[2].r;
					unsigned char g = u1 * ts[i].tpts[0].g + u2 * ts[i].tpts[1].g + u3 * ts[i].tpts[2].g;
					unsigned char b = u1 * ts[i].tpts[0].b + u2 * ts[i].tpts[1].b + u3 * ts[i].tpts[2].b;
					//cout << r << " " << g << " " << b << endl;
					//cout << static_cast<unsigned>(r) << " " << static_cast<unsigned>(g) << " " << static_cast<unsigned>(b) << endl;
					//z  coordinate used with z buffering.
					float z = u1 * ts[i].tpts[0].z + u2 * ts[i].tpts[1].z + u3 * ts[i].tpts[2].z;
					if (z > -100000000) {//if the z value is larger than what is in the buffer
						image->setPixel(x, y, r, g, b);//set the pixel
					}
				}
			}		
		}
			
	}
	image->writeToFile(outFile);
}
