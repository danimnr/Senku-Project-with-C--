/*
* Hecho por: 
*            Mayo de 2025
*            Daniel Agud Sorlí
*            Néstor Blanco Ramos
*
*/

#include <iostream>
#include <fstream>
#include <unistd.h>
#include "GSenku.hpp"
using namespace std;

// Cuenta las fichas de un tablero
int contarFichas(tpTablero tablero){
    int nFichas = 0;
    for ( int c = 0; c < tablero.nfils; c++ ) {

        for ( int i = 0; i < tablero.ncols; i++ ) {

            if ( tablero.matriz[c][i] == OCUPADA) {

                nFichas++;

            }

        }

    }

    return nFichas;
    
}

// PRE: ∃ ficheroTablero
// POST: (filas ^ cols) valen lo que deberían
void sacarFilasYCols(string ficheroTablero, int &filas, int &cols) {
    // Funciona
    ifstream f(ficheroTablero);
    if (f.is_open()) {
        char aux[3]; // 
        f >> aux;
        filas = atoi(aux);
        f >> aux;
        cols = atoi(aux);
        f.close();

    } else {
        cerr << "Error al abrir el fichero \' ficheroTablero \'" << endl;
        return;
    }
}

// PRE: true
// POST: En la matriz v se guarda el tablero de juego
bool inicializarTablero(const string nombreFichero, tpTablero &tablero) {
    // Funciona
    ifstream f(nombreFichero);
    // La idea es que en la matriz acabe estando el tablero elegido
    if (f.is_open()) {
        string aux;
        getline(f, aux);     // Ignoramos la primera línea
        for (int i = 0; i < tablero.nfils; i++) {
            for (int j = 0; j < tablero.ncols; j++) {
                f >> aux;
                if (aux == "-") {
                    tablero.matriz[i][j] = NO_USADA;
                } else if (aux == "x") {
                    tablero.matriz[i][j] = VACIA;
                } else if (aux == "o") {
                    tablero.matriz[i][j] = OCUPADA;
                } else {
                    cerr << "Error en el formato del fichero" << endl;
                    return false;
                }
            }
        }
        f.close();
        return true;
    }
    else {
        cerr << "Error al abrir el fichero" << endl;
        return false;
    }
}

// Pre: true 
// Post: lee los movimientos válidos del fichero que se le pasa como argumento 
//      inicializando la estructura y devolviendo true si todo ha ido bien y false si ha habido algún error
bool inicializarMovimientosValidos(const string nombreFichero, tpMovimientosValidos &movimientos){
    // Funciona
    ifstream f(nombreFichero);
    for (int i = 0; i < 8; i++) {
        movimientos.validos[i] = false;
    }
    if (f.is_open()) {
        string aux;
        // SUPERIOR IZQD: [0]
        getline(f, aux, ' ');
        if(aux == "+") {
            movimientos.validos[0] = true;
        } 
        // SUPERIOR CENTRAL: [1]
        f >> aux;
        if (aux == "+"){
            movimientos.validos[1] = true;
        }
        // SUPERIOR DCHA: [2]
        f >> aux;
        if(aux == "+") {
            movimientos.validos[2] = true;
        }
        // CENTRAL IZQ: [3]
        f >> aux;
        if(aux == "+") {
            movimientos.validos[3] = true;
        }
        f.ignore();
        f.ignore();
        f.ignore();
        // CENTRAL DCH: [4]
        f >> aux;
        if(aux == "+") {
            movimientos.validos[4] = true;
        }
        // INFERIOR IZQ: [5]
        f >> aux;
        if(aux == "+") {
            movimientos.validos[5] = true;
        }
        // INFERIOR CENTRAL: [6]
        f >> aux;
        if(aux == "+") {
            movimientos.validos[6] = true;
        }
        // INFERIOR DCHA : [7]
        f >> aux;
        if(aux == "+") {
            movimientos.validos[7] = true;
        }
        f.close();
        return true;
    }
    else {
        cerr << "Error al abrir el fichero" << endl;
        return false;
    }
}

// Pre: tablero contiene el estado inicial del que se parte para la búsqueda, 
//      movimientosValidos contiene los movimientos que están permitidos, 
//      solucionParcial contiene la solución actual como lista de movimientos, En el tablero se han colocada las n primeras piezas de vEntrada, en la columnas indicadas respectivamente en vSalida
// Post: solucionParcial contendrá la lista de movimientos completa (si no se llega a una solución, estará vacía, numMovs == 0)
//       Devuelve 1 si encuentra solución, -1 si no la encuentra.
int buscaSolucion(tpTablero &tablero, const tpMovimientosValidos &movValidos, tpListaMovimientos &solucionParcial, const int retardo){  
    // Sleep
    sleep( retardo / 1000 );
    if (retardo > 0) {
        mostrarTablero(tablero);
    }
    //BUSCAMOS DONDE HAY PIEZA, Y PARA CADA PIEZA
    // Filas
    for (int i = 0; i < tablero.nfils; i++) {
        // Columnas
        for(int j = 0; j < tablero.ncols; j++){
            // SI NO HAY MAS QUE UNA FICHA TERMINAMOS
            if (contarFichas(tablero) == 1) {
                // escribeListaMovimientos("resultado.txt", solucionParcial);
                return 1;
            }

            //SI HAY UNA FICHA MOVEMOS LA FICHA
            if (tablero.matriz[i][j] == OCUPADA) {
                // Buscamos movimientos válidos 
                // SUPERIOR IZQD: [0]
                if( movValidos.validos[0] ) {

                    if ( (tablero.matriz[i-2][j-2] == VACIA ) && (tablero.matriz[i-1][j-1] == OCUPADA)){
                            
                        tablero.matriz[i-2][j-2] = OCUPADA; 
                        tablero.matriz[i][j] = VACIA;   // Mover    
                        tablero.matriz[i-1][j-1] = VACIA; // Comer
                        
                        solucionParcial.movs[solucionParcial.numMovs].origen.x = i;
                        solucionParcial.movs[solucionParcial.numMovs].origen.y = j;
                        solucionParcial.movs[solucionParcial.numMovs].destino.x = i-2;
                        solucionParcial.movs[solucionParcial.numMovs].destino.y = j-2;
                        solucionParcial.numMovs++;
                        
                        buscaSolucion(tablero,movValidos,solucionParcial,retardo);
                        if (contarFichas(tablero) == 1) { //fin
                            return 1;
                        }
                        solucionParcial.numMovs--;
                        tablero.matriz[i-2][j-2] = VACIA;   // Saltar
                        tablero.matriz[i][j] = OCUPADA;           // Mover    
                        tablero.matriz[i-1][j-1] = OCUPADA;     // Comer
                                            
                    } 
                
                }
                // SUPERIOR CENTRAL: [1]
                if( movValidos.validos[1]){
                        
                    if ( (tablero.matriz[i - 2][j] == VACIA) && (tablero.matriz[i - 1][j] == OCUPADA)){

                        tablero.matriz[i-2][j] = OCUPADA; 
                        tablero.matriz[i][j] = VACIA;   // Mover    
                        tablero.matriz[i-1][j] = VACIA; // Comer


                        solucionParcial.movs[solucionParcial.numMovs].origen.x = i;
                        solucionParcial.movs[solucionParcial.numMovs].origen.y = j;
                        solucionParcial.movs[solucionParcial.numMovs].destino.x = i-2;
                        solucionParcial.movs[solucionParcial.numMovs].destino.y = j;
                        solucionParcial.numMovs++;

                        buscaSolucion(tablero,movValidos,solucionParcial,retardo);
                        if (contarFichas(tablero) == 1) {
                            return 1;
                        }
                        solucionParcial.numMovs--;
                        tablero.matriz[i-2][j] = VACIA; 
                        tablero.matriz[i][j] = OCUPADA;   // Mover    
                        tablero.matriz[i-1][j] = OCUPADA; // Comer
            
                    } 

                }
                    
                // SUPERIOR DERECHA: [2]
                if ( movValidos.validos[2] ) {

                    if ( (tablero.matriz[i - 2][j + 2] == VACIA) && (tablero.matriz[i-1][j+1] == OCUPADA)){
                    
                        tablero.matriz[i-2][j+2] = OCUPADA; 
                        tablero.matriz[i][j] = VACIA;   // Mover    
                        tablero.matriz[i-1][j+1] = VACIA; // Comer

                        solucionParcial.movs[solucionParcial.numMovs].origen.x = i;
                        solucionParcial.movs[solucionParcial.numMovs].origen.y = j;
                        solucionParcial.movs[solucionParcial.numMovs].destino.x = i-2;
                        solucionParcial.movs[solucionParcial.numMovs].destino.y = j+2;
                        solucionParcial.numMovs++;

                        buscaSolucion(tablero,movValidos,solucionParcial,retardo);
                        if (contarFichas(tablero) == 1) {
                            return 1;
                        }
                        solucionParcial.numMovs--;
                        tablero.matriz[i-2][j+2] = VACIA; 
                        tablero.matriz[i][j] = OCUPADA;   // Mover    
                        tablero.matriz[i-1][j+1] = OCUPADA; // Comer
                                                
                    } 
                    
                }
                // CENTRAL IZQ: [3]
                if( movValidos.validos[3]){
                            
                    if ( (tablero.matriz[i][j - 2] == VACIA) && (tablero.matriz[i][j-1] == OCUPADA)){
                                
                        tablero.matriz[i][j-2] = OCUPADA; //Saltar
                        tablero.matriz[i][j] = VACIA;   // Mover    
                        tablero.matriz[i][j-1] = VACIA; // Comer

                        solucionParcial.movs[solucionParcial.numMovs].origen.x = i;
                        solucionParcial.movs[solucionParcial.numMovs].origen.y = j;
                        solucionParcial.movs[solucionParcial.numMovs].destino.x = i;
                        solucionParcial.movs[solucionParcial.numMovs].destino.y = j-2;
                        solucionParcial.numMovs++;

                        buscaSolucion(tablero,movValidos,solucionParcial,retardo);
                        if (contarFichas(tablero) == 1) {
                            return 1;
                        }

                        solucionParcial.numMovs--;
                        tablero.matriz[i][j-2] = VACIA; //Saltar
                        tablero.matriz[i][j] = OCUPADA;   // Mover    
                        tablero.matriz[i][j-1] = OCUPADA; // Comer
                                            
                    } 

                }
                // CENTRAL DCH: [4]
                if( movValidos.validos[4]){
                            
                    if ( (tablero.matriz[i][j + 2] == VACIA) && (tablero.matriz[i][j+1] == OCUPADA)){
                               
                        tablero.matriz[i][j+2] = OCUPADA;   // Saltar
                        tablero.matriz[i][j] = VACIA;       // Mover    
                        tablero.matriz[i][j+1] = VACIA;     // Comer

                        
                        solucionParcial.movs[solucionParcial.numMovs].origen.x = i;
                        solucionParcial.movs[solucionParcial.numMovs].origen.y = j;
                        solucionParcial.movs[solucionParcial.numMovs].destino.x = i;
                        solucionParcial.movs[solucionParcial.numMovs].destino.y = j+2;
                        solucionParcial.numMovs++;

                        buscaSolucion(tablero,movValidos,solucionParcial,retardo);
                        if (contarFichas(tablero) == 1) {
                            return 1;
                        }
                        solucionParcial.numMovs--;
                        tablero.matriz[i][j+2] = VACIA;   // Saltar
                        tablero.matriz[i][j] = OCUPADA;       // Mover    
                        tablero.matriz[i][j+1] = OCUPADA;     // Comer
    
                    } 
                            
                }
                // INFERIOR IZQ: [5]
                if( movValidos.validos[5]){
                            
                    if ( (tablero.matriz[i + 2][j - 2] == VACIA) && (tablero.matriz[i+1][j-1] == OCUPADA)){
                    
                        tablero.matriz[i+2][j-2] = OCUPADA;   // Saltar
                        tablero.matriz[i][j] = VACIA;       // Mover    
                        tablero.matriz[i+1][j-1] = VACIA;     // Comer

                        solucionParcial.movs[solucionParcial.numMovs].origen.x = i;
                        solucionParcial.movs[solucionParcial.numMovs].origen.y = j;
                        solucionParcial.movs[solucionParcial.numMovs].destino.x = i+2;
                        solucionParcial.movs[solucionParcial.numMovs].destino.y = j-2;
                        solucionParcial.numMovs++;

                        buscaSolucion(tablero,movValidos,solucionParcial,retardo);
                        if (contarFichas(tablero) == 1) {
                            return 1;
                        }
                        solucionParcial.numMovs--;
                        tablero.matriz[i+2][j-2] = VACIA;       // Saltar
                        tablero.matriz[i][j] = OCUPADA;         // Mover    
                        tablero.matriz[i+1][j-1] = OCUPADA;     // Comer
                                
                    } 

                }
                // INFERIOR CENTRAL: [6]
                if ( movValidos.validos[6] ) {

                    if ( (tablero.matriz[i + 2][j] == VACIA) && (tablero.matriz[i+1][j] == OCUPADA) ){
                           
                        tablero.matriz[i+2][j] = OCUPADA;   // Saltar
                        tablero.matriz[i][j] = VACIA;       // Mover    
                        tablero.matriz[i+1][j] = VACIA;     // Comer

                        solucionParcial.movs[solucionParcial.numMovs].origen.x = i;
                        solucionParcial.movs[solucionParcial.numMovs].origen.y = j;
                        solucionParcial.movs[solucionParcial.numMovs].destino.x = i+2;
                        solucionParcial.movs[solucionParcial.numMovs].destino.y = j;
                        solucionParcial.numMovs++;


                        buscaSolucion(tablero,movValidos,solucionParcial,retardo);
                        if (contarFichas(tablero) == 1) {
                            return 1;
                        }
                        solucionParcial.numMovs--;
                        tablero.matriz[i+2][j] = VACIA;   // Saltar
                        tablero.matriz[i][j] = OCUPADA;       // Mover    
                        tablero.matriz[i+1][j] = OCUPADA;     // Comer
                                
                    } 
                    
                }
                // INFERIOR DCHA : [7]
                if ( movValidos.validos[7] ) {

                    if ( (tablero.matriz[i + 2][j + 2] == VACIA) && (tablero.matriz[i+1][j+1] == OCUPADA)){
                    
                        tablero.matriz[i+2][j+2] = OCUPADA;   // Saltar
                        tablero.matriz[i][j] = VACIA;           // Mover    
                        tablero.matriz[i+1][j+1] = VACIA;     // Comer
                        
                        solucionParcial.movs[solucionParcial.numMovs].origen.x = i;
                        solucionParcial.movs[solucionParcial.numMovs].origen.y = j;
                        solucionParcial.movs[solucionParcial.numMovs].destino.x = i+2;
                        solucionParcial.movs[solucionParcial.numMovs].destino.y = j+2;
                        solucionParcial.numMovs++;

                        buscaSolucion(tablero,movValidos,solucionParcial,retardo);
                        if (contarFichas(tablero) == 1) {
                            return 1;
                        }
                        solucionParcial.numMovs--;
                        tablero.matriz[i+2][j+2] = VACIA;   // Saltar
                        tablero.matriz[i][j] = OCUPADA;           // Mover    
                        tablero.matriz[i+1][j+1] = OCUPADA;     // Comer
                                
                    } 
                    
                }
                    
            }

        }
    }
    return -1;
}


// Muestra el tablero por pantalla
void mostrarTablero(const tpTablero &tablero) {
    cout << endl;
    for ( int i = 0; i < tablero.nfils; i++ ) {
        for ( int c = 0; c < tablero.ncols; c++ ) {
        // escribe en azul con fondo verde, y después resetea los atributos         
            if(tablero.matriz[i][c] == NO_USADA) { //FONDO ROJO, COLOR NEGRO
                cout << "\033[30;41m";
                cout << '-';
                cout << "\033[0m";
            } else if (tablero.matriz[i][c] == OCUPADA) { //FONDO VERDE, COLOR NEGRO
                cout << "\033[30;42m";
                cout << 'o';
                cout << "\033[0m";
            } else if (tablero.matriz[i][c] == VACIA) { //FONDO AMARILLO, COLOR NEGRO
                cout << "\033[30;44m";
                cout << 'x';
                cout << "\033[0m";
            }
            cout << " ";
        }
        cout << endl;
    }
    cout << endl;
    return;
}

// Pre: listaMovimientos contiene la lista de movimientos con la solucion 
// Post: escribe la lista de movimientos en el fichero que se le pasa como argumento siguiendo el 
//      formato especificado en el guión (si está vacía, se escribe un -1 en el fichero)
void escribeListaMovimientos (string nombreFichero, const tpListaMovimientos &solucion) {
    ofstream f;
    f.open(nombreFichero);
    if (f.is_open()) {
        if (solucion.numMovs == 0) {
            f << -1 << endl;
        } else {
            // Escribimos los movimientos
            for (int i = 0; i < solucion.numMovs; i++) {
                f << solucion.movs[i].origen.x << "," << solucion.movs[i].origen.y;
                f << ":" << solucion.movs[i].destino.x << "," << solucion.movs[i].destino.y;
                f << endl;
            }
        }
        f.close();
    } else {
        cerr << "Error al abrir el fichero" << endl;
    }
}

