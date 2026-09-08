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

typedef struct nodo_lvo {
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
    *pos = anterior; //DEVOLVEMOS LA POS PARA EL ALTA
}

//Alta (in x, in y, out éxito)
//doble puntero para cabeza porque si quiero almacenar en el primer elemento me dice que estoy haciend segmental fail
void Alta_LVO(Elector nuevo, nodo_lvo** cabeza, int* exito){
    int dni_busc = nuevo.DNI;
    nodo_lvo* Pos = NULL;
    int ext;
    Localizar_LVO(dni_busc, *cabeza, &Pos, &ext);
    if (ext == 1){
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
void Baja_LVO(Elector a_dar_de_baja, nodo_lvo** cabeza, int* exito) {
    int dni_busc = a_dar_de_baja.DNI;
    nodo_lvo* Pos = NULL;
    int ext;

    Localizar_LVO(dni_busc, *cabeza, &Pos, &ext);

    if (ext == 1) { // existe una nupla con ese x

        // Identificamos cuál es el nodo a borrar
        nodo_lvo* a_eliminar = NULL;
        if (Pos == NULL) {
            a_eliminar = *cabeza;
        } else {
            a_eliminar = Pos->PS;
        }


	    if (strcmp(a_eliminar->VIPD.Nombre_Apellido, a_dar_de_baja.Nombre_Apellido) == 0 &&
            strcmp(a_eliminar->VIPD.Domicilio, a_dar_de_baja.Domicilio) == 0 &&
            a_eliminar->VIPD.Cod_Postal == a_dar_de_baja.Cod_Postal &&
            a_eliminar->VIPD.Mesa == a_dar_de_baja.Mesa &&
            a_eliminar->VIPD.Circuito == a_dar_de_baja.Circuito) {

                   if (Pos == NULL) {
                *cabeza = a_eliminar->PS; // Si era el primero, la cabeza avanza
            } else {
                Pos->PS = a_eliminar->PS; // Puenteamos el nodo a eliminar
            }

            free(a_eliminar); // Liberamos la memoria RAM
            *exito = 1;

        } else {
            *exito = 0; // Fracasa: El DNI existe pero los otros datos no coinciden
        }
    } else {
        *exito = 0; // Fracasa: no hay una nupla con ese x
    }
}


void Init_LVO(nodo_lvo** cabeza) {
    *cabeza = (nodo_lvo*)malloc(sizeof(nodo_lvo));

    if (*cabeza != NULL) {
        //Le asignamos el valor centinela al DNI
        (*cabeza)->VIPD.DNI = 999999999;
        (*cabeza)->PS = NULL;
    }
}













//===========================================================================================================================
//===========================================================================================================================
//========================================================ABB================================================================
//===========================================================================================================================
//===========================================================================================================================
//Definimo estructura ABB
typedef struct nodo_abb{
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
void Baja_ABB(Elector a_eliminar, nodo_abb** raiz, int* exito) {
    int dni = a_eliminar.DNI;
    nodo_abb* actual = *raiz;
    nodo_abb* padre = NULL;

    // 1. Búsqueda simultánea (avanzamos actual, pero retenemos al padre)
    while (actual != NULL && actual->VIPD.DNI != dni) {
        padre = actual;
        if (dni < actual->VIPD.DNI) {
            actual = actual->P_izq;
        } else {
            actual = actual->P_der;
        }
    }

    // Si actual cayó a NULL, el elemento no está en el árbol
    if (actual == NULL) {
        *exito = 0;
        return;
    }


    if (strcmp(actual->VIPD.Nombre_Apellido, a_eliminar.Nombre_Apellido) == 0 &&
        strcmp(actual->VIPD.Domicilio, a_eliminar.Domicilio) == 0 &&
        actual->VIPD.Cod_Postal == a_eliminar.Cod_Postal &&
        actual->VIPD.Mesa == a_eliminar.Mesa &&
        actual->VIPD.Circuito == a_eliminar.Circuito) {

        //LoGICA DE DESCONEXIÓN

        //caso1: El nodo es una hoja
        if (actual->P_izq == NULL && actual->P_der == NULL) {
            if (padre == NULL) {
                *raiz = NULL; // Borramos la raíz si era el único nodo del árbol
            } else if (padre->P_izq == actual) {
                padre->P_izq = NULL; // Lo desenganchamos de la izquierda
            } else {
                padre->P_der = NULL; // Lo desenganchamos de la derecha
            }
            free(actual);
        }

        //caso2: El nodo tiene un solo hijo
        else if (actual->P_izq == NULL || actual->P_der == NULL) {
            // Identificamos de qué lado está el "nieto" que debemos salvar
            nodo_abb* hijo_unico = NULL;
            if (actual->P_izq != NULL) {
                hijo_unico = actual->P_izq;
            } else {
                hijo_unico = actual->P_der;
            }

            // Hacemos el puenteo directo entre el abuelo (padre) y el nieto (hijo_unico)
            if (padre == NULL) {
                *raiz = hijo_unico; // Borramos la raíz, el hijo pasa a ser la nueva raíz
            } else if (padre->P_izq == actual) {
                padre->P_izq = hijo_unico;
            } else {
                padre->P_der = hijo_unico;
            }
            free(actual);
        }

        //caso3:El nodo tiene dos hijos
        else {
            nodo_abb* padre_reemplazo = actual;
            nodo_abb* reemplazo = actual->P_izq; //1.buscamos en el subárbol izquierdo

            //2.Viajamos todo hacia la derecha para encontrar al predecesor
            while (reemplazo->P_der != NULL) {
                padre_reemplazo = reemplazo;
                reemplazo = reemplazo->P_der;
            }

            //3.copiamos la información de la nupla y al nodo que queríamos borrar
            actual->VIPD = reemplazo->VIPD;

            //4.Desconectamos físicamente al nodo reemplazo
            if (padre_reemplazo == actual) {
                //Caso extremo:el reemplazo era el hijo izquierdo directo (no hubo que bajar a la derecha)
                padre_reemplazo->P_izq = reemplazo->P_izq;
            } else {
                padre_reemplazo->P_der = reemplazo->P_izq;
            }
            free(reemplazo);
        }




}}

//Evocación (in x, out y, out éxito)
void Evocacion_ABB(int Dni_Busq, Elector* salida, nodo_abb* raiz, int* exito){

int ext;
nodo_abb* pos;

Localizar_ABB(Dni_Busq, raiz, &pos, &ext);


if(ext == 1){ //El elemento existe
*salida = pos->VIPD;
*exito = 1;
} else{
*exito = 0;
}
}






//===========================================================================================================================
//===========================================================================================================================
//========================================================LSOBB==============================================================
//===========================================================================================================================
//===========================================================================================================================

//Localizacion
//Localizar (in x, out pos, out éxito)
void Localizar_LSOBB(int dni_buscado, Elector LSOBB[], int cant_elementos, int* pos, int* exito) {
    int li = 0;
    int ls = cant_elementos - 1;
    int m = (li + ls) / 2;

    while (li <= ls && LSOBB[m].DNI != dni_buscado) {
        if (LSOBB[m].DNI < dni_buscado) {
            li = m + 1; // Descartamos la mitad izq
        } else {
            ls = m - 1; // Descartamos la mitad der
        }
        m = (li + ls) / 2; // Recalculamos el medio
    }

    // Al salir, verificamos por qué se detuvo el while
    if (li <= ls) {
        *exito = 1;
        *pos = m;  // Devuelve el índice exacto donde está
    } else {
        *exito = 0;
        *pos = li; // Devuelve la posición donde DEBERÍA ir para mantener el orden
    }
}


//Alta
//Alta (in x, in y, out éxito)
void Alta_LSOBB(Elector Nuevo, Elector LSOBB[], int* cant_Elementos,int* exito){

int dni_busq = Nuevo.DNI;
int pos;
int ext;

Localizar_LSOBB(dni_busq, LSOBB, *cant_Elementos, &pos ,&ext);

if(ext == 1){
   //El elemento ya existe
   *exito = 0;
}else{
    //tengo que correr todo lo que esta a la der un lugar para hacer espacio al nuevo element
    int i;
    for (i = *cant_Elementos - 1; i >= pos; i--){
        LSOBB[i+1] = LSOBB[i];
    }
    LSOBB[pos] = Nuevo;
    *cant_Elementos = *cant_Elementos + 1;
    *exito = 1;
}
}


//Baja
//Baja (in x, in y, out exito)
void Baja_LSOBB(Elector a_eliminar, Elector LSOBB[], int* cant_Elem, int* exito){
    int ext;
    int pos;

    Localizar_LSOBB(a_eliminar.DNI, LSOBB, *cant_Elem, &pos, &ext);

    if(ext == 1){
        if (strcmp(LSOBB[pos].Nombre_Apellido, a_eliminar.Nombre_Apellido) == 0 &&
            strcmp(LSOBB[pos].Domicilio, a_eliminar.Domicilio) == 0 &&
            LSOBB[pos].Cod_Postal == a_eliminar.Cod_Postal &&
            LSOBB[pos].Mesa == a_eliminar.Mesa &&
            LSOBB[pos].Circuito == a_eliminar.Circuito) {

            int i;
            for (i = pos; i < *cant_Elem - 1; i++){
                LSOBB[i] = LSOBB[i + 1];
            }

            *cant_Elem = *cant_Elem - 1;
            *exito = 1;
        } else {
            *exito = 0;
        }
    } else {

        *exito = 0;
    }
}


//Evocacion
//Evocación (in x, out y, out éxito)
void Evocacion_LSOBB(int Dni_Busq, Elector LSOBB[], Elector* Salida, int cant_Elem, int* exito){
    int ext;
    int pos;

    Localizar_LSOBB(Dni_Busq, LSOBB, cant_Elem, &pos, &ext);

    if(ext == 1){
        *Salida = LSOBB[pos];
        *exito = 1;
    } else {
        *exito = 0;
    }
}












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
    int cant_Elementos = 0;
    //inicializamos ambos puntos a Null, despues hay que agregar el centinela a lvo
    nodo_lvo* Acc_LVO = NULL;
    nodo_abb* Raiz_ABB = NULL;

    Init_LVO(&Acc_LVO);






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
        printf("[1] --> Comparacion de estructuras.\n");
        printf("[2] --> Mostrar Estructuras.\n");
        printf("[3] --> Salir.\n");
        printf("====================================================\n");
        printf("Ingrese una opcion: ");
        scanf("%d", &opcion);
        switch(opcion) {
            case 1:
                  printf("Datos cargados con exito...\n");
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


	int ext1, ext2, ext3;

        if (cod_operacion == 1) { // ALTA
            Alta_LVO(temp, &Acc_LVO, &ext1);
            Alta_ABB(temp, &Raiz_ABB, &ext2);
            Alta_LSOBB(temp, LSOBB, &cant_Elementos, &ext3);

            if(ext1 == 1 && ext2 == 1 && ext3 == 1){
                printf("Carga exitosa.\n");
            } else {
                printf("Algo salio mal bro.\n");
            }

        } else if (cod_operacion == 2) { // BAJA
            Baja_LVO(temp, &Acc_LVO, &ext1);
            Baja_ABB(temp, &Raiz_ABB, &ext2);
            Baja_LSOBB(temp, LSOBB, &cant_Elementos, &ext3);

            if(ext1 == 1 && ext2 == 1){
                printf("Baja exitosa.\n");
            }

        } else if (cod_operacion == 3) { // EVOCACION
            Elector recup_LVO, recup_ABB, recup_LSOBB;

            Evocacion_LVO(temp.DNI, &recup_LVO, Acc_LVO, &ext1);
            Evocacion_ABB(temp.DNI, &recup_ABB, Raiz_ABB, &ext2);
            Evocacion_LSOBB(temp.DNI, LSOBB, &recup_LSOBB, cant_Elementos, &ext3);
            if(ext1 == 1 && ext2 == 1){
                // La teoria prohíbe imprimir ADENTRO de la función,
                // por lo que imprimimos aquí en el main:
                // printf("Encontrado: %s\n", recup_ABB.Nombre_Apellido);
            }
        }
    }
    fclose(archivo);

                break;
            case 2:
                printf("Mostrando...\n");
                break;

            case 3:
                printf("Saliendo del programa...\n");
                break;

            default:
                printf("Opcion invalida. Intente de nuevo.\n");
                break;
        }
    }while(opcion != 3);



return 1;
}


