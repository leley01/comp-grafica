#include "rasterization.h"
#include "algorithm"
std::vector<Pixel> simple(Pixel p0, Pixel p1){
	std::vector<Pixel> out;
	int x0 = p0.x, y0 = p0.y;
	int x1 = p1.x, y1 = p1.y;

	float m = float(y1 - y0)/(x1 - x0);
	float b = y0 - m*x0;
	for(int x=x0; x <= x1; x++){
		int y = round(m*x + b);
		out.push_back({x, y});
	}
	return out;
}

std::vector<Pixel> dda(Pixel p0, Pixel p1){
	std::vector<Pixel> out;
	int x0 = p0.x, y0 = p0.y;
	int x1 = p1.x, y1 = p1.y;

	float dx = (x1-x0);
	float dy = (y1-y0);

	float delta = std::max(abs(dx), abs(dy));

	dx /= delta;
	dy /= delta;

	float x = x0;
	float y = y0;

	for(int i = 0; i <= delta; i++){
		out.push_back({(int)round(x), (int)round(y)});
		x += dx;
		y += dy;
	}
	return out;
}

std::vector<Pixel> bres(Pixel P){
	int dx = P.x;
	int dy = P.y;

	std::vector<Pixel> out;

	if(dy > dx){
		out = bres({dy, dx});
		for(Pixel& p: out)
			std::swap(p.x, p.y);
		return out;
	}

	out.reserve(dx);

	int D = 2*dy - dx;
	int y = 0;
	for(int x = 0; x <= dx; x++){
		out.push_back({x, y});
		if(D > 0){
			D -= 2*dx;
			y++;
		}
		D += 2*dy;
	}
	return out;
}

std::vector<Pixel> bresenham(Pixel p0, Pixel p1){
	int x0 = p0.x, y0 = p0.y;
	int x1 = p1.x, y1 = p1.y;

	if(x0 > x1){
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	int dx = x1 - x0;
	int dy = y1 - y0;

	std::vector<Pixel> out = bres({dx, abs(dy)});

	if(dy >= 0)
		for(Pixel& p: out)
			p = {x0 + p.x, y0 + p.y};
	else
		for(Pixel& p: out)
			p = {x0 + p.x, y0 - p.y};

	return out;
}

std::vector<Pixel> rasterizeCircle(Pixel C, int R){
	std::vector<Pixel> out;
	int y = R;
    int d = 1-R;
    for(int x = 0; x<=y; ++x){
        out.push_back({C.x + x, C.y + y});
        out.push_back({C.x + x, C.y - y});
        out.push_back({C.x - x, C.y + y});
        out.push_back({C.x - x, C.y - y});
        out.push_back({C.x + y, C.y + x});
        out.push_back({C.x + y, C.y - x});
        out.push_back({C.x - y, C.y + x});
        out.push_back({C.x - y, C.y - x});


        if(d > 0){
            d += 2*x-2*y+5;
            y--;
        }else
            d += 2*x + 3;
    }
    return out;
}

std::vector<Pixel> rasterizeTriangle(std::array<vec2, 3> P){
	std::vector<Pixel> out;

        float maxy,miny;
        //Pega os valores máximos e minimos da imagem
        maxy = floor(std::max(P[0].y,std::max(P[1].y,P[2].y)));
        miny = ceil(std::min(P[0].y,std::min(P[1].y,P[2].y)));

        std::vector<vec4> valido;
        std::vector<vec4> nvalido;

        //coloco em um vetor(valido) o par de vetores validos e os nao validos(nvalidos)
        for(int i=0;i<3;i++){
                int j=i+1;
                if(i==2){
                    j=0;
                }
                if(fabs(P[i].x - P[j].x)< 1e-5|| fabs (P[i].y - P[j].y)<1e-5){
                    nvalido.push_back({P[i].x,P[i].y,P[j].x,P[j].y});
                }else{
                    valido.push_back({P[i].x,P[i].y,P[j].x,P[j].y});
                }

        }
        double m[3],b[3];
        for(unsigned int k=0;k<valido.size();k++){
            m[k]=(double)(valido[k].w-valido[k].y)/(valido[k].z-valido[k].x);
            b[k]=(double)valido[k].w-(m[k]*valido[k].z);
        }
        std::vector<float> pos;
        float x[3];
        for(int y = miny; y<=maxy; y++){
            pos.clear();
            for(unsigned int l=0;l<valido.size();l++){
                x[l]=(float)(y-b[l])/m[l];
                if(x[l]<= std::max(valido[l].x,valido[l].z) && x[l]>= std::min(valido[l].x,valido[l].z)){
                    pos.push_back(x[l]);
                }
            }
            if(nvalido.size()>0){
                int m=0;
                for(int n=nvalido.size();n>0;n--,m++){
                    x[3-n]=nvalido[m].x;
                    if(y >= ceil(std::min(nvalido[m].y,nvalido[m].w)) && y <= floor(std::max(nvalido[m].y,nvalido[m].w))){
                        pos.push_back(nvalido[m].x);
                    }
                }
            }
            if(pos.size()==2){
                for(int x=ceil(std::min(pos[0],pos[1]));x<=floor(std::max(pos[0],pos[1]));x++){
                    out.push_back({x,y});
                }
            }else if(pos.size()==3){
                for(int x=ceil(std::min(pos[0],std::min(pos[1],pos[2])));x<=floor(std::max(pos[0],std::max(pos[1],pos[2])));x++){
                    out.push_back({x,y});
                }
            }
        }
	return out;
}
