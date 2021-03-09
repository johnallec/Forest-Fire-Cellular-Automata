#include <allegro5/allegro.h>
#include <time.h>
#include <iostream>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
using namespace std;
#define COLOR8 al_map_rgb(252, 107, 3)
#define COLOR9 al_map_rgb(252, 202, 3)
#define RED al_map_rgb(255,0,0)
#define BLACK al_map_rgb(0,0,0)
#define GREEN al_map_rgb(0,255,0)

#define N 500

bool generaAlbero();
bool generaFulmine();
bool alberoVicinoInFiamme(int M[][N], int i, int j);

int main(int argc, char **argv) {
    srand(time(NULL));

    ALLEGRO_DISPLAY* display;
    ALLEGRO_BITMAP * alive;
    ALLEGRO_BITMAP * dead;
    ALLEGRO_BITMAP * empty;
    ALLEGRO_BITMAP * burning;
    ALLEGRO_BITMAP * burning8;
    ALLEGRO_BITMAP * burning9;

    al_init();
    al_init_image_addon();
    display = al_create_display(805,805);
    alive = al_create_bitmap(4,4);
    al_set_target_bitmap(alive);
    al_clear_to_color(GREEN);
    burning = al_create_bitmap(4,4);
    al_set_target_bitmap(burning);
    al_clear_to_color(RED);
    burning8 = al_create_bitmap(4,4);
    al_set_target_bitmap(burning8);
    al_clear_to_color(COLOR8);
    burning9 = al_create_bitmap(4,4);
    al_set_target_bitmap(burning9);
    al_clear_to_color(COLOR9);
    empty = al_create_bitmap(4,4);
    al_set_target_bitmap(empty);
    al_clear_to_color(BLACK);
    al_set_target_bitmap(al_get_backbuffer(display));


    int A[N][N];
    int B[N][N];

    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            A[i][j] = 0;
            B[i][j]=A[i][j];
        }
    }

    

    for(int v=0; v<1000; v++){

       for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if(A[i][j]== 0){
                if(generaAlbero())
                    B[i][j] = 1;
                else
                    B[i][j] = A[i][j];
            }
            else if(A[i][j]==1){
                if(alberoVicinoInFiamme( A, i, j) || generaFulmine())
                        B[i][j]= 2;
                else
                    B[i][j] = A[i][j];
            }
            else if(A[i][j]==2)
                        B[i][j]=8;

            else if(A[i][j]==8)
                        B[i][j]=9;

            else if(A[i][j]==9)
                        B[i][j]=0;
        }
    }
        
        for(int i=0; i<N; i++){
            for(int j=0; j<N; j++){
                A[i][j] = B[i][j];
            }
        }

        for(int i=0; i<N; i++){
            for(int j=0; j<N; j++){
                if(A[i][j] == 1){
                    al_draw_bitmap(alive,j*2,i*2,0);
                }
                if(A[i][j] == 2){
                    al_draw_bitmap(burning,j*2,i*2,0);
                }
                if(A[i][j] == 8){
                    al_draw_bitmap(burning8,j*2,i*2,0);
                }
                if(A[i][j] == 9){
                    al_draw_bitmap(burning9,j*2,i*2,0);
                }
                if(A[i][j] == 0){
                    al_draw_bitmap(empty,j*2,i*2,0);
                }
            }

        }
    al_flip_display(); 

    } 
 

}

bool generaAlbero(){
//se la dimensione della matrice = n*n allora k= sqrt(n) all'incirca
    int k=100;
    int n = rand()%k+1;
    
    if(n == rand()%k+1 || n == rand()%k+1) {
        return true;
    } 
    return false; 

}

bool generaFulmine(){

    int k=100000;
    int n = rand()%k+1;

    if(n == rand()%k+1 || n == rand()%k+1){
        return true;
    }
    return false;

}

bool alberoVicinoInFiamme(int M[][N], int i, int j){

    bool b=false;

    for(int k=i-1; k<=i+1; k++){
        for(int h=j-1; h<=j+1; h++)
            if(k!=i || h!=j ){
                
                if(M[k][h]==2){
                    b=true;
                    return b;
                }
            }
    }

    return b;

}