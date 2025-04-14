#include "tdas/list.h"
#include "tdas/extra.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h> //HORA
#include <string.h> //Cadenas

//Definire Prioridad aqui
#define ALTA 3
#define MEDIA 2
#define BAJA 1

typedef struct tarea1
{
  char idCliente[50];
  int numeroLlegada;
  char descripcion[301];
  int prioridad; // 0 baja, 1 media, 2 alta
  time_t horaRegistro; //

}Ticket;

/*
Esta funcion limpia el buffer de entrada para evitar que queden caracteres residuales como '\n'
luego de un scanf o fgets, lo cual podría causar errores en futuras lecturas de datos.
*/
void limpiarBuffer()
{
  int c;
  while ((c = getchar()) != '\n' && c != EOF); 
}

// Menú principal
void mostrarMenuPrincipal() 
{
  limpiarPantalla();
  puts("========================================");
  puts("     Sistema de Gestion Servicio Tecnico");
  puts("========================================");

  puts("1) Registrar Ticket");
  puts("2) Asignar prioridad a ticket");
  puts("3) Mostrar lista de espera");
  puts("4) Atender al siguiente ticket");
  puts("5) Mostrar ticket por ID");
  puts("6) Salir");
}
/*
Esta funcion registra la informacion proporcionada por el cliente, su nombre (caracteres)
y una descripcion del problema que presenta para que luego el que gestiona el servicio pueda
asignar la urgencia de su problema, pero de momento se le asigna a todos la prioridad BAJA
ademas de asignarsele un numero por orden de llegada y se guarda la hora del registro.
Reserva memoria para el nuevo ticket y es guardado en la lisat de baja prioridad.
*/
void registrar_cliente(List *bajaPrioridad, int *numeroLlegada) 
{
  printf("Registrar nuevo ticket\n");
  char idCliente[50];
  char descripcion[301];

  do
  {
    printf("ID Cliente: ");
    fgets(idCliente, 50, stdin);
    idCliente[strcspn(idCliente, "\n")] = '\0';

    if(strlen(idCliente) == 0)
    {

      printf("Error: El ID del cliente no puede estar vacio. Ingrese ID valido.\n");
    }
  }while(strlen(idCliente)== 0);

  do
  {
    printf("Descripcion: (maximo 300 caracteres): ");
    fgets(descripcion,301 , stdin);
    descripcion[strcspn(descripcion, "\n")] = '\0';

    if(strlen(descripcion) == 0)
    {
      printf("Error: La descripcion no puede estar vacia.");
    }
  }while(strlen(descripcion)== 0);

  Ticket *nuevoTicket = (Ticket*)malloc(sizeof(Ticket));
  if(nuevoTicket == NULL){
    printf("ERROR AL ASIGNAR MEMORIA\n");
    return;
  }
  strcpy(nuevoTicket->idCliente, idCliente);
  strcpy(nuevoTicket->descripcion, descripcion);
  nuevoTicket->prioridad = BAJA;
  nuevoTicket->numeroLlegada = (*numeroLlegada)++; 
  nuevoTicket->horaRegistro = time(NULL);
  list_pushBack(bajaPrioridad, nuevoTicket);
}
/*
Busca un ticket por ID dentro de una lista (alta, media, baja)
Si lo encuentra, solicita una nueva prioridad al usuario y mueve el ticket a la lista que corresponda
Elimina el ticket de la lista original y lo reubica.
*/
int actualizarPrioridad(List *listaActual, List *altaPrioridad, List *mediaPrioridad, List *bajaPrioridad, char *idCliente)
{
  Ticket *ticket;
  //list_first(listaActual);
  for(ticket = list_first(listaActual); ticket != NULL; ticket = list_next(listaActual))
  {
  
    if(strcmp(ticket->idCliente, idCliente) == 0)
    {
        int nuevaPrioridad;
        do
        {
          printf("Nueva Prioridad (1 BAJA; 2 MEDIA; 3 ALTA): ");
          scanf("%d", &nuevaPrioridad);
          limpiarBuffer();

          if(nuevaPrioridad < 1 || nuevaPrioridad > 3){
            printf("Error: ingrese prioridad valida");

          }
        }while((nuevaPrioridad < 1 || nuevaPrioridad > 3));
        list_popCurrent(listaActual);

        switch(nuevaPrioridad)
        {
            case 1:
              list_pushBack(bajaPrioridad, ticket);
              break;

            case 2:
              list_pushBack(mediaPrioridad, ticket);
              break;
            
            case 3:
              list_pushBack(altaPrioridad, ticket);
              break;
        }
        return 1;
    }
  }
  return 0;
}
/*
Esta funcion busca segun el id del cliente y busca en que lisat esta posicionado para luego cambiar 
a la prioridad deseada
*/
void asignarPrioridad(List *altaPrioridad,List *mediaPrioridad, List *bajaPrioridad)
{
  char idCliente[50];
  printf("ID del Cliente: ");
  fgets(idCliente, 50, stdin);
  idCliente[strcspn(idCliente,"\n")] = '\0';

  if(actualizarPrioridad(altaPrioridad, altaPrioridad, mediaPrioridad, bajaPrioridad, idCliente))return;
  if(actualizarPrioridad(mediaPrioridad, altaPrioridad, mediaPrioridad, bajaPrioridad, idCliente))return;
  if(actualizarPrioridad(bajaPrioridad, altaPrioridad, mediaPrioridad, bajaPrioridad, idCliente))return;

  printf("Ticket no encontrado\n");  
}
/*
Esta función muestra por pantalla todos los tickets presentes en una lista dada.
Imprime: ID del cliente, número de llegada, hora de registro, descripción y nivel de prioridad.
*/
void mostrarTicketsEnLista(List* lista, char *prioridad)
{
  Ticket *ticket;
  for(ticket = list_first(lista); ticket != NULL; ticket = list_next(lista)){
    
    char buffer[80];
    struct tm *info = localtime(&ticket->horaRegistro);
    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", info);
    printf("ID: %s | Numero de llegada %d | Hora: %s | Descripcion: %s | Prioridad: %s\n", ticket->idCliente, ticket->numeroLlegada, buffer, ticket->descripcion, prioridad);

  }
}
/*
Muestra todos los tickets/clientes en espera por orden de prioridad 
si es que no hay clientes en espera muestar un mensaje de que quedan clientes.
*/
void mostrar_lista_clientes(List *altaPrioridad, List *mediaPrioridad, List *bajaPrioridad)
{
  if(list_size(altaPrioridad) == 0 && list_size(mediaPrioridad)== 0 && list_size(bajaPrioridad) == 0)
  {
    printf("No hay Clientes Pendientes\n");
  }
  else
  {
    printf("Clientes en espera: \n");
    mostrarTicketsEnLista(altaPrioridad, "ALTA");
    mostrarTicketsEnLista(mediaPrioridad, "MEDIA");
    mostrarTicketsEnLista(bajaPrioridad, "BAJA");
  }
}
/*
Esta funcion "atiende" a los clientes por orden de prioridad (alta, media, baja)
luego de atender libera memoria.
*/
void procesarTicketEnLista(List *lista, char *prioridad)
{
  Ticket *ticketProcesado = list_first(lista);
  list_popFront(lista);

  printf("Procesando ticket:\n");
  printf("ID: %s\n", ticketProcesado->idCliente);
  printf("Numero de llegada: %d\n", ticketProcesado->numeroLlegada);

  char buffer[80];
  struct tm *info = localtime(&ticketProcesado->horaRegistro);
  strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", info);

  printf("Hora Registro: %s\n", buffer);
  printf("Descripcion: %s\n", ticketProcesado->descripcion);

  printf("Prioridad: %s\n", prioridad);
  free(ticketProcesado);
}
/*
Determina cuál es el siguiente ticket a ser atendido según su prioridad:
primero los de ALTA, luego los de MEDIA y finalmente los de BAJA.
Procesa y elimina ese ticket de su lista.
*/
void procesarSiguiente(List* altaPrioridad, List *mediaPrioridad, List *bajaPrioridad)
{
  if(list_size(altaPrioridad) > 0)
  {
    procesarTicketEnLista(altaPrioridad, "ALTA");
  }
  else if(list_size(mediaPrioridad) > 0)
  {
    procesarTicketEnLista(mediaPrioridad, "MEDIA");
  }
  else if(list_size(bajaPrioridad) > 0)
  {
    procesarTicketEnLista(bajaPrioridad, "BAJA");
  }
  else{
    printf("No hay tickets pendientes\n");
  }
}
/*
Busca un ticket específico en una lista, comparando el ID del cliente.
Si lo encuentra, imprime todos sus detalles incluyendo su prioridad.
*/
int mostrarTicketEnLista(List *lista, char *prioridad, char *idCliente){
  Ticket *ticket;
  for(ticket = list_first(lista); ticket != NULL;ticket = list_next(lista))
  {
    if(strcmp(ticket->idCliente, idCliente) == 0){
      printf("ID: %s\n", ticket->idCliente);
      printf("Numero de llegada: %d\n", ticket->numeroLlegada);

      char buffer[80];
      struct tm *info = localtime(&ticket->horaRegistro);
      strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", info);

      printf("Hora Registro: %s\n", buffer);
      printf("Descripcion: %s\n", ticket->descripcion);
      printf("Prioriodad: %s\n", prioridad);
      return 1;
    }
  }
  return 0;
}
/*
Pide el ID del cliente y busca su ticket en todas las listas.
cuando lo encuentra, muestra la información del ticket.
Si no existe, imprime que no se encontro el ticket.
*/
void mostrarTicketsPorID(List *altaPrioridad, List *mediaPrioridad, List *bajaPrioridad)
{
  char idCliente[50];
  printf("Ingrese el ID del cliente: ");
  fgets(idCliente, 50, stdin);
  idCliente[strcspn(idCliente, "\n")] = '\0';

  if(mostrarTicketEnLista(altaPrioridad, "ALTA", idCliente)) return;
  if(mostrarTicketEnLista(mediaPrioridad, "MEDIA", idCliente)) return;
  if(mostrarTicketEnLista(bajaPrioridad, "BAJA", idCliente)) return;

  printf("Ticket no encontrado\n");
}

int main() 
{
  char opcion;
  //List *tickets = list_create(); // puedes usar una lista para gestionar los pacientes
  List* altaPrioridad = list_create();
  List* mediaPrioridad = list_create();
  List* bajaPrioridad = list_create();
  
  int numeroLLegada = 1;

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opcion: ");
    scanf(" %c", &opcion); // Nota el espacio antes de %c para consumir el
    limpiarBuffer();

    switch (opcion) 
    {
    case '1':
      registrar_cliente(bajaPrioridad, &numeroLLegada);
      break;
    case '2':
      asignarPrioridad(altaPrioridad, mediaPrioridad, bajaPrioridad);
      break;
    case '3':
      mostrar_lista_clientes(altaPrioridad, mediaPrioridad, bajaPrioridad);
      break;
    case '4':
      procesarSiguiente(altaPrioridad, mediaPrioridad, bajaPrioridad);
      break;
    case '5':
      mostrarTicketsPorID(altaPrioridad, mediaPrioridad, bajaPrioridad);
      break;
    case '6':
      puts("Saliendo del sistema de Gestion de Clientes");
      break;
    default:
      puts("Opcion no valida. Por favor, intente de nuevo.");
    }
    presioneTeclaParaContinuar();

  } while (opcion != '6');

  // Liberar recursos, si es necesario
  list_clean(altaPrioridad);
  list_clean(mediaPrioridad);
  list_clean(bajaPrioridad);

  return 0;
}

