/*
* Hecho por: 
*            Daniel Agud Sorlí, 928236
*            Néstor Blanco Ramos, 926915
*
*/
#include <iostream>
#include <fstream>
#include <unistd.h>
#include "GSenku.hpp"
using namespace std;

// g++ main.cpp GSenku.cpp -o juego

int main(int argc, char* argv[]) {  
    // Pide ficheroTablero, ficheroMovimiento, retardo(nº de milisegundos para esperar entre movimientos), ficheroSalida
    if ( argc == 4 || argc == 5){
        string ficheroTablero = argv[1];
        string ficheroMovimiento = argv[2];
        int retardo = atoi(argv[3]);
        string ficheroSalida = "resultado.txt";     // Para saber si hay fichero de salida o no
        if(argc == 5) {
            ficheroSalida = argv[4];
        }

        // cd "/home/nestea/Programación/programacion2/senku/" && g++ main.cpp -o main && "/home/nestea/Programación/programacion2/senku/"main Tableros/tableroDos.txt Movimientos/movimientosClasicos.txt 1000 resultado.txt

        int filas = 0;
        int cols = 0;

        tpTablero tablero;

        sacarFilasYCols(ficheroTablero, filas, cols);

        tablero.nfils = filas;
        tablero.ncols = cols;

        bool todoOK = true;
        todoOK = inicializarTablero(ficheroTablero,tablero);

        if (!todoOK) {
            cerr << "Error al inicializar el tablero" << endl;
            return 1;
        }

        tpMovimientosValidos movimientos;
        inicializarMovimientosValidos(ficheroMovimiento, movimientos);

        tpListaMovimientos solucionParcial;
        solucionParcial.numMovs = 0;
        for (int i = 0; i < MAXDIM*MAXDIM; i++) {
            solucionParcial.movs[i].origen.x = -1;
            solucionParcial.movs[i].origen.y = -1;
            solucionParcial.movs[i].destino.x = -1;
            solucionParcial.movs[i].destino.y = -1;
        }

        int haySolucion = buscaSolucion(tablero, movimientos, solucionParcial, retardo);

        if (haySolucion == 1) {
            cout << "Solucion!!!" << endl;
        }
        else {
            cout << "No hay solucion" << endl;
        }

        if (ficheroSalida != "x") {
            escribeListaMovimientos(ficheroSalida, solucionParcial);
        }

    }
    else {
        cerr << "Numero de argumentos incorrecto." << endl;
        return 1;
    }



}

/*
   Falta:
   - Implementar la función de salida
   - Imprimir con colores
   - Modularizar todo correctamente
   - MakeFile
   - Comprobar todas las soluciones
 */