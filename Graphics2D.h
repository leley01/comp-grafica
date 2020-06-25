#ifndef GRAPHICS2D_H
#define GRAPHICS2D_H

#include <vector>
#include "Image.h"
#include "Primitives.h"
#include "rasterization.h"
#include "vec.h"
#include "geometry.h"

struct Semiplane{
        vec2 A;
        vec2 n;

        bool has(vec2 P){
            return dot(P-A, n) >= 0;
        }

    float intersect(vec2 P, vec2 Q){
            return dot(A-P, n)/dot(Q-P,n);
        }
    };


class Graphics2D{
	Image img;

	public:
	Graphics2D(int w, int h) : img{w, h}{}

	void clear(Color col = white){
		img.fill(col);
	}

	void savePNG(const char* filename){
		img.savePNG(filename);
	}

	void paint(Pixel p, Color c){
        if(p.x >= 0 && p.y >= 0 && p.x < img.width && p.y < img.height)
            img(p.x,p.y) = c;
    }

	template<class Prims>
    void draw(
        const std::vector<vec2>& V,
        const Prims& prims,
        Color col
    ){
        std::vector<Vec2Col> PV;
        for(vec2 pos: V)
            PV.push_back({pos, col});

        draw(PV, prims);
    }


    template<class Prims>
    void draw(const std::vector<Vec2Col>& V, const Prims& P){
        for(size_t i = 0; i < P.size(); i++)
            draw_primitive(assemble(V, P[i]));
    }
	private:

    void draw_primitive(Line<Vec2Col> line){
        if(clip(line)){
            return;
        }
        vec2 v0 = line[0].position, v1 = line[1].position;
        Color c0 = line[0].color, c1 = line[1].color;

        Pixel p0 = {(int)round(v0.x), (int)round(v0.y)};
        Pixel p1 = {(int)round(v1.x), (int)round(v1.y)};

        for(Pixel p: bresenham(p0, p1)){
            vec2 v = {(float)p.x, (float)p.y};
            float t = findMixParam(v, v0, v1);
            Color color = lerp(t, c0, c1);
            paint(p, color);
        };
    }


	 void draw_primitive(Triangle<Vec2Col> tri){
	     if(clip(tri)){
            return;
	     }
	    vec2 v0 = tri[0].position, v1 = tri[1].position, v2 = tri[2].position;
        Color c0 = tri[0].color, c1 = tri[1].color, c2 =tri[2].color;

        std::array<vec2,3> pontos = {v0,v1,v2};
        for(Pixel p: rasterizeTriangle(pontos)){
            vec2 v = {(float)p.x, (float)p.y};
            std::array<float,3> bar= barycentric(v,pontos);
            Color color = toColor(bar[0]*toVec3(c0)+bar[1]*toVec3(c1)+bar[2]*toVec3(c2));
            paint(p, color);
        };
	}

	bool clip(Line<Vec2Col>& line){
        int N=4;
        float t_in ,t_out ;
        float xmin=30, xmax=img.width-30,ymin=30,ymax=img.height-30;
        std::vector <float> p;
        std::vector <float> q;
        t_in = 0;
        t_out = 1;
        vec2 a=line[0].position;
        vec2 b=line[1].position;
        Color ca=line[0].color;
        Color cb=line[1].color;

        ////Cálculo dos valores de P e Q
        p.push_back((b.x-a.x)*-1);
        p.push_back((b.x-a.x));
        p.push_back((b.y-a.y)*-1);
        p.push_back((b.y-a.y));

        q.push_back(a.x-xmin);
        q.push_back(xmax-a.x);
        q.push_back(a.y-ymin);
        q.push_back(ymax-a.y);
        ////////////////////////////////////

        for(int i=0;i<N;i++){
            if(p[i]<0){
                t_in=std::max(t_in,q[i]/p[i]);
            }else if(p[i]>0){
                t_out=std::min(t_out,q[i]/p[i]);
            }else if(q[i]<0){
                return true;
            }
        }
        if(t_in>t_out){
            return true;
        }else{
            line[0].position=lerp(t_in,a,b);
            line[0].color=lerp(t_in,ca,cb);
            line[1].position=lerp(t_out,a,b);
            line[1].color=lerp(t_out,ca,cb);
        }
        return false;
    }

    bool clip(Triangle<Vec2Col>& tri){
        float xmin = 30-0.5, ymin = 30-0.5;
        float xmax = img.width-29.5, ymax = img.height-25.9;

        Semiplane walls[] = {
            {{xmin, ymin}, {1, 0}}, // left
            {{xmin, ymin}, {0, 1}}, // down
            {{xmax, ymax}, {-1,0}}, // right
            {{xmax, ymax}, {0,-1}}, // top
        };
        for(Semiplane wall: walls)
            if(clip(tri, wall))
                return true;

        return false;
    }

    bool clip(Triangle<Vec2Col>& tri, Semiplane S){
        std::vector<Vec2Col>in;
        std::vector<Vec2Col>out;
        for(int i=0;i<3;i++){
            if(S.has(tri[i].position)){
                in.push_back(tri[i]);
            }else{
                out.push_back(tri[i]);
            }
        }
        if(in.size()==3){
            return false;
        }else if(out.size()==3){
            return true;
        }
        std::vector<Vec2Col> L;
        L.clear();
        if(in.size()==1){
            L.push_back(in[0]);
            float t1=0,t2=0;
            Vec2Col aux1,aux2;
            t1=S.intersect(in[0].position,out[0].position);
            t2=S.intersect(in[0].position,out[1].position);
            aux1.position=(lerp(t1,in[0].position,out[0].position));
            aux2.position=(lerp(t2,in[0].position,out[1].position));
            aux1.color=(lerp(t1,in[0].color,out[0].color));
            aux2.color=(lerp(t2,in[0].color,out[1].color));
            L.push_back(aux1);
            L.push_back(aux2);
        }else{
            L.push_back(in[1]);
            L.push_back(in[0]);
            float t1=0,t2=0;
            Vec2Col aux1,aux2;
            t1=S.intersect(in[0].position,out[0].position);
            t2=S.intersect(in[1].position,out[0].position);
            aux1.position=(lerp(t1,in[0].position,out[0].position));
            aux2.position=(lerp(t2,in[1].position,out[0].position));
            aux1.color=(lerp(t1,in[0].color,out[0].color));
            aux2.color=(lerp(t2,in[1].color,out[0].color));
            L.push_back(aux1);
            L.push_back(aux2);
        }
    tri = {L[0], L[1], L[2]};
    if(L.size()==4){
        Triangle<Vec2Col> ntri = {L[0], L[2], L[3]};
        draw_primitive(ntri);
    }
    return false;
    }

};

#endif
