
# Sistema de Gestión de Tickets de Soporte Técnico

Este proyecto implementa una aplicación en C para administrar tickets de soporte técnico en una empresa, siguiendo un sistema de colas con prioridades.

Cada ticket contiene:
- Un ID único
- Numero de llegada (orden en el cual se ingresaron los tickets)
- Una descripción del problema
- Un nivel de prioridad (`Alto`, `Medio` o `Bajo`)
- La hora de registro

La aplicación permite registrar, modificar, buscar y procesar tickets, priorizando siempre los más urgentes y antiguos.

---

## 🚀 Instalación y ejecución

Para compilar el proyecto, abre una terminal en la raíz del proyecto y ejecuta el siguiente comando:

```bash
gcc tdas/*.c tarea1.c -Wno-unused-result -o tarea1
```

Luego, para ejecutar el programa:

```bash
./tarea1
```

---

## 🧠 Funcionalidades

1. **Registrar ticket**
   - Ingresa un ID y una descripción del problema.
   - Se registra automáticamente con prioridad **Bajo** y hora actual.

2. **Asignar prioridad a ticket**
   - Permite actualizar la prioridad de un ticket dado su ID.

3. **Mostrar tickets pendientes**
   - Muestra todos los tickets ordenados por prioridad (Alto > Medio > Bajo) y dentro de la misma prioridad, por hora de registro.

4. **Procesar siguiente ticket**
   - Atiende y elimina el ticket con mayor prioridad y más antiguo.

5. **Buscar ticket por ID**
   - Muestra los datos del ticket si existe, o un mensaje de error si no está.

---

## 📦 Uso de TDAs

El sistema utiliza un **TDA de Lista** para representar las colas de prioridad. En particular:
- Se utilizan **listas dobles enlazadas** para almacenar los tickets.
- Se emplea una **lista por cada nivel de prioridad**: Alto, Medio y Bajo.
- Esto permite simular un sistema de **colas con prioridad** al recorrer primero las listas más urgentes.

> Técnicamente no se implementó una estructura `Queue`, pero el comportamiento final representa una **cola con prioridad**.

---

## 📋 Ejemplo de uso

```
=== SISTEMA DE TICKETS ===
1. Registrar ticket
2. Asignar prioridad
3. Mostrar tickets pendientes
4. Procesar siguiente ticket
5. Buscar ticket por ID
6. Salir
Opcion:
```

- Opción 1: Ingresas un ID y descripción. El ticket se registra automáticamente con prioridad "Bajo".
- Opción 2: Puedes cambiar la prioridad del ticket (Alto, Medio, Bajo).
- Opción 3: Muestra todos los tickets ordenados por prioridad y antigüedad.
- Opción 4: Atiende y elimina el ticket más urgente.
- Opción 5: Busca un ticket específico por su ID.

---

## ⚠️ Funcionalidades no implementadas o limitaciones

- [ ] No se guarda la información en archivos, es decir, los datos se pierden al cerrar el programa.
- [ ] No se validan ciertos errores como IDs duplicados al registrar un nuevo ticket.
- [ ] Al ingresar mas de 300 caracteres en la descripcion en menu se ejecuta una cantidad exageradad de veces.


---

## 🛠️ Contribución individual

Este proyecto fue desarrollado de manera individual por [Tu Nombre Aquí]. Me encargué de:
- Implementar la lógica de registro y procesamiento de tickets
- Organizar los tickets en estructuras con prioridad
- Usar listas dobles enlazadas como base del sistema

---

## 📄 Licencia

Este proyecto es parte de una tarea universitaria y su uso está limitado a fines académicos.
