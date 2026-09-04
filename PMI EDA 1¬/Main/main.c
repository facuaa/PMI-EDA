#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//registro del padron electoral de San Luis

/*
--------Lista Vinculada Ordenada con terminacion dada por contenido (+ infinito) (LVO).
--------Lista Secuencial Ordenada con busqueda binaria (LSOBB).
--------Arbol Binario de Busqueda (ABB).
*/


// Definir nupla  electorr
typedef struct {
    int DNI;
    char Nombre_Apellido[50];
    char Domicilio[80];
    int Cod_Postal;
    int Mesa;
    int Circuito;
} Elector;

//Definimos estrucitura para LVO
typedef struct{
    Elector VIPD;
   struct nodo_lvo* PS;
} nodo_lvo;


//Definimo estructura ABB
typedef struct{
    Elector VIPD;
    struct nodo_abb* P_izq;
    struct nodo_abb* P_der;
} nodo_abb;

//Por ultimo LSOBB en el main, es un arreglo mas

// Función mayus
void mayusc(char* cadena) {
    int i = 0;
    while (cadena[i] != '\0') {
        cadena[i] = toupper(cadena[i]);
        i++;
    }
}

int main(){
    Elector LSOBB[2200]; //apropocito le di de mas, por si se carga un archivo con 2003 personas, asi no revienta

    //inicializamos ambos puntos a Null, despues hay que agregar el centinela a lvo
    nodo_lvo Acc_lvo=Null;
    nodo_abb Raiz_abb=Null;

    //=========================================================================
    //=============================leer el archivo=============================
    int codigo_Operacion;
    Elector temp;
    FILE *archivo = fopen("Operaciones_Padron.txt", "r");

    if(archivo == Null){
        printf("Error: No se pudo abrir el archivo:Operaciones_Padron.txt");
        return -1;
    }

    while(fscanf(archivo,"%d",&codigo_Operacion) != EOF){
        fscanf(archivo, "%d", &temp.DNI);
        // Leemos nombre y direccion, permite espacios
        fscanf(archivo, " %[^\n]", temp.Nombre_Apellido);
        mayusc(temp.Nombre_Apellido);

        fscanf(archivo, " %[^\n]", temp.Domicilio);
        mayusc(temp.Domicilio);

        fscanf(archivo, "%d", &temp.Cod_Postal);
        fscanf(archivo, "%d", &temp.Mesa);
        fscanf(archivo, "%d", &temp.Circuito);






        if (cod_operacion == 1) {
            // Llamada a función ALTA
        } else if (cod_operacion == 2) {
            // Llamada a función BAJA
        } else if (cod_operacion == 3) {
            // Llamada a función EVOCACIÓN
        }

    }
    fclose(archivo);


    //menu
    /*
    1. Comparación de estructuras.
    2. Mostrar Estructura LVO.
    3. Mostrar Estructura LSOBB.
    4. Mostrar Estructura ABB.
    */


}
