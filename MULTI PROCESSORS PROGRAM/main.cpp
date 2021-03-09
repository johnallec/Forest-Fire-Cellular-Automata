#include <allegro5/allegro.h>
#include <time.h>
#include <iostream>
#include <mpi.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
using namespace std;
#define COLOR8 al_map_rgb(252, 107, 3)
#define COLOR9 al_map_rgb(252, 202, 3)
#define RED al_map_rgb(255,0,0)
#define BLACK al_map_rgb(0,0,0)
#define GREEN al_map_rgb(0,255,0)

#define dim 500

bool generaAlbero(int);
bool generaFulmine(int);
bool alberoVicinoInFiamme( int , int , int , int [][dim], int [dim] , int [dim]);
void print();

int main(int argc, char **argv) {
    srand(time(NULL));


    int WORLD[dim][dim];    
    int tempWorld[dim][dim];
    
    int sizeWorld, rank;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &sizeWorld);

    ALLEGRO_DISPLAY* display;
    ALLEGRO_BITMAP * alive;
    ALLEGRO_BITMAP * dead;
    ALLEGRO_BITMAP * empty;
    ALLEGRO_BITMAP * burning;
    ALLEGRO_BITMAP * burning8;
    ALLEGRO_BITMAP * burning9;


    if(rank == 0){
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

        for (int i=0;i<dim;i++) {
            for (int j=0;j<dim;j++){
                WORLD[i][j]=0;
                tempWorld[i][j]=0;
            }
        }  

        
    }
    

    int myDim = dim / sizeWorld;


    int mySubset[myDim][dim];
    int tempSubset[myDim][dim];
    int currentSubset[myDim][dim];
    int newSubset[myDim][dim];
    int upVector[dim];
    int downVector[dim];
    int timeStart;
    int timeEnd; 
    MPI_Scatter(WORLD, myDim*dim, MPI_INT, mySubset, myDim*dim, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    if(rank == 0){
        timeStart = MPI_Wtime();
    }

  
    for(int generazione = 0; generazione<1000; generazione++){

    

    if(rank == 0){
        MPI_Send(mySubset[0], dim, MPI_INT, sizeWorld-1, 8, MPI_COMM_WORLD);
        MPI_Send(mySubset[myDim-1], dim, MPI_INT, rank+1, 8, MPI_COMM_WORLD);
        MPI_Recv(upVector, dim, MPI_INT, sizeWorld-1, 8, MPI_COMM_WORLD, &status);
        MPI_Recv(downVector, dim, MPI_INT, rank+1, 8, MPI_COMM_WORLD, &status);
    }
    else if(rank == sizeWorld-1){
        MPI_Send(mySubset[0], dim, MPI_INT, rank-1, 8, MPI_COMM_WORLD);
        MPI_Send(mySubset[myDim-1], dim, MPI_INT, 0, 8, MPI_COMM_WORLD);
        MPI_Recv(upVector, dim, MPI_INT, rank-1, 8, MPI_COMM_WORLD, &status);
        MPI_Recv(downVector, dim, MPI_INT, 0, 8, MPI_COMM_WORLD, &status);
    }
    else {   
        MPI_Send(mySubset[0], dim, MPI_INT, rank-1, 8, MPI_COMM_WORLD);
        MPI_Send(mySubset[myDim-1], dim, MPI_INT, rank+1, 8, MPI_COMM_WORLD);
        MPI_Recv(upVector, dim, MPI_INT, rank-1, 8, MPI_COMM_WORLD, &status);
        MPI_Recv(downVector, dim, MPI_INT, rank+1, 8, MPI_COMM_WORLD, &status);
    }

    MPI_Barrier(MPI_COMM_WORLD);



    for(int i=0; i<myDim; i++){
        for(int j=0; j<dim; j++){
            tempSubset[i][j] = mySubset[i][j];
        }
    }
    


    for (int i = 0; i < myDim; i++) {
        for (int j = 0; j < dim; j++) {
            if(tempSubset[i][j]== 0){
                if(generaAlbero(rank))
                    currentSubset[i][j] = 1;
                else
                    currentSubset[i][j] = tempSubset[i][j];
            }
            else if(tempSubset[i][j]==1){
                if(alberoVicinoInFiamme( i, j, myDim, tempSubset, upVector, downVector) || generaFulmine(rank))
                        currentSubset[i][j]= 2;
                else
                    currentSubset[i][j] = tempSubset[i][j];
            }
            else if(tempSubset[i][j]==2)
                        currentSubset[i][j]=8;

            else if(tempSubset[i][j]==8)
                        currentSubset[i][j]=9;

            else if(tempSubset[i][j]==9)
                        currentSubset[i][j]=0;
        }
    }
    
        
 

   for(int i=0; i<myDim; i++){
        for(int j=0; j<dim; j++){
            mySubset[i][j] = currentSubset[i][j];
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    

    MPI_Gather(currentSubset, myDim*dim, MPI_INT, tempWorld, myDim*dim, MPI_INT, 0, MPI_COMM_WORLD);
    

    MPI_Barrier(MPI_COMM_WORLD);

    if(rank == 0){
        for(int k=0; k<dim; k++){
            for(int t=0; t<dim; t++){
                WORLD[k][t]=tempWorld[k][t];
            }
        }
        for(int i=0; i<dim; i++){
            for(int j=0; j<dim; j++){
                    if(WORLD[i][j] == 1){
                        al_draw_bitmap(alive,j*2,i*2,0);
                    }
                    if(WORLD[i][j] == 2){
                        al_draw_bitmap(burning,j*2,i*2,0);
                    }
                    if(WORLD[i][j] == 8){
                        al_draw_bitmap(burning8,j*2,i*2,0);
                    }
                    if(WORLD[i][j] == 9){
                        al_draw_bitmap(burning9,j*2,i*2,0);
                    }
                    if(WORLD[i][j] == 0){
                        al_draw_bitmap(empty,j*2,i*2,0);
                    }
            }

        }
        al_flip_display();   
         
    }

    MPI_Barrier(MPI_COMM_WORLD);


    }
    
    if(rank==0){
        timeEnd = MPI_Wtime();

        printf("Il tempo impiegato è: %d", timeEnd-timeStart);
    }

    MPI_Finalize();
    exit(0);
}


bool generaAlbero(int rank){
//se la dimensione della matrice = n*n allora k= sqrt(n) all'incirca
    int k=1000; //1000
    int n = rand()%k+1 + rank;
    if(n == rand()%k+1  || n == rand()%k+1 ) {
        return true;
    } 
    return false; 

}

bool generaFulmine(int rank){

    int k=1000000; //1000000
    int n = rand()%k+1+ rank;

    if(n == rand()%k+1 || n == rand()%k+1){
        return true;
    }
    return false;

}

bool alberoVicinoInFiamme( int i, int j, int myDim, int M[][dim], int upVector[dim], int downVector [dim]){
   
    bool b=false;
    if(i==0){
        if(j==0){
            for(int k=i; k<=i+1; k++){
                for(int h=j; h<=j+1; h++){
                    if(k!=i || h!=j ){
                        if(M[k][h]==2){
                            b=true;
                            return b;
                        }
                    }
                }
            }

            for(int v=j; v<=j+1; v++){
                if(upVector[v]==2){
                    b=true;
                    return b;
                }
            }

        }
        else if(j==dim){

            for(int k=i; k<=i+1; k++){
                for(int h=j-1; h<=j; h++){
                    if(k!=i || h!=j ){
                        if(M[k][h]==2){
                            b=true;
                            return b;
                        }
                    }
                }
            }

            for(int v=j; v<=j+1; v++){
                if(upVector[v]==2){
                    b=true;
                    return b;
                }
            }

        }
        else{

            for(int k=i; k<=i+1; k++){
                for(int h=j-1; h<=j+1; h++){
                    if(k!=i || h!=j ){
                        if(M[k][h]==2){
                            b=true;
                            return b;
                        }
                    }
                }
            }

            for(int v=j; v<=j+1; v++){
                if(upVector[v]==2){
                    b=true;
                    return b;
                }
            }

        }
    }
    else if(i==myDim){
        if(j==0){

            for(int k=i-1; k<=i; k++){
                for(int h=j; h<=j+1; h++){
                    if(k!=i || h!=j ){
                        if(M[k][h]==2){
                            b=true;
                            return b;
                        }
                    }
                }
            }

            for(int v=j; v<=j+1; v++){
                if(downVector[v]==2){
                    b=true;
                    return b;
                }
            }
            
        }
        else if(j==dim){

            for(int k=i-1; k<=i; k++){
                for(int h=j-1; h<=j; h++){
                    if(k!=i || h!=j ){
                        if(M[k][h]==2){
                            b=true;
                            return b;
                        }
                    }
                }
            }

            for(int v=j; v<=j+1; v++){
                if(upVector[v]==2){
                    b=true;
                    return b;
                }
            }
            
        }
        else{

            for(int k=i-1; k<=i; k++){
                for(int h=j-1; h<=j+1; h++){
                    if(k!=i || h!=j ){
                        if(M[k][h]==2){
                            b=true;
                            return b;
                        }
                    }
                }
            }

            for(int v=j; v<=j+1; v++){
                if(upVector[v]==2){
                    b=true;
                    return b;
                }
            }

        }
    }
    else if(j==0){

        for(int k=i-1; k<=i+1; k++){
            for(int h=j; h<=j+1; h++){
                if(k!=i || h!=j ){
                    if(M[k][h]==2){
                        b=true;
                        return b;
                    }
                }
            }
        }
        
    }
    else if(j==dim){

        for(int k=i-1; k<=i+1; k++){
            for(int h=j-1; h<=j; h++){
                if(k!=i || h!=j ){
                    if(M[k][h]==2){
                        b=true;
                        return b;
                    }
                }
            }
        }
        
    }
    else{

        for(int k=i-1; k<=i+1; k++){
            for(int h=j-1; h<=j+1; h++){
                if(k!=i || h!=j ){
                    if(M[k][h]==2){
                        b=true;
                        return b;
                    }
                }
            }
        }

    }   

    return b;

}

