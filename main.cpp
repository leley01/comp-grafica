#include "Image.h"
#include "tarefa2.h"
#include "rasterization.h"
#include "tarefa3.h"
#include "Primitives.h"
#include "Graphics2D.h"
#include "stdio.h"
#include "stdlib.h"
#include "bezier.h"
#include "matrix.h"

mat3 calcula3(double teta){
    return {cos(teta), -sin(teta), 0.0,
            sin(teta),  cos(teta), 0.0,
		    0.0,    0.0, 1.0};
}


int main(){
   /* vector<vec2> P = loadCurve("borboleta.txt");
    vector<vec2> Q = sample_cubic_bezier_spline(P, 30);

	vec2 v = {500, 500};

	mat3 T = {
		1.0, 0.0, v.x,
		0.0, 1.0, v.y,
		0.0, 0.0, 1
	};

	mat3 C = {
		1.0, 0.0, 50,
		0.0, 1.0, 50,
		0.0, 0.0, 1
	};

	mat3 Ti = {
		1.0, 0.0, -v.x,
		0.0, 1.0, -v.y,
		0.0, 0.0, 1.0
	};
    int N=16;
	double t = 2*M_PI/N;

	LineStrip L{Q.size()};
	Graphics2D G(1000, 1000);
	G.clear();
	for(int i=0;i<N;i++){
        G.draw((T*(calcula3(t*i))*Ti)*Q, L, blue);
	}
    G.savePNG("circuloborb1.png");*/

   /* vector<Vec2Col> P = {
        {{ 60, 105}, red},
        {{229, 114}, green},
        {{145, 270}, blue},
        {{364, 208}, yellow},
        {{283, 333}, cyan},
        {{471, 298}, orange},
    };

    Triangles T{P.size()};

    Graphics2D G(500, 400);
    G.clear();
    G.draw(P, T);
    G.savePNG("output10.png");*/

  /* vector<Vec2Col> P = {
        {{-26, 68}, red},
        {{76, 145}, green},
        {{40, 22}, blue},
        {{240, 45}, red},
        {{73, 225}, yellow},
        {{-20, 220}, cyan},
        {{114, 143}, green}
    };

    LineStrip L{P.size()};

    Graphics2D G(200, 200);
    G.clear();
    G.draw(P, L);
    G.savePNG("output46.png");*/


    vector<Vec2Col> P = {
        {{36, 116}, red}, {{108, 90}, green}, {{103, 164}, blue},
        {{-34, -37}, red}, {{32, 78}, green}, {{127, 26}, blue},
        {{145, 43}, red}, {{278, 15}, green}, {{125, -82}, blue},
        {{256, 172}, red}, {{271, 69}, green}, {{149, 118}, blue},
        {{185, 213}, red}, {{247, 254}, green}, {{229, 188}, blue},
    };

    Triangles T{P.size()};

    Graphics2D G(200, 200);
    G.clear();
    G.draw(P, T);
    G.savePNG("output101.png");



    return 0;
}
