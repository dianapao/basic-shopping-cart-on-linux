#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>
/*****Funciones*****/
void *HiloCatalogo();
void Proceso_Principal();
void *HiloRegistro();
void Proceso_Sesion();
void Productos_Proveedores();
void Agregar_Producto();
int Buscar_Producto();
void Actualizar_Producto();
short int Total_Lineas();
void Agregar_Carrito();
double Total_Ticket(int);
void Guardar_Ticket(char original[30]);

/*****Variables Globales*****/
char nombreg[50],correog[50],passwordg[20],a;
int sesion=0,opcion,compra;
/*****Codigo Main*****/
int main(){
	int pid;
	int salida;
	pid=fork();
	if(pid==-1){
		perror("Error al crear el proceso");
		exit(-1);
	}
	else if(pid==0){
		Proceso_Principal();
	}
	pid=wait(&salida);
}
/*****Proceso Principal*****/
void Proceso_Principal(){
	while(1){
		printf("*****Tienda Virtual*****\n");
		printf("Seleccione la opcion.\n1. Catalogo\n2. Carrito\n3. Cuenta\n4. Iniciar Sesion\n5. Proveedores\n6. Crear Usuario\n7. Salir\n");
		printf("PID: %d\n",getpid());
		scanf("%d",&opcion);
		fflush(stdin);
		while(opcion<1||opcion>7){
			printf("Selecione una opcion disponible.\n");
			scanf("%d",&opcion);
		}
		if(opcion==1){
			system("clear");
			pthread_t id_hiloCatalogo;
			pthread_create(&id_hiloCatalogo,NULL,HiloCatalogo,NULL);
			pthread_join(id_hiloCatalogo,NULL);
		}
		if(opcion==2){
			Agregar_Carrito();
		}
		if(opcion==3){
			if(sesion==1){
				system("clear");
				printf("*****Datos de la Cuenta*****\n");
				printf("Nombre: %s\nCorreo: %s\nPassword: %s",nombreg,correog,passwordg);
				printf("Presione enter para continuar y regresar al menu principal.\n");
				scanf("%c",&a);
				scanf("%c",&a);
				system("clear");
				//system("PAUSE");
			}else{
				system("clear");
				printf("Debe iniciar sesion antes.\n");
				Proceso_Sesion();
			}
		}
		if(opcion==4){
			Proceso_Sesion();
		}
		if(opcion==5){
			system("clear");
			Productos_Proveedores();
		}
		if(opcion==6){
			system("clear");
			pthread_t id_hiloRegistro;
			pthread_create(&id_hiloRegistro,NULL,HiloRegistro,NULL);
			pthread_join(id_hiloRegistro,NULL);
		}
		if(opcion==7)return;
	}
}
/*****Hilo del Catalogo*****/
void *HiloCatalogo(){
	FILE *catalogo;
	char texto[100];
	int cuenta;
	catalogo=fopen("Catalogo.txt","r");
	if(catalogo==NULL){
		printf("No se pudo abrir el archivo\n");
		exit(1);
	}
	printf("*****Catalogo*****\n");
	printf("PRODUCTO\tPRECIO\tExistencia \n");
	fgets(texto,100,catalogo);
	while(feof(catalogo)==0){
		printf("%s",texto);
		fgets(texto,100,catalogo);
	}
	if(fclose(catalogo)!=0)printf("Error al cerrar el archivo\n");
	printf("Desea realizar alguna compra.\n1. Si\n2. No\n");
	scanf("%d",&compra);
	if(compra==1){
		if(sesion==1);
		else{
			system("clear");
			printf("Tiene una cuenta.\n1. Si\n2. No\n");
			scanf("%d",&cuenta);
			if(cuenta==1){
				printf("Inicie sesion con su cuenta.\n");
				Proceso_Sesion();
				system("clear");
				pthread_exit(0);
			}
			else{
				system("clear");
				pthread_t id_hiloRegistro;
				pthread_create(&id_hiloRegistro,NULL,HiloRegistro,NULL);
				pthread_join(id_hiloRegistro,NULL);
			}
		}
	}
	system("clear");
	pthread_exit(0);
}
void *HiloRegistro(){
	char nombre[50];
	char correo[50];
	char password[20];
	char texto[80];
	printf("Introduzca su nombre completo.\n");
	fflush(stdin);
	fgets(nombre,50,stdin); //Se llama 2 veces debido a que detecta el salto de linea anterior
	fgets(nombre,50,stdin);
	printf("Introduzca su correo.\n");
	fflush(stdin);
	fgets(correo,50,stdin);
	printf("Introduzca una contrasena.\n");
	fflush(stdin);
	fgets(password,20,stdin);
	FILE *usuarios;
	usuarios=fopen("Usuarios.txt","r+");
	if(usuarios==NULL){
		printf("No se pudo abrir el archivo\n");
		exit(1);
	}
	fgets(texto,80,usuarios);
	while(feof(usuarios)==0){
		if(strcmp(correo,texto)==0){
			printf("Ya hay un usuario registrado con ese correo.\nRegresará a la pantalla de inicio.\n");
			if(fclose(usuarios)!=0)printf("Error al cerrar el archivo\n");
			sleep(2);
			system("clear");
			pthread_exit(0);
		}
		fgets(texto,50,usuarios);
	}
	fputs(nombre,usuarios);
	fputs(correo,usuarios);
	fputs(password,usuarios);
	fputs("------------------------\n",usuarios);
	system("clear");
	if(fclose(usuarios)!=0)printf("Error al cerrar el archivo\n");
	pthread_exit(0);
}
void Proceso_Sesion(){
	char correo[50];
	char password[20];
	char texto[80];
	int control=2;
	system("clear");
	printf("Introduzca su correo.\n");
	fflush(stdin);
	fgets(correo,50,stdin); //Se llama 2 veces debido a que detecta el salto de linea anterior
	fgets(correo,50,stdin);
	printf("Introduzca su contrasena.\n");
	fflush(stdin);
	fgets(password,20,stdin);
	FILE *usuarios;
	usuarios=fopen("Usuarios.txt","r");
	if(usuarios==NULL){
		printf("No se pudo abrir el archivo\n");
		exit(1);
	}
	fgets(texto,80,usuarios);
	while(feof(usuarios)==0){
		if(strcmp(correo,texto)==0){
			strcpy(correog,texto);
			control=0;
		}
		if(control==1&&strcmp(password,texto)==0){
			printf("Ha iniciado sesion correctamente.\n");
			sesion=1;
			strcpy(passwordg,texto);
			sleep(2);
			if(fclose(usuarios)!=0)printf("Error al cerrar el archivo\n");
			system("clear");
			return;
		}
		if(control>1){
			strcpy(nombreg,texto);
		}
		fgets(texto,50,usuarios);
		++control;
	}
	printf("Datos incorrectos.\n");
	if(fclose(usuarios)!=0)printf("Error al cerrar el archivo\n");
	sleep(2);
	system("clear");
	return;
}

void Mostrar_Catalogo(){
	FILE *catalogo;
	char texto[100];
	int cuenta;
	catalogo=fopen("Catalogo.txt","r");
	if(catalogo==NULL){
		printf("No se pudo abrir el archivo\n");
		exit(1);
	}
	printf("*****Catalogo*****\n");
	printf("PRODUCTO\tPRECIO\tExistencia \n");
	fgets(texto,100,catalogo);
	while(feof(catalogo)==0){
		printf("%s",texto);
		fgets(texto,100,catalogo);
	}
	if(fclose(catalogo)!=0)printf("Error al cerrar el archivo\n");
}

void Productos_Proveedores(){
	int option;
	do{
		printf("Seleccione una opción: \n");
		printf("1. Buscar artículo\n");
		printf("2. Agregar artículo\n");
		printf("3. Actualizar existencia\n");
		printf("4. Salir\n");

		scanf("%d", &option);
		switch(option){
			case 1:
				Buscar_Producto();
				break;
			case 2: Agregar_Producto();
				break;
			case 3: Actualizar_Producto();
				break;
		}
	}while(option!=4);

	return;
}

void Agregar_Producto(){
	FILE *ptrFile;
	ptrFile = fopen("Catalogo.txt", "a");
		if(ptrFile == NULL){
			printf("Error al abrir el archivo\n");
			exit(1);
		}

	char nombreP[30];	
	float precio;
	int cantidad;

	printf("Introduzca el nombre del producto: \n");
	fflush(stdin);
	fgets(nombreP,30,stdin); //Se llama 2 veces debido a que detecta el salto de linea anterior
	scanf("%[^\n]%*c", nombreP);
	fflush(stdin);

	printf("Precio del producto: \n$");
	scanf("%f", &precio);

	printf("Productos en existencia:\n");
	scanf ("%d", &cantidad);

	fputs(nombreP, ptrFile);
	fprintf(ptrFile, ", %f, ", precio);
	fprintf(ptrFile, "%d\n",cantidad );

	fclose(ptrFile);
}

int Buscar_Producto(){
	FILE *ptrFile;
	ptrFile = fopen("Catalogo.txt", "r");
		if(ptrFile == NULL){
			printf("Error al abrir el archivo\n");
			exit(1);
		}
	char nombreP[30];
	char search[30];
	int cantidad;
	float precio;
	short int flag=0, line=0;

	printf("Ingrese el nombre del producto: \n");
	fflush(stdin);
	fgets(search,30,stdin); //Se llama 2 veces debido a que detecta el salto de linea anterior
	scanf("%[^\n]%*c", search);
	fflush(stdin);

	while((fscanf(ptrFile, "%[^,]%*c %f%*c%d", nombreP, &precio, &cantidad))!=EOF){
		line++;
		if (nombreP[0] == '\n') 
    		memmove(nombreP, nombreP+1, strlen(nombreP));	//elimina el salto de línea del princio
		//printf("Igualdad: (%s,%s)=%d\n", search, nombreP, strcmp(search, nombreP));
		fflush(stdin);
		if(strcmp(nombreP,search)==0){
			system("clear");
			printf("\t\t\tProducto encontrado.\n");
			printf("\t producto:%s, precio:$%f, existencia:%d \n\n",nombreP, precio, cantidad );
			flag = 1;
			break;
		}
	}
	fclose(ptrFile);

	if(flag==0){
		system("clear");
		printf("El articulo no existe \n\n");
		return 0;
	}
	return line;
}

short int Total_Lineas(){
	FILE *ptrFile;
	ptrFile = fopen("Catalogo.txt", "r");
	if(ptrFile == NULL){
		printf("Error al abrir el archivo\n");
		exit(1);
	}
	char text[50];
	short int lines=0;
	while((fscanf(ptrFile, "%[^\n]%*c ", text))!=EOF){
		lines++;
	}
	fclose(ptrFile);
	return lines;
}

void Actualizar_Producto(){
	short int line = Buscar_Producto();		//busca en que línea se encuentra el producto a modificar
	short int totLineas = Total_Lineas();	//el total de productos que existen 

	char nombreP[30];
	int cantidad;
	float precio;
	short int aux=0;

	char original[30] = "Catalogo.txt";
	char copy[30] = "newText.txt";
	FILE *ptrFile, *ptrFileCopy;
	ptrFile = fopen("Catalogo.txt", "r");
	ptrFileCopy = fopen("newText.txt", "w");
	if(ptrFile == NULL){
		printf("Error al abrir el archivo\n");
		exit(1);
	}
	if(ptrFileCopy == NULL){
		printf("Error al abrir el archivo\n");
		exit(1);
	}

	while((fscanf(ptrFile, "%[^,]%*c %f%*c%d", nombreP, &precio, &cantidad))!=EOF && (totLineas--) ){
		aux++;
		if(aux == line){
			printf("Ingresa la nueva cantidad de productos en existencia: \n");
			scanf("%d", &cantidad);
		}
		memmove(nombreP, nombreP+1, strlen(nombreP));	//elimina el salto de línea del princio	
		fprintf(ptrFileCopy, "%s, ", nombreP);
		fprintf(ptrFileCopy, "%f, ", precio);
		fprintf(ptrFileCopy, "%d\n", cantidad);
	}

	fclose(ptrFileCopy);
	fclose(ptrFile);
	remove(original);	//eliminamos el primer archivo
	rename(copy,original);	//renombramos el nuevo como el anterior

	return;
}

void Guardar_Ticket(char original[30]){
	char newFile[30], ch;
	printf("Nombre para el archivo de este nuevo ticket: ");
	fflush(stdin);
	fgets(newFile,30,stdin); //Se llama 2 veces debido a que detecta el salto de linea anterior
	fgets(newFile,30,stdin);
	FILE *ptrFile, *ptrFileNew;
	ptrFile = fopen(original, "r");
	ptrFileNew = fopen(newFile, "w");

	if(ptrFile == NULL){
		printf("Error al abrir el archivo\n");
		exit(1);
	}
	if(ptrFileNew == NULL){
		printf("Error al abrir el archivo\n");
		exit(1);
	}

	while( ( ch = fgetc(ptrFile) ) != EOF )
      fputc(ch, ptrFileNew);

  	fclose(ptrFile);
	fclose(ptrFileNew);
	remove(original);	//eliminamos archivo original
}

void Agregar_Carrito(){
	int add = 1;
	int tot_productos_carrito=0;
	FILE *ptrFile, *ptrFileCopy, *ptrFileCarrito;
	
	do{
		Mostrar_Catalogo();
		printf("\n");
		short int line = Buscar_Producto();		//busca en que línea se encuentra el producto a agregar
		short int totLineas = Total_Lineas();	//total de productos en la lista

		if(line == 0){
			printf("Ese producto no existe. Intenta nuevamente \n");
			add=1;
			continue;
		}

		char nombreP[30];
		int cantidad;
		float precio;
		short int aux=0;

		char original[30] = "Catalogo.txt";
		char copy[30] = "newText";

		
		ptrFile = fopen(original, "r");
		ptrFileCopy = fopen(copy, "w");
		ptrFileCarrito = fopen("Carrito.txt", "a");

		if(ptrFile == NULL || ptrFileCopy == NULL || ptrFileCarrito == NULL){
			printf("Error al abrir el archivo\n");
			exit(1);
		}

		/*Vamos a actualizar el stock disminuyendo el uno la cantidad del producto seleccionado*/
		while((fscanf(ptrFile, "%[^,]%*c %f%*c%d", nombreP, &precio, &cantidad))!=EOF && (totLineas--) ){
			aux++;
			if(aux != 1)		//si no se trata del primer elemento de la lista
				memmove(nombreP, nombreP+1, strlen(nombreP));	//elimina el salto de línea del princio	
			if(aux == line){	//lo imprimimos en el ticket
				fprintf(ptrFileCarrito, "%s, ", nombreP);
				fprintf(ptrFileCarrito, "%f\n", precio);
				cantidad = cantidad-1;	//actualizamos el stock
				tot_productos_carrito = tot_productos_carrito+1;
			}
			fprintf(ptrFileCopy, "%s, ", nombreP);
			fprintf(ptrFileCopy, "%f, ", precio);
			fprintf(ptrFileCopy, "%d\n", cantidad);
		}

		printf("Producto agregado al carrito. \n");
		fclose(ptrFileCopy);
		fclose(ptrFile);
		fclose(ptrFileCarrito);
		remove(original);	//eliminamos el primer archivo
		rename(copy,original);	//renombramos el nuevo como el anterior

		printf("Deseas agregar otro producto al carrito?\n");
		printf("1. Si\n");
		printf("2. No\n");
		scanf("%d", &add);
	}while(add == 1);

	double tot = Total_Ticket(tot_productos_carrito);
	printf("\n \t Total a pagar: $ %lf \n", tot);

	ptrFileCarrito = fopen("Carrito.txt", "a");
	fprintf(ptrFileCarrito, "\n TOTAL: %lf ", tot);
	fclose(ptrFileCarrito);

	printf("Vamos a guardar el ticket: \n");
	Guardar_Ticket("Carrito.txt");

}

double Total_Ticket(int tot_productos_carrito){
	FILE *ptrFile;
	ptrFile = fopen("Carrito.txt", "r");
	if(ptrFile == NULL){
		printf("Error al abrir el archivo\n");
		exit(1);
	}

	char nombreP[30];
	float precio;
	double tot=0;
	
	while((fscanf(ptrFile, "%[^,]%*c %f%*c", nombreP, &precio))!=EOF && (tot_productos_carrito--)){
		tot += precio;
	}
	fclose(ptrFile);
	return tot;
}