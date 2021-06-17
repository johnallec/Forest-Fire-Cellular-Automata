# Forest-Fire-Cellular-Automata
University's project consist of Forest Fire's cellular automata. C has been used as the main language and MPI to create a multi-processors application (Allegro library for graphic). 

Su Windows installare il software Xming, avviarlo, andare sulla bash Ubuntu ed incollare la
seguente riga:
"export DISPLAY=localhost:0.0"

Per compilare, inserire la seguente stringa nella bash se si tratta del programma sequenziale:
g++ main.cpp -lallegro -lallegro_primitives -lallegro_image

Per eseguire:
"./a.out"

Questa invece è per compilare il programma parallelizzato:
mpic++ main.cpp -lallegro -lallegro_primitives -lallegro_image

Per eseguire:
"mpirun -np '*' main.cpp" 
//dove '*' indica il numero di processori che si desidera inserire (da 2 a quanti ne supporta
il sistema).


ATTENZIONE:
Modificare a piacere probabilità di generazione albero o fulmine, il numero di generazioni e 
la dimensione della matrice (sconsigliato > 500).

(Sort of) English:

No problem on Linux. On Windows install Xming, run it, go on the shell(Ubuntu) and paste this:
"export DISPLAY=localhost:0.0" (if there ll be problems check how Xming works on Win10)

{	
	To compile mono-thread program paste this:
	g++ main.cpp -lallegro -lallegro_primitives -lallegro_image
	To run:
	"./a.out"
}

{
	To compile multi-threads program past this:
	mpic++ main.cpp -lallegro -lallegro_primitives -lallegro_image
	To run:
	"mpirun -np '*' main.cpp" //where '*' is the number of processors you want to use
}

The program doesn't use dynamic memory, so the max matrix size supported (in most cases) is 500x500.

Enjoy.

