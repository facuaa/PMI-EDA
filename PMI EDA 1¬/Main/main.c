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

typedef struct {
    // ALTAS (Solo exitosas)
    float costo_total_alta;
    int cant_alta_exito;
    float max_alta;

    // BAJAS (Solo exitosas)
    float costo_total_baja;
    int cant_baja_exito;
    float max_baja;

    // EVOCACIONES (Separadas en Exito y Fracaso)
    float costo_total_evo_exito;
    int cant_evo_exito;
    float max_evo_exito;

    float costo_total_evo_fracaso;
    int cant_evo_fracaso;
    float max_evo_fracaso;
} Metricas;














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
// Localizar LVO
void Localizar_LVO(int dni_buscado, nodo_lvo* cabeza, nodo_lvo** pos, int* exito, float* costo_busq) {
    nodo_lvo* aux = cabeza;
    nodo_lvo* anterior = NULL;
    *costo_busq = 0.0;

    while(aux->VIPD.DNI < dni_buscado) {
        *costo_busq += 1.0;
        anterior = aux;
        aux = aux->PS;
    }

    *costo_busq += 1.0;

    if (aux->VIPD.DNI == dni_buscado) *exito = 1;
    else *exito = 0;

    *pos = anterior;
}

// Alta LVO
void Alta_LVO(Elector nuevo, nodo_lvo** cabeza, int* exito, float* costo_estructural) {
    nodo_lvo* Pos = NULL;
    int ext;
    float costo_busq = 0;

    Localizar_LVO(nuevo.DNI, *cabeza, &Pos, &ext, &costo_busq);
    *costo_estructural = 0.0;

    if (ext == 1) {
        *exito = 0;
    } else {
        nodo_lvo* nuevo_nodo = (nodo_lvo*)malloc(sizeof(nodo_lvo));
        if(nuevo_nodo != NULL) {
            nuevo_nodo->VIPD = nuevo;

            if (Pos == NULL) {
                nuevo_nodo->PS = *cabeza;
                *cabeza = nuevo_nodo;
            } else {
                nuevo_nodo->PS = Pos->PS;
                Pos->PS = nuevo_nodo;
            }

            *costo_estructural += 1.0;
            *exito = 1;
        } else {
            *exito = 2;
        }
    }
}

// Baja LVO
void Baja_LVO(Elector a_dar_de_baja, nodo_lvo** cabeza, int* exito, float* costo_estructural) {
    nodo_lvo* Pos = NULL;
    int ext;
    float costo_busq = 0;

    Localizar_LVO(a_dar_de_baja.DNI, *cabeza, &Pos, &ext, &costo_busq);
    *costo_estructural = 0.0;

    if (ext == 1) {
        nodo_lvo* a_eliminar = (Pos == NULL) ? *cabeza : Pos->PS;

        if (strcmp(a_eliminar->VIPD.Nombre_Apellido, a_dar_de_baja.Nombre_Apellido) == 0 &&
            strcmp(a_eliminar->VIPD.Domicilio, a_dar_de_baja.Domicilio) == 0 &&
            a_eliminar->VIPD.Cod_Postal == a_dar_de_baja.Cod_Postal &&
            a_eliminar->VIPD.Mesa == a_dar_de_baja.Mesa &&
            a_eliminar->VIPD.Circuito == a_dar_de_baja.Circuito) {

            if (Pos == NULL) *cabeza = a_eliminar->PS;
            else Pos->PS = a_eliminar->PS;

            *costo_estructural += 0.5;
            free(a_eliminar);
            *exito = 1;
        } else {
            *exito = 0;
        }
    } else {
        *exito = 0;
    }
}

// Evocacion LVO
void Evocacion_LVO(int Dni_Buscado, Elector* Elec_Buscado, nodo_lvo* cabeza, int* exito, float* costo_busq) {
    nodo_lvo* Pos = NULL;
    int ext;

    Localizar_LVO(Dni_Buscado, cabeza, &Pos, &ext, costo_busq);

    if(ext == 1) {
        if(Pos == NULL) *Elec_Buscado = cabeza->VIPD;
        else *Elec_Buscado = Pos->PS->VIPD;
        *exito = 1;
    } else {
        *exito = 0;
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
// Localizar ABB
void Localizar_ABB(int Dni_Buscado, nodo_abb* raiz, nodo_abb** pos, int* exito, float* costo_busq) {
    nodo_abb* actual = raiz;
    nodo_abb* padre = NULL;
    *costo_busq = 0.0;

    while(actual != NULL && Dni_Buscado != actual->VIPD.DNI) {
        *costo_busq += 1.0;
        padre = actual;
        if(actual->VIPD.DNI > Dni_Buscado) actual = actual->P_izq;
        else actual = actual->P_der;
    }

    if (actual != NULL) {
        *costo_busq += 1.0; // Consulta final exitosa
        *exito = 1;
        *pos = actual;
    } else {
        *exito = 0;
        *pos = padre;
    }
}

// Alta ABB
void Alta_ABB(Elector Nuevo, nodo_abb** raiz, int* exito, float* costo_estructural) {
    nodo_abb* pos;
    int ext;
    float costo_busq = 0;

    Localizar_ABB(Nuevo.DNI, *raiz, &pos, &ext, &costo_busq);
    *costo_estructural = 0.0; // Aislamos modificaciones de punteros

    if(ext == 0) {
        nodo_abb* nuevo_nodo = (nodo_abb*)malloc(sizeof(nodo_abb));
        if(nuevo_nodo != NULL) {
            nuevo_nodo->VIPD = Nuevo;
            nuevo_nodo->P_izq = NULL;
            nuevo_nodo->P_der = NULL;

            if(pos == NULL) {
                *raiz = nuevo_nodo;
            } else {
                if(pos->VIPD.DNI > Nuevo.DNI) pos->P_izq = nuevo_nodo;
                else pos->P_der = nuevo_nodo;
            }
            *costo_estructural += 0.5; // 1 conexión de puntero
            *exito = 1;
        } else {
            *exito = 2; // Sin espacio
        }
    } else {
        *exito = 0;
    }
}

// Baja ABB
void Baja_ABB(Elector a_eliminar, nodo_abb** raiz, int* exito, float* costo_estructural) {
    int dni = a_eliminar.DNI;
    nodo_abb* actual = *raiz;
    nodo_abb* padre = NULL;
    *costo_estructural = 0.0;


    while (actual != NULL && actual->VIPD.DNI != dni) {
        padre = actual;
        if (dni < actual->VIPD.DNI) actual = actual->P_izq;
        else actual = actual->P_der;
    }

    if (actual == NULL) {
        *exito = 0;
        return;
    }

    // Confirmación
    if (strcmp(actual->VIPD.Nombre_Apellido, a_eliminar.Nombre_Apellido) == 0 &&
        strcmp(actual->VIPD.Domicilio, a_eliminar.Domicilio) == 0 &&
        actual->VIPD.Cod_Postal == a_eliminar.Cod_Postal &&
        actual->VIPD.Mesa == a_eliminar.Mesa &&
        actual->VIPD.Circuito == a_eliminar.Circuito) {

        // Caso 1: Hoja
        if (actual->P_izq == NULL && actual->P_der == NULL) {
            if (padre == NULL) *raiz = NULL;
            else if (padre->P_izq == actual) padre->P_izq = NULL;
            else padre->P_der = NULL;
            *costo_estructural += 0.5;
            free(actual);
        }
        // Caso2: un hijo
        else if (actual->P_izq == NULL || actual->P_der == NULL) {
            nodo_abb* hijo_unico = (actual->P_izq != NULL) ? actual->P_izq : actual->P_der;
            if (padre == NULL) *raiz = hijo_unico;
            else if (padre->P_izq == actual) padre->P_izq = hijo_unico;
            else padre->P_der = hijo_unico;
            *costo_estructural += 0.5;
            free(actual);
        }
        // Caso 3: dos hijos Política de reemplazo
        else {
            nodo_abb* padre_reemplazo = actual;
            nodo_abb* reemplazo = actual->P_izq; // Menor de los mayores (buscamos predecesor)

            while (reemplazo->P_der != NULL) {
                padre_reemplazo = reemplazo;
                reemplazo = reemplazo->P_der;
            }

            actual->VIPD = reemplazo->VIPD;
            *costo_estructural += 1.0; // Costo de copia de datos

            if (padre_reemplazo == actual) padre_reemplazo->P_izq = reemplazo->P_izq;
            else padre_reemplazo->P_der = reemplazo->P_izq;

            *costo_estructural += 0.5; // Desconexión de puntero
            free(reemplazo);
        }
        *exito = 1;
    } else {
        *exito = 0;
    }
}

// Evocacion ABB (la que ya habíamos armado)
void Evocacion_ABB(int Dni_Busq, Elector* salida, nodo_abb* raiz, int* exito, float* costo_busq) {
    int ext;
    nodo_abb* pos;
    Localizar_ABB(Dni_Busq, raiz, &pos, &ext, costo_busq);
    if(ext == 1) {
        *salida = pos->VIPD;
        *exito = 1;
    } else *exito = 0;
}






//===========================================================================================================================
//===========================================================================================================================
//========================================================LSOBB==============================================================
//===========================================================================================================================
//===========================================================================================================================

//Localizacion
//Localizar (in x, out pos, out éxito)
void Localizar_LSOBB(int dni_buscado, Elector LSOBB[], int cant_elementos, int* pos, int* exito, int* costo_busq) {

   if (cant_elementos == 0) {
        *exito = 0;
        *pos = 0;
        return;
    }

    int li = 0;
    int ls = cant_elementos - 1;
    int m = (li + ls + 1) / 2;
    *costo_busq = 0;

   while (li < ls) {
        int m = (li + ls) / 2;
        *costo_busq += 1;

        if (LSOBB[m].DNI < dni_buscado) {
            li = m + 1;
        } else {
            ls = m;
        }
    }
    *costo_busq += 1;
    if (LSOBB[li].DNI == dni_buscado) {
        *exito = 1;
        *pos = li;
    } else {
        *exito = 0;
        // Si no lo encontro indicamos dónde deberia ir para el ALTAA
        if (LSOBB[li].DNI < dni_buscado) *pos = li + 1;
        else *pos = li;
    }
}

void Alta_LSOBB(Elector Nuevo, Elector LSOBB[], int* cant_Elementos, int* exito, int* costo_estructural) {
    int pos, ext;
    int costo_busq = 0; // Variable temporal

    Localizar_LSOBB(Nuevo.DNI, LSOBB, *cant_Elementos, &pos, &ext, &costo_busq);

    *costo_estructural = 0;

    if(ext == 1) {
        *exito = 0;
    } else {
        for (int i = *cant_Elementos - 1; i >= pos; i--) {
            LSOBB[i+1] = LSOBB[i];
            *costo_estructural += 1;
        }
        LSOBB[pos] = Nuevo;
        *cant_Elementos += 1;
        *exito = 1;
    }
}

// Baja LSOBB
void Baja_LSOBB(Elector a_eliminar, Elector LSOBB[], int* cant_Elem, int* exito, int* costo_estructural) {
    int ext, pos;
    int costo_busq = 0;

    Localizar_LSOBB(a_eliminar.DNI, LSOBB, *cant_Elem, &pos, &ext, &costo_busq);

    *costo_estructural = 0; // Arranca en 0

    if(ext == 1) {
        if (strcmp(LSOBB[pos].Nombre_Apellido, a_eliminar.Nombre_Apellido) == 0 &&
            strcmp(LSOBB[pos].Domicilio, a_eliminar.Domicilio) == 0 &&
            LSOBB[pos].Cod_Postal == a_eliminar.Cod_Postal &&
            LSOBB[pos].Mesa == a_eliminar.Mesa &&
            LSOBB[pos].Circuito == a_eliminar.Circuito) {

            // Corrimientos físicos hacia la izquierda
            for (int i = pos; i < *cant_Elem - 1; i++) {
                LSOBB[i] = LSOBB[i + 1];
                *costo_estructural += 1;
            }
            *cant_Elem -= 1;
            *exito = 1;
        } else {
            *exito = 0;
        }
    } else {
        *exito = 0;
    }
}

// Evocación LSOBB
void Evocacion_LSOBB(int Dni_Busq, Elector LSOBB[], Elector* Salida, int cant_Elem, int* exito, int* costo_busq) {
    int ext, pos;
    *costo_busq = 0;

    Localizar_LSOBB(Dni_Busq, LSOBB, cant_Elem, &pos, &ext, costo_busq);

    if(ext == 1) {
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
    nodo_lvo* Acc_LVO = NULL;
    nodo_abb* Raiz_ABB = NULL;

    Init_LVO(&Acc_LVO);



    Metricas met_LVO = {0};
    Metricas met_ABB = {0};
    Metricas met_LSO = {0};








//===========================================================================================================================
//===========================================================================================================================
//========================================================MENU===============================================================
//===========================================================================================================================
//===========================================================================================================================


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





            case 1:{

    //=========================================================================
    //=============================leer el archivo=============================
    int cod_operacion;
    Elector temp;
    Elector recup_LVO, recup_ABB, recup_LSOBB;

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


        //Costos temps
        int ext1, ext2, ext3;
        int costo_LSOBB = 0;
        float costo_ABB = 0.0;
        float costo_LVO = 0.0;



        switch(cod_operacion) {
            // ALTA
            case 1:
                // --- LVO ---
                Alta_LVO(temp, &Acc_LVO, &ext1, &costo_LVO);
                if (ext1 == 1) {
                    met_LVO.costo_total_alta += costo_LVO;
                    met_LVO.cant_alta_exito++;

                    // --- CHEQUEO DEL MÁXIMO ---
                    if (costo_LVO > met_LVO.max_alta) {
                        met_LVO.max_alta = costo_LVO;
                    }
                }

                // --- ABB ---
                Alta_ABB(temp, &Raiz_ABB, &ext2, &costo_ABB);
                if (ext2 == 1) {
                    met_ABB.costo_total_alta += costo_ABB;
                    met_ABB.cant_alta_exito++;

                    // --- CHEQUEO DEL MÁXIMO ---
                    if (costo_ABB > met_ABB.max_alta) {
                        met_ABB.max_alta = costo_ABB;
                    }
                }

                // --- LSOBB ---
                Alta_LSOBB(temp, LSOBB, &cant_Elementos, &ext3, &costo_LSOBB);
                if (ext3 == 1) {
                    met_LSO.costo_total_alta += costo_LSOBB;
                    met_LSO.cant_alta_exito++;

                    // --- CHEQUEO DEL MÁXIMO ---
                    if (costo_LSOBB > met_LSO.max_alta) {
                        met_LSO.max_alta = costo_LSOBB;
                    }
                }
                break;

            // BAJA
            case 2:
            Baja_LVO(temp, &Acc_LVO, &ext1, &costo_LVO);
                if (ext1 == 1) {
                    met_LVO.costo_total_baja += costo_LVO;
                    met_LVO.cant_baja_exito++;

                    // --- CHEQUEO DEL MÁXIMO ---
                    if (costo_LVO > met_LVO.max_baja) {
                        met_LVO.max_baja = costo_LVO;
                    }
                }


            Baja_ABB(temp, &Raiz_ABB, &ext2, &costo_ABB);
                if (ext2 == 1) {
                    met_ABB.costo_total_baja += costo_ABB;
                    met_ABB.cant_baja_exito++;

                    // --- CHEQUEO DEL MÁXIMO ---
                    if (costo_ABB > met_ABB.max_baja) {
                        met_ABB.max_baja = costo_ABB;
                    }
                }

            Baja_LSOBB(temp, LSOBB, &cant_Elementos, &ext3, &costo_LSOBB);
                if (ext3 == 1) {
                    met_LSO.costo_total_baja += costo_LSOBB;
                    met_LSO.cant_baja_exito++;

                    // --- CHEQUEO DEL MÁXIMO ---
                    if (costo_LSOBB > met_LSO.max_baja) {
                        met_LSO.max_baja = costo_LSOBB;
                    }
                }
                break;

            // EVOCACION
            case 3: {
                Elector recup_LVO, recup_ABB, recup_LSOBB;

                // --- Evocación LVO ---
                Evocacion_LVO(temp.DNI, &recup_LVO, Acc_LVO, &ext1, &costo_LVO);
                if (ext1 == 1) {
                    met_LVO.costo_total_evo_exito += costo_LVO;
                    met_LVO.cant_evo_exito++;
                    if (costo_LVO > met_LVO.max_evo_exito) met_LVO.max_evo_exito = costo_LVO;
                } else {
                    met_LVO.costo_total_evo_fracaso += costo_LVO;
                    met_LVO.cant_evo_fracaso++;
                    if (costo_LVO > met_LVO.max_evo_fracaso) met_LVO.max_evo_fracaso = costo_LVO;
                }

                // --- Evocación ABB ---
                Evocacion_ABB(temp.DNI, &recup_ABB, Raiz_ABB, &ext2, &costo_ABB);
                if (ext2 == 1) {
                    met_ABB.costo_total_evo_exito += costo_ABB;
                    met_ABB.cant_evo_exito++;
                    if (costo_ABB > met_ABB.max_evo_exito) met_ABB.max_evo_exito = costo_ABB;
                } else {
                    met_ABB.costo_total_evo_fracaso += costo_ABB;
                    met_ABB.cant_evo_fracaso++;
                    if (costo_ABB > met_ABB.max_evo_fracaso) met_ABB.max_evo_fracaso = costo_ABB;
                }

                // --- Evocación LSOBB ---
                Evocacion_LSOBB(temp.DNI, LSOBB, &recup_LSOBB, cant_Elementos, &ext3, &costo_LSOBB);
                if (ext3 == 1) {
                    met_LSO.costo_total_evo_exito += costo_LSOBB;
                    met_LSO.cant_evo_exito++;
                    if (costo_LSOBB > met_LSO.max_evo_exito) met_LSO.max_evo_exito = costo_LSOBB;
                } else {
                    met_LSO.costo_total_evo_fracaso += costo_LSOBB;
                    met_LSO.cant_evo_fracaso++;
                    if (costo_LSOBB > met_LSO.max_evo_fracaso) met_LSO.max_evo_fracaso = costo_LSOBB;
                }
                break;
            }
        }

    }




    fclose(archivo);

    float med_alta_lvo = 0, med_alta_abb = 0, med_alta_lso = 0;
    float med_baja_lvo = 0, med_baja_abb = 0, med_baja_lso = 0;
    float med_eve_lvo = 0, med_eve_abb = 0, med_eve_lso = 0;
    float med_evf_lvo = 0, med_evf_abb = 0, med_evf_lso = 0;


    // --- ALTAS ---
    if (met_LVO.cant_alta_exito > 0) {
        med_alta_lvo = met_LVO.costo_total_alta / met_LVO.cant_alta_exito;
    }
    if (met_ABB.cant_alta_exito > 0) {
        med_alta_abb = met_ABB.costo_total_alta / met_ABB.cant_alta_exito;
    }
    if (met_LSO.cant_alta_exito > 0) {
        med_alta_lso = met_LSO.costo_total_alta / met_LSO.cant_alta_exito;
    }

    // --- BAJAS ---
    if (met_LVO.cant_baja_exito > 0) {
        med_baja_lvo = met_LVO.costo_total_baja / met_LVO.cant_baja_exito;
    }
    if (met_ABB.cant_baja_exito > 0) {
        med_baja_abb = met_ABB.costo_total_baja / met_ABB.cant_baja_exito;
    }
    if (met_LSO.cant_baja_exito > 0) {
        med_baja_lso = met_LSO.costo_total_baja / met_LSO.cant_baja_exito;
    }

    // --- EVOCACIÓN ÉXITO ---
    if (met_LVO.cant_evo_exito > 0) {
        med_eve_lvo = met_LVO.costo_total_evo_exito / met_LVO.cant_evo_exito;
    }
    if (met_ABB.cant_evo_exito > 0) {
        med_eve_abb = met_ABB.costo_total_evo_exito / met_ABB.cant_evo_exito;
    }
    if (met_LSO.cant_evo_exito > 0) {
        med_eve_lso = met_LSO.costo_total_evo_exito / met_LSO.cant_evo_exito;
    }

    // --- EVOCACIÓN FRACASO ---
    if (met_LVO.cant_evo_fracaso > 0) {
        med_evf_lvo = met_LVO.costo_total_evo_fracaso / met_LVO.cant_evo_fracaso;
    }
    if (met_ABB.cant_evo_fracaso > 0) {
        med_evf_abb = met_ABB.costo_total_evo_fracaso / met_ABB.cant_evo_fracaso;
    }
    if (met_LSO.cant_evo_fracaso > 0) {
        med_evf_lso = met_LSO.costo_total_evo_fracaso / met_LSO.cant_evo_fracaso;
    }

    // =======================================================================================
    // 2. IMPRESIÓN DE LA TABLA COMPARATIVA
    // =======================================================================================
    printf("\n");
    printf("\t\t\t+----------------------+------------+------------+------------+\n");
    printf("\t\t\t| METRICA              |    LVO     |   LSOBB    |    ABB     |\n");
    printf("\t\t\t+----------------------+------------+------------+------------+\n");
    printf("\t\t\t| Alta Maxima          | %10.2f | %10.2f | %10.2f |\n", met_LVO.max_alta, met_LSO.max_alta, met_ABB.max_alta);
    printf("\t\t\t| Alta Media           | %10.2f | %10.2f | %10.2f |\n", med_alta_lvo, med_alta_lso, med_alta_abb);
    printf("\t\t\t| Alta Cantidad        | %10d | %10d | %10d |\n", met_LVO.cant_alta_exito, met_LSO.cant_alta_exito, met_ABB.cant_alta_exito);
    printf("\t\t\t+----------------------+------------+------------+------------+\n");
    printf("\t\t\t| Baja Maxima          | %10.2f | %10.2f | %10.2f |\n", met_LVO.max_baja, met_LSO.max_baja, met_ABB.max_baja);
    printf("\t\t\t| Baja Media           | %10.2f | %10.2f | %10.2f |\n", med_baja_lvo, med_baja_lso, med_baja_abb);
    printf("\t\t\t| Baja Cantidad        | %10d | %10d | %10d |\n", met_LVO.cant_baja_exito, met_LSO.cant_baja_exito, met_ABB.cant_baja_exito);
    printf("\t\t\t+----------------------+------------+------------+------------+\n");
    printf("\t\t\t|   Evocacion          |            |            |            |\n");
    printf("\t\t\t+----------------------+------------+------------+------------+\n");
    printf("\t\t\t| Exito Maxima         | %10.2f | %10.2f | %10.2f |\n", met_LVO.max_evo_exito, met_LSO.max_evo_exito, met_ABB.max_evo_exito);
    printf("\t\t\t| Exito Media          | %10.2f | %10.2f | %10.2f |\n", med_eve_lvo, med_eve_lso, med_eve_abb);
    printf("\t\t\t| Exito Cantidad       | %10d | %10d | %10d |\n", met_LVO.cant_evo_exito, met_LSO.cant_evo_exito, met_ABB.cant_evo_exito);
    printf("\t\t\t+----------------------+------------+------------+------------+\n");
    printf("\t\t\t| Fracaso Maxima       | %10.2f | %10.2f | %10.2f |\n", met_LVO.max_evo_fracaso, met_LSO.max_evo_fracaso, met_ABB.max_evo_fracaso);
    printf("\t\t\t| Fracaso Media        | %10.2f | %10.2f | %10.2f |\n", med_evf_lvo, med_evf_lso, med_evf_abb);
    printf("\t\t\t| Fracaso Cantidad     | %10d | %10d | %10d |\n", met_LVO.cant_evo_fracaso, met_LSO.cant_evo_fracaso, met_ABB.cant_evo_fracaso);
    printf("\t\t\t+----------------------+------------+------------+------------+\n\n");

    break;
            }
















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


