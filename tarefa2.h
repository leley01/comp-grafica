#include "math.h"
#include"Image.h"

int distancia(int x, int y){
    return sqrt((x-300)*(x-300)+(y-300)*(y-300));
}

void desenhaAlvo(){
    Image img(600, 600);
    for(int y = 0; y < 600; y++){
        for(int x = 0; x < 600; x++){
            int dist = distancia(x,y);
            if(dist%120<60){
                img(x,y)=black;
            }else{
                img(x,y)=white;
            }
        }
    }
    img.savePNG("Alvo.png");
}

void desenhaXadrez(){
    Image img(600, 600);
    for(int y = 0; y < 600; y++){
        for(int x = 0; x < 600; x++){
            if(x%150<75 && y%150<75){
                img(x,y)=black;
            }else{
                if(x%150>=75 && y%150>=75){
                    img(x,y)=black;
                }else{
                    img(x,y)=white;
                }
            }
        }
    }

    img.savePNG("Xadrez.png");
}

void interpolacaoBilinear(){
    Image img(600,600);
    Color
        A = {255, 0, 0},
        B = {0, 255, 0},
        C = {0, 0, 255},
        D = {255, 0, 255};

    float u,v;

    for(int y = 0; y < 600; y++){
        for(int x = 0; x < 600; x++){
            u=(float)x/600;
            v=(float)y/600;
            img(x,y)= bilerp(u,v,D,A,B,C);
        }
    }
    img.savePNG("teste.png");
}
