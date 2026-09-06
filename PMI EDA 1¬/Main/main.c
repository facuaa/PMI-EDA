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

















//===========================================================================================================================
//===========================================================================================================================
//========================================================LVO================================================================
//===========================================================================================================================
//===========================================================================================================================

typedef struct{
    Elector VIPD;
   struct nodo_lvo* PS;
} nodo_lvo;

//LOCALIZAR LVO
//Localizar (in x, out pos, out éxito)
//El valor utilizado como +infinito en LVO es el numero  999.999.999.
void Localizar_LVO(int dni_buscado, nodo_lvo* cabeza, nodo_lvo** pos, int* exito){
    nodo_lvo* aux = cabeza;
    nodo_lvo* anterior = NULL; //Para saber donde insertar dsp.

    while(aux->VIPD.DNI < dni_buscado){
        anterior = aux;
        aux= aux->PS;
    }
    if (aux->VIPD.DNI == dni_buscado) {
        *exito = 1; //EXITO
    } else {
        *exito = 0; //DERROTA
    }
    *pos = ant; //DEVOLVEMOS LA POS PARA EL ALTA
}

//Alta (in x, in y, out éxito)
//doble puntero para cabeza porque si quiero almacenar en el primer elemento me dice que estoy haciend segmental fail
void Alta_LVO(Elector nuevo, nodo_lvo** cabeza, int* exito){
    int dni_busc = nuevo.DNI;
    nodo_lvo* Pos = NULL;
    int ext;
    Localizar_LVO(dni_busc, *cabeza, &Pos, &ext);
    if (ext == 1){
        printf("Error: Ya existe el DNI: %d en LVO\n", dni_busc); //VERRRRRRRRRRR LOS CARTELES VAN EN EL MAIN????????? O NO?
        *exito = 0;
    }else{
        nodo_lvo* nuevo_nodo = (nodo_lvo*)malloc(sizeof(nodo_lvo)); //vemos si hay espacio

        if(nuevo_nodo != NULL){
            nuevo_nodo->VIPD = nuevo;
            //SI Pos=NULL significa que la lista estaba vacia o inserto por el primer elemento
            if (Pos == NULL){
                nuevo_nodo->PS = *cabeza;
                *cabeza = nuevo_nodo;
            }else{
            nuevo_nodo->PS = Pos->PS;
            Pos->PS = nuevo_nodo;
            *exito = 1; //exito total
        }}else{
            *exito = 2; //no hay espacio
        }
    }
}

//Evocación ( in x, out y, out éxito)
void Evocacion_LVO(int Dni_Buscado, Elector* Elec_Buscado, nodo_lvo* cabeza, int* exito){

nodo_lvo* Pos = NULL;
int ext;

Localizar_LVO(Dni_Buscado,cabeza, &Pos,&ext);

if(ext == 1){
    if(Pos == NULL){ //Esta cond es por si el elemento es el primero de la lista, Pos es el anterior, el dni buscado esta en Pos->PS
        *Elec_Buscado = cabeza->VIPD;
    }else{
        *Elec_Buscado = Pos->PS->VIPD;
    }
    *exito = 1;
}else{
    *exito = 0;
    }
}


//Baja ( in x, in y, out éxito)
void Baja_LVO(Elector a_Eliminar, nodo_lvo** cabeza, int* exito){

int Dni_Busco = a_Eliminar.DNI;
int ext;
nodo_lvo* Pos = NULL;

Localizar_LVO(Dni_Busco, *cabeza, &Pos, &ext);


if(ext == 1){
    nodo_lvo* Aux_Borrar = NULL;
    if(Pos == NULL){ //Primer y unico elemento
        Aux_Borrar = *cabeza;
        *cabeza = Aux_Borrar->PS;
    }
    else{
        Aux_Borrar = Pos->PS;
        Pos->PS = Aux_Borrar->PS;
    }
    free(Aux_Borrar);
    *exito = 1;
    }
    else{
     *exito = 0;
    }
}













//===========================================================================================================================
//===========================================================================================================================
//========================================================ABB================================================================
//===========================================================================================================================
//===========================================================================================================================
//Definimo estructura ABB
typedef struct{
    Elector VIPD;
    struct nodo_abb* P_izq;
    struct nodo_abb* P_der;
} nodo_abb;

//Localizar (in x, out pos, out éxito)
void Localizar_ABB(int Dni_Buscado, nodo_abb* raiz, nodo_abb** pos, int* exito){
nodo_abb* actual = raiz;
nodo_abb* padre = NULL;

while(actual != NULL && Dni_Buscado != actual->VIPD.DNI){
        padre = actual;
        if(actual->VIPD.DNI > Dni_Buscado){ //Debo de ir por la izq si el actual es > buscado
            actual = actual->P_izq;
        }else{
            actual = actual->P_der;
        }
}
if (actual != NULL && actual->VIPD.DNI == Dni_Buscado){
    *exito = 1;
    *pos = actual;
}else{
    *exito = 0;
    *pos = padre;}
}


//Alta (in x, in y, out éxito)
void Alta_ABB(Elector Nuevo,nodo_abb** raiz, int* exito){
int dni = Nuevo.DNI;
nodo_abb* pos;
int ext;

Localizar_ABB(dni,*raiz,&pos,&ext);

if(ext == 0){ //no existe el elemento
    nodo_abb* nuevo_nodo = (nodo_abb*)malloc(sizeof(nodo_abb)); //vemos si hay espacio
    if(nuevo_nodo != NULL){
    //HAY ESPACIO TODAVIA
        nuevo_nodo->VIPD = Nuevo;
        nuevo_nodo->P_izq = NULL;
        nuevo_nodo->P_der = NULL;

        if(pos == NULL){
            //Arbol vacio
            *raiz = nuevo_nodo;

        }else{
            if(pos->VIPD.DNI > dni){
                pos->P_izq = nuevo_nodo;
            }else{
                pos->P_der = nuevo_nodo;
            }
        }

    *exito = 1;
    } else{
    //NO HAY ESPACIO
   *exito = 2;
    }
    }else{
    *exito = 0; //dni ya existe
}
}

//Baja (in x, in y, out éxito)
void Baja_ABB(){}

//Evocación (in x, out y, out éxito)
void Evocacion_ABB(){}





















// Función mayus
void mayusc(char* cadena) {
    int i = 0;
    while (cadena[i] != '\0') {
        cadena[i] = toupper(cadena[i]);
        i++;
    }
}

//===========================================================================================================================
//===========================================================================================================================
//========================================================LSOBB==============================================================
//===========================================================================================================================
//===========================================================================================================================
int main(){
    Elector LSOBB[2200]; //apropocito le di de mas, por si se carga un archivo con 2003 personas, asi no revienta
    int cant_lsob = 0;
    //inicializamos ambos puntos a Null, despues hay que agregar el centinela a lvo
    nodo_lvo Cabeza_lvo = NULL;
    nodo_abb Raiz_abb = NULL;

    //=========================================================================
    //=============================leer el archivo=============================
    int cod_operacion;
    Elector temp;
    FILE *archivo = fopen("Operaciones_Padron.txt", "r");

    if(archivo == NULL){
        printf("Error: No se pudo abrir el archivo:Operaciones_Padron.txt");
        return -1;
    }

    while(fscanf(archivo,"%d",&cod_operacion) != EOF){
        fscanf(archivo, "%d", &temp.DNI);
        // Leemos nombre y direccion, permite espacios
        fscanf(archivo, " %[^\n]", temp.Nombre_Apellido);
        mayusc(temp.Nombre_Apellido);

        fscanf(archivo, " %[^\n]", temp.Domicilio);
        mayusc(temp.Domicilio);

        fscanf(archivo, "%d", &temp.Cod_Postal);
        fscanf(archivo, "%d", &temp.Mesa);
        fscanf(archivo, "%d", &temp.Circuito);

        //===)((/()=$#%&/())&%$%&/(%$%&/(%$%&/()
        //Tengo que hacer las altas para las estructuras todavia





        if (cod_operacion == 1) {
            // Llamada a función ALTA
        } else if (cod_operacion == 2) {
            // Llamada a función BAJA
        } else if (cod_operacion == 3) {
            // Llamada a función EVOCACIÓN
        }

    }
    fclose(archivo);






//===========================================================================================================================
//===========================================================================================================================
//========================================================MENU===============================================================
//===========================================================================================================================
//===========================================================================================================================
    /*
    1. Comparación de estructuras.
    2. Mostrar Estructura LVO.
    3. Mostrar Estructura LSOBB.
    4. Mostrar Estructura ABB.
    */
    int opcion;

    do{
        printf("\n======================= MENU =======================\n");
        printf("1. Comparacion de estructuras.\n");
        printf("2. Mostrar Estructura LVO.\n");
        printf("3. Mostrar Estructura LSOBB.\n");
        printf("4. Mostrar Estructura ABB.\n");
        printf("5. Salir.\n");
        printf("====================================================\n");
        printf("Ingrese una opcion: ");
        scanf("%d", &opcion);
        switch(opcion) {
            case 1:
                printf("Opcion en construccion...\n");
                break;

            case 2:
                printf("Mostrando LVO...\n");
                break;

            case 3:
                printf("Mostrando LSOBB...\n");
                break;

            case 4:
                printf("Mostrando ABB...\n");
                break;

            case 5:
                printf("Saliendo del programa...\n");
                break;

            default:
                printf("Opcion invalida. Intente de nuevo.\n");
                break;
        }
    }while(opcion != 5);




}
