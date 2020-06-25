#include "Image.h"
#include "Color.h"
#include "matrix.h"
#include "Graphics2D.h"

Image draw_circle(Image img, int cx, int cy, int raio, Color cor){
    for(Pixel p:rasterizeCircle({cx,cy},raio)){
            img(p.x,p.y)=cor;
        }
    return img;
}

///Tarefa rotação

mat2 calcula(double teta){
    return {cos(teta),-sin(teta),
        sin(teta),cos(teta)};
}

void rotacionaCubo(){
    vector<vec2> P = {
        {0, -1},
        {-1, 0},
        {0, 1},
        {1, 0}
    };

    vector<vec2> aux = {
        {0, -1},
        {-1, 0},
        {0, 1},
        {1, 0}
    };

    vec2 escala={20,20};
    vec2 translacao={300,300};
    double teta=0.175,raio=150.0;


    Graphics2D G(600, 600);
    G.clear();
    for(int k=0;k<36;k++){
        aux=P;
        for(int i=0;i<P.size();i++){
            aux[i]={aux[i].x*escala.x,aux[i].y*escala.y};
            aux[i]=calcula(teta*k)*aux[i];
            vec2 offset={raio*cos(teta*k),raio*sin(teta*k)};
            aux[i]=aux[i]+translacao+offset;
        }
         LineLoop L{aux.size()};
         G.draw(aux, L, blue);
    }
    G.savePNG("output.png");
}
