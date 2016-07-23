//AQUI ESTARAN LAS FUNCIONES DE LA LIBRERIA
#include "Radio.h"

void pausar(){
	printf("\n");
    printf("\t \t \t \t \t PRESIONA CUALQUIER TECLA PARA VOLVER...");
    getch();
}

void errorCritico()
{
  printf("No se ha podido reservar memoria\n");
  getch();
  exit(1);
}

void mensajePorDefecto()
{
    textcolor(RED);
    printf("%cLa opcion que ingreso no es correcta!",173);
    Sleep(1000);
}

//INICIALIZAR LA LISTA
void iniciarListas(Radio *estacion_de_radio){
    //SE INICIAN TODAS LAS LISTAS DE LA ESTACIÓN DE RADIO
	estacion_de_radio->lista_de_locutores=NULL;
	estacion_de_radio->lista_de_secretarias=NULL;
}

void cargarListaLocutores(Radio *estacion_de_radio){
	system("cls");
	//CREAR ARCHIVO O ABRIR PARA LA LECTURA
	FILE *file;
	file=fopen(ARCHIVO_LOCUTORES,"r");
	//SI NO SE PUDO LEER ES PORQUE NO EXISTE
	 if(!file){
	 	system("cls");
		printf("El archivo esta vacio\n");
		pausar();
	 }else{
	    //LOCUTOR GENERAL QUE SE USARA PARA LEER CADA LOCUTOR QUE HAY EN EL ARCHIVO
	 	Locutor *locutorALeer;
	 	//REBOBINAR ARCHIVO
	 	rewind(file);
	 	//RESERVAR MEMORIA PARA LA PRIMERA VEZ QUE USARÁ EL LOCUTOR GENERAL
        locutorALeer=(Locutor*)malloc(sizeof(Locutor));

	 //MIENTRAS VAYA LEYENDO EL LOCUTOR QUE LO VAYA INGRENSANDO EN LA LISTA
        while(fread(locutorALeer, sizeof(Locutor),1, file)){
            //SE INGRESA EL LOCUTOR A LA LISTA DE LOCUTORES DE LA RADIO
	        ingresarLocutorALaLista(estacion_de_radio,locutorALeer);
            //SE ASIGNA OTRO ESPACIO DE MEMORIA PARA EL SIGUIENTE LOCUTOR
            locutorALeer=(Locutor*)malloc(sizeof(Locutor));
        }
        //QUE SE LIBERE EL ESPACIO CREADO POR EL LOCUTR
        free(locutorALeer);
	 }
	 //AL TERMINAR EL PROCESO QUE SE CIERRE EL ARCHIVO
	 fclose(file);
}

void controlDeUsuarios(Radio *estacion_de_radio)
{
    char op;
    do{
    system("cls");
    textcolor(WHITE);
    printf("[1] Registro de locutores \n");
    printf("[2] Registro de secretaria \n");
    printf("[3] Modificar registro de locutor \n");
    printf("[4] Modificar registro de secretaria \n");
    printf("[5] Mostrar listado de locutores \n");
    printf("[6] Mostrar listado de secretarias \n");
    printf("[7] Mostrar due%co \n",164);
    printf("[8] Menu principal \n");
    textcolor(YELLOW);
    printf("Ingrese la opcion:");
    textcolor(WHITE);fflush(stdin); op=getchar();

    switch(op){
        case 49: registroLocutor(estacion_de_radio); break;
        case 50: registroSecretaria(estacion_de_radio);break;
        case 51: modificarRegistroLocutor(estacion_de_radio); break;
        case 52: mostrarListaDeLocutores2(estacion_de_radio); break;
        case 53: mostrarListaDeLocutores(); break;
        case 54: mostrarListaDeSecretarias();break;
        case 55: break;
        case 56: menu(estacion_de_radio); break;
        default : mensajePorDefecto();
    }
    }while(op!=56);

}

static void guardarLocutor(Locutor *nuevoLocutor){
	FILE *file;

	file=fopen(ARCHIVO_LOCUTORES,"ab");
	if(!file){
		errorCritico();
	}else{
		fwrite(nuevoLocutor, sizeof(Locutor), 1,file);
	}
	fclose(file);
}

static void guardarSecretaria(Secretaria *nuevaSecretaria){
	FILE *file;

	file=fopen(ARCHIVO_SECRETARIAS,"ab");
	if(!file){
		errorCritico();
	}else{
		fwrite(nuevaSecretaria, sizeof(Secretaria), 1,file);
	}
	fclose(file);
}

static void ingresarLocutorALaLista(Radio *estacion_de_radio, Locutor *nuevoLocutor){
    //EL NUEVO NODO LOCUTOR SU SIGUIENTE APUNTARA A NULL PARA LOS ENLACES
    nuevoLocutor->sig=NULL;
    //SI LA LISTA ESTA VACIA EL NUEVO LOCUTOR SERA EL PRIMER NODO EN LA LISTA
	if(!estacion_de_radio->lista_de_locutores){
		estacion_de_radio->lista_de_locutores=nuevoLocutor;
	} // SI LA LISTA YA TIENE ELEMENTOS SE CORRERA HASTA LLEGAR AL FINAL PARA HACER EL ENLACE
	else{
		Locutor *listaAuxiliar;
		listaAuxiliar=estacion_de_radio->lista_de_locutores;
		while(listaAuxiliar->sig)
			listaAuxiliar=listaAuxiliar->sig;
		//SE HACE EL ENLACE CON EL ULTIMO NODO DE LA LISTA
		listaAuxiliar->sig=nuevoLocutor;

	}
}

static void ingresarSecretariaALaLista(Radio *estacion_de_radio, Secretaria *nuevaSecretaria){
	if(!estacion_de_radio->lista_de_secretarias){
		estacion_de_radio->lista_de_secretarias=nuevaSecretaria;
	}
	else{
		Locutor *listaAuxiliar;
		listaAuxiliar=estacion_de_radio->lista_de_secretarias;
		while(listaAuxiliar->sig)
			listaAuxiliar=listaAuxiliar->sig;
		listaAuxiliar->sig=nuevaSecretaria;

	}
}

static int validarNombre(char nombre[]){
    //EL NOMBRE NO PUEDE TENER ESPACIOS
    //NO PUEDE TENER MAS DE 20 LETRAS
    //NO PUEDE TENER MAS DE UNA MAYUSCULA Y POR DEFECTO QUE SEA LA PRIMERA
    int i=0;
    while(nombre[i]!='\0'){
        if(nombre[i]==' ')
            return 0;
        i++;
    }
    if(strlen(nombre)>20)
        return 0;
    i=1;
    while(nombre[i]!='\0'){
        if(nombre[i]==toupper(nombre[i]))
            return 0;
        i++;
    }
    return 1;

}

static int validarCedula(Radio *estacion_de_radio, int cedula){
    Locutor * listaAuxiliar=estacion_de_radio->lista_de_locutores;
    Secretaria *listaAuxiliarS=estacion_de_radio->lista_de_secretarias;
    while(listaAuxiliar){
        if(listaAuxiliar->persona_locutor.cedula==cedula)
            return 0;
        listaAuxiliar=listaAuxiliar->sig;
    }
    while(listaAuxiliarS){
        if(listaAuxiliarS->persona_secretaria.cedula==cedula)
            return 0;
        listaAuxiliarS=listaAuxiliarS->sig;
    }
    if(cedula<9000000 || cedula>40000000)
        return 0;
    return 1;
}

static int validarEdad(int edad){
    if(edad<18)
        return 0;
    else if(edad>80)
        return 0;

    return 1;
}

static int validarSueldo(int sueldo){
        if(sueldo>50000)
            return 0;
        else if(sueldo<0)
            return 0;
    return 1;
}

void registroLocutor(Radio *estacion_de_radio){
	system("cls");

    Locutor *nuevoLocutor;
    nuevoLocutor=(Locutor*)calloc(1,sizeof(Locutor));

    if(!nuevoLocutor){
    	errorCritico();
    }
    else{
    	FILE * file=fopen(ARCHIVO_LOCUTORES,"rb");
	    int id=0, valido=0;

	    fseek(file,(sizeof(Locutor)*(-1)),SEEK_END);
	    if(fread(nuevoLocutor,sizeof(Locutor),1,file))
	    	id=nuevoLocutor->empleado_locutor.id;
        do{
        textcolor(WHITE);
	    printf("Ingrese el nombre del nuevo locutor:                                  \n");
	    fflush(stdin);
	    gets(nuevoLocutor->persona_locutor.nombre);
	    valido=validarNombre(nuevoLocutor->persona_locutor.nombre);
            if(!valido){
                textcolor(RED);
                printf("El nombre no puede tener espacios o ser mas largo de 20 letras\n");
                Sleep(2000);
                system("cls");
            }
        }while(!valido);

	     do{
        textcolor(WHITE);
	    printf("Ingrese el Apellido del nuevo locutor:                                  \n");
	    fflush(stdin);
	    gets(nuevoLocutor->persona_locutor.apellido);
	    valido=validarNombre(nuevoLocutor->persona_locutor.apellido);
            if(!valido){
                textcolor(RED);
                printf("El Apellido no puede tener espacios o ser mas largo de 20 letras\r");
                Sleep(2000);
            }
        }while(!valido);

	    do{
            textcolor(WHITE);
            printf("Ingrese la cedula del locutor                                \n");
            scanf("%i",&nuevoLocutor->persona_locutor.cedula);
            valido=validarCedula(estacion_de_radio,nuevoLocutor->persona_locutor.cedula);
            if(!valido){
                textcolor(RED);
                printf("La cedula que ingreso es incorrecta o ya esta registrada \r");
                Sleep(2000);
            }
	    }while(!valido);

	    do{
            textcolor(WHITE);
            printf("Ingrese la edad del locutor                                \n");
            scanf("%i",&nuevoLocutor->persona_locutor.edad);
            valido=validarEdad(nuevoLocutor->persona_locutor.edad);
            if(!valido){
                textcolor(RED);
                printf("La edad valida tiene que estar entre 18 y 80 a%cos \r",164);
                Sleep(2000);
            }
	    }while(!valido);

	    do{
            textcolor(WHITE);
            printf("Ingrese el sueldo del locutor                                \n");
            scanf("%i",&nuevoLocutor->empleado_locutor.sueldo);
            valido=validarSueldo(nuevoLocutor->empleado_locutor.sueldo);
            if(!valido){
                textcolor(RED);
                printf("El sueldo tiene que estar entre el presupuesto \r");
                Sleep(2000);
            }
	    }while(!valido);

	    if(valido){
            id++;
            nuevoLocutor->empleado_locutor.id=id;
            nuevoLocutor->empleado_locutor.activo=1;
            nuevoLocutor->sig=NULL;
            fclose(file);
            ingresarLocutorALaLista(estacion_de_radio,nuevoLocutor);
            guardarLocutor(nuevoLocutor);
	    }
    }
}

Locutor * buscarLocutor(Radio * estacion_de_radio,Locutor *locutorAModificar){

        int op=0;
        do{
        system("cls");
        textcolor(WHITE);
        printf("[1] Buscar por ID\n");
        printf("[2] Buscar por Cedula\n");
        printf("[3] Buscar por Nombre\n");
        textcolor(YELLOW);
        printf("Ingrese la opcion: ");
        textcolor(WHITE);scanf("%i",&op);

        switch(op){
            case 1: break;
            case 2: system("cls");
                    int cedula;
                    printf("Ingresar Cedula: \n");
                    scanf("%i",&cedula);
                    //Para recorrer la lista
                    locutorAModificar=estacion_de_radio->lista_de_locutores;
                    while(locutorAModificar){
                        if(locutorAModificar->persona_locutor.cedula==cedula){
                            return locutorAModificar;
                        }
                        locutorAModificar=locutorAModificar->sig;
                    }
                    printf("No hay ningun Locutor registrado con esa Cedula\n");
                    pausar();
                    return 0;
                    break;

            case 3: system("cls");
                    char nombre[30];
                    printf("Ingresar Nombre: \n");
                    fflush(stdin);
                    gets(nombre);
                    printf("%-10s %-20s %-10s %-10s %-10s %-10s %-10s %-10s\n"," ","Id Locutor","Nombre","Apellido","Cedula","Edad","Sueldo","Nombre del programa");
                    int i=1, encontrado=0;
                    locutorAModificar=estacion_de_radio->lista_de_locutores;
                    while(locutorAModificar){
                        if(strcmpi(locutorAModificar->persona_locutor.nombre,nombre)==0){
                            printf("[%i] %-6s %-20i %-10s %-10s %-10i %-10i %-10i %-10s \n",i," ",locutorAModificar->empleado_locutor.id,
                            locutorAModificar->persona_locutor.nombre,
                            locutorAModificar->persona_locutor.apellido,
                            locutorAModificar->persona_locutor.cedula,
                            locutorAModificar->persona_locutor.edad,
                            locutorAModificar->empleado_locutor.sueldo,
                            "Nombre del programa");
                            i++;
                            encontrado=1;
                            }
                            locutorAModificar=locutorAModificar->sig;
                        }

                    if(encontrado){
                        do{
                            textcolor(YELLOW);
                            printf("Ingrese el locutor a elegir: "); textcolor(WHITE);scanf("%i", &op);
                          }while(op>=i || op<0);
                        locutorAModificar=estacion_de_radio->lista_de_locutores;
                        while(locutorAModificar){
                            if(strcmpi(locutorAModificar->persona_locutor.nombre,nombre)==0)
                                --op;
                            if(op==0){
                                pausar();
                                return locutorAModificar;
                              }
                            locutorAModificar=locutorAModificar->sig;
                          }
                      }
                    printf("No hay ningun Locutor registrado con ese Nombre\n");
                    pausar();
                    return 0;
                    break;
            default : mensajePorDefecto(); break;
        }
        }while(op!=3);

}

static Locutor * eliminarLocutor(Radio *estacion_de_radio, Locutor *locutorAModificar)
{
    Locutor *nodoAnterior, *cabezaAuxiliar=estacion_de_radio->lista_de_locutores;
    while(cabezaAuxiliar){
            //SI ALGUN LOCUTOR TIENE REGISTRADA ESA CEDULA QUE SALGA DEL BUCLE
        if(cabezaAuxiliar->persona_locutor.cedula==locutorAModificar->persona_locutor.cedula)
            break;
        //LOCUTOR ANTERIOR
        nodoAnterior=cabezaAuxiliar;
        //PARA RECORRER LA LISTA SE DIRECCIONA LA CABEZA AUXILIAR AL PROXIMO LOCUTOR DE LA LISTA
        cabezaAuxiliar=cabezaAuxiliar->sig;
    }
    //SI EXISTE EL LOCUTOR CON LA CEDULA
    if(cabezaAuxiliar){
        //SI ES EL PRIMER LOCUTOR
        if(cabezaAuxiliar==estacion_de_radio->lista_de_locutores)
           estacion_de_radio->lista_de_locutores=cabezaAuxiliar->sig;

        else{
            nodoAnterior->sig=cabezaAuxiliar->sig;
            free(cabezaAuxiliar);
        }
        printf("Locutor despedido\n");
    }
    return estacion_de_radio->lista_de_locutores;
}

static void modificarRegistroLocutor(Radio * estacion_de_radio){
    system("cls");
    if(estacion_de_radio->lista_de_locutores){
            Locutor *locutorAModificar;
            locutorAModificar=(Locutor*)malloc(sizeof(Locutor));
            locutorAModificar->sig=NULL;
        //FUNCION DE  BUSQUEDA DEL LOCUTOR A PROCESAR
        if(locutorAModificar=buscarLocutor(estacion_de_radio,locutorAModificar)){
            int op=0;
            system("cls");
            do{
                //OPCIONES DE MODIFICACIÓN DEL LOCUTOR
                textcolor(WHITE);
                printf("[1] Modificar sueldo base\n");
                printf("[2] Despedir Locutor\n");
                printf("[3] Cancelar\n");
                textcolor(YELLOW);
                printf("Ingrese la opcion: ");
                textcolor(WHITE);scanf("%i",&op);
                FILE *file;
                switch(op){
                   //MODIFICAR EL SUELDO BASE DEL LOCUTOR
                    case 1:
                            file=fopen(ARCHIVO_LOCUTORES,"r+b");
                            if(!file){
                             errorCritico();
                            }else{
                                system("cls");
                                printf("Ingrese el nuevo sueldo para el locutor %s:",locutorAModificar->persona_locutor.nombre);
                                scanf("%i",&locutorAModificar->empleado_locutor.sueldo);
                                fseek(file,(sizeof(Locutor)*(locutorAModificar->empleado_locutor.id))-sizeof(Locutor),0);
                                    fwrite(locutorAModificar,sizeof(Locutor),1,file);
                                printf("Locutor Modificado..\n");
                                fclose(file);
                                getch(); controlDeUsuarios(estacion_de_radio); break;
                            }
                    //DESPEDIR LOCUTOR
                    case 2: //SE ABRE EN MODO W PARA QUE SOBREESCRIBA
                            file=fopen(ARCHIVO_LOCUTORES,"w");
                            if(!file){
                             errorCritico();
                            }else{
                                system("cls");
                                estacion_de_radio->lista_de_locutores=eliminarLocutor(estacion_de_radio,locutorAModificar);
                                Locutor *aux=estacion_de_radio->lista_de_locutores;
                                int id=1;
                                rewind(file);
                                while(aux){
                                    aux->empleado_locutor.id=id;
                                    fwrite(aux,sizeof(Locutor),1,file);
                                    id++;
                                    aux=aux->sig;
                                }
                                printf("Locutor Modificado..\n");
                                fclose(file);
                                pausar(); controlDeUsuarios(estacion_de_radio); break;
                            }
                    //REGRESAR AL MENU PRINCIPAL
                    case 3: menu(estacion_de_radio); break;
                    default : mensajePorDefecto(); break;
                }
            }while(op!=3);
        }
    }else{
        printf("No hay registros en el archivo");
        pausar();
    }

}

void registroSecretaria(Radio *estacion_de_radio){
	system("cls");

    Secretaria *nuevaSecretaria;
    nuevaSecretaria=(Secretaria*)calloc(1,sizeof(Secretaria));

    if(!nuevaSecretaria){
    	errorCritico();
    }
    else{
    	FILE * file=fopen(ARCHIVO_SECRETARIAS,"rb");
	    int id=0,valido=0;

	    fseek(file,(sizeof(Secretaria)*(-1)),SEEK_END);
	    if(fread(nuevaSecretaria,sizeof(Secretaria),1,file))
	    	id=nuevaSecretaria->empleado_secretaria.id;

	    do{
        textcolor(WHITE);
	    printf("Ingrese el nombre de la nueva secretaria:                                  \n");
	    fflush(stdin);
	    gets(nuevaSecretaria->persona_secretaria.nombre);
	    valido=validarNombre(nuevaSecretaria->persona_secretaria.nombre);
            if(!valido){
                textcolor(RED);
                printf("El nombre no puede tener espacios o ser mas largo de 20 letras\n");
                Sleep(2000);
                system("cls");
            }
        }while(!valido);

	     do{
        textcolor(WHITE);
	    printf("Ingrese el apellido del nueva secretaria:                                  \n");
	    fflush(stdin);
	    gets(nuevaSecretaria->persona_secretaria.apellido);
	    valido=validarNombre(nuevaSecretaria->persona_secretaria.apellido);
            if(!valido){
                textcolor(RED);
                printf("El apellido no puede tener espacios o ser mas largo de 20 letras\r");
                Sleep(2000);
            }
        }while(!valido);

	    do{
            textcolor(WHITE);
            printf("Ingrese la cedula de la secretaria                                \n");
            scanf("%i",&nuevaSecretaria->persona_secretaria.cedula);
            valido=validarCedula(estacion_de_radio,nuevaSecretaria->persona_secretaria.cedula);
            if(!valido){
                textcolor(RED);
                printf("La cedula que ingreso es incorrecta o ya esta registrada \r");
                Sleep(2000);
            }
	    }while(!valido);

	    do{
            textcolor(WHITE);
            printf("Ingrese la edad de la secretaria                                \n");
            scanf("%i",&nuevaSecretaria->persona_secretaria.edad);
            valido=validarEdad(nuevaSecretaria->persona_secretaria.edad);
            if(!valido){
                textcolor(RED);
                printf("La edad valida tiene que estar entre 18 y 80 a%cos \r",164);
                Sleep(2000);
            }
	    }while(!valido);

	    do{
            textcolor(WHITE);
            printf("Ingrese el sueldo de la secretaria                                \n");
            scanf("%i",&nuevaSecretaria->empleado_secretaria.sueldo);
            valido=validarSueldo(nuevaSecretaria->empleado_secretaria.sueldo);
            if(!valido){
                textcolor(RED);
                printf("El sueldo tiene que estar entre el presupuesto \r");
                Sleep(2000);
            }
	    }while(!valido);

	    if(valido){
            id++;
            nuevaSecretaria->empleado_secretaria.id=id;
            nuevaSecretaria->empleado_secretaria.activo=1;
            nuevaSecretaria->sig=NULL;
            fclose(file);
            ingresarSecretariaALaLista(estacion_de_radio,nuevaSecretaria);
            guardarSecretaria(nuevaSecretaria);
	    }
    }
}

void mostrarListaDeLocutores(){
	system("cls");
	FILE *file;
	file=fopen(ARCHIVO_LOCUTORES,"rb");
	Locutor locutorALeer;

	printf("%-10s %-20s %-10s %-10s %-10s %-10s %-10s %-10s\n"," ","Id Locutor","Nombre","Apellido","Cedula","Edad","Sueldo","Nombre del programa");
	while(fread(&locutorALeer, sizeof(Locutor),1, file)){
	 	printf("%-10s %-20i %-10s %-10s %-10i %-10i %-10i %-10s \n"," ",locutorALeer.empleado_locutor.id,
	    locutorALeer.persona_locutor.nombre,
	    locutorALeer.persona_locutor.apellido,
	    locutorALeer.persona_locutor.cedula,
	    locutorALeer.persona_locutor.edad,
	    locutorALeer.empleado_locutor.sueldo,
	    "Nombre del programa");
	}
	printf("\n");
	printf("\t \t \t \t  PRESIONA CUALQUIER TECLA PARA VOLVER...");
    getch();
}

//MOSTRAR LISTA ENLAZADA
void mostrarListaDeLocutores2(Radio *estacion_de_radio){
	Locutor *listaAuxiliar;

	listaAuxiliar=estacion_de_radio->lista_de_locutores;
	system("cls");
	printf("%-10s %-20s %-10s %-10s %-10s %-10s %-10s %-10s\n"," ","Id Locutor","Nombre","Apellido","Cedula","Edad","Sueldo","Nombre del programa");
	while(listaAuxiliar){
		printf("%-10s %-20i %-10s %-10s %-10i %-10i %-10i %-10s \n"," ",listaAuxiliar->empleado_locutor.id,
	    listaAuxiliar->persona_locutor.nombre,
	    listaAuxiliar->persona_locutor.apellido,
	    listaAuxiliar->persona_locutor.cedula,
	    listaAuxiliar->persona_locutor.edad,
	    listaAuxiliar->empleado_locutor.sueldo,
	    "Nombre del programa");
		listaAuxiliar=listaAuxiliar->sig;
	}
	printf("\n");
	printf("\t \t \t \t  PRESIONA CUALQUIER TECLA PARA VOLVER...");
    getch();
}
//MOSTRAR LOS REGISTROS EN LOS ARCHIVOS
void mostrarListaDeSecretarias(){
	system("cls");
	FILE *file;
	file=fopen(ARCHIVO_SECRETARIAS,"rb");
	Secretaria secretariaALeer;

	printf("%-10s %-20s %-10s %-10s %-10s %-10s %-10s %-10s\n"," ","Id Locutor","Nombre","Apellido","Cedula","Edad","Sueldo","Nombre del programa");
	while(fread(&secretariaALeer, sizeof(Locutor),1, file)){
	 	printf("%-10s %-20i %-10s %-10s %-10i %-10i %-10i %-10s \n"," ",secretariaALeer.empleado_secretaria.id,
	    secretariaALeer.persona_secretaria.nombre,
	    secretariaALeer.persona_secretaria.apellido,
	    secretariaALeer.persona_secretaria.cedula,
	    secretariaALeer.persona_secretaria.edad,
	    secretariaALeer.empleado_secretaria.sueldo,
	    "Nombre del programa");
	}
	printf("\n");
	printf("\t \t \t \t  PRESIONA CUALQUIER TECLA PARA VOLVER...");
    getch();
}
//ELIMINAR CADA NODO DE LA LISTA
void eliminarListaLocutor(Radio * estacion_de_radio){
	Locutor *listaAuxiliar;
	while(estacion_de_radio->lista_de_locutores){
		listaAuxiliar=estacion_de_radio->lista_de_locutores;
		estacion_de_radio->lista_de_locutores=estacion_de_radio->lista_de_locutores->sig;
		free(listaAuxiliar);
	}
}

