# kernel data structure

Esta práctica consiste en una serie de ejercicios para familiarizarnos con el uso intermedio del lenguaje de programación C (Apuntadores y Estructuras). En el desarrollo de esta práctica trabajaremos con dos sistemas:

Una aplicación que correrá en el espacio de usuario y que se encargará de solicitarle la informacón al usuario y establecer comunicación con una aplicación que correrá en el espacio del kernel (un módulo/driver) y que se encargará de manipular las estructuras de datos ofrecidas por el kernel para cumplir con las funcionalidades indicadas en cada uno de los items de la práctica.

Esta práctica también nos permitirá aprender a utilizar ciertas funciones que el kernel de Linux ofrece para el trabajo con Pilas y Colas. Para resolver esta práctica se requiere que haga uso de dichas funciones.

1. Escriba un programa que lea las líneas de un archivo y las muestre en el orden inverso al orden en el que fueron leídas.
2. Escriba un programa que baraje aleatoriamente las líneas de un archivo.
3. Escriba un programa que analice un archivo de codigo fuente C y que verifique que los paréntesis y las llaves estén balanceadas. Esto significa que todo ( y { debe ser cerrado por un ) y } respectivamente.
4. Implemente una cola con prioridades que maneje tres niveles: alta, media y baja.
5. Implemente la función "destruirListaTotalmente" que destruya por completo una lista enlazada.
6. Implemente la funcion "invertir". Esa funcion debe invertir el orden de los nodos de una lista. No puede crear otra lista para realizar dicha tarea.
7. Implemente la función "concatenarListas", que genera una nueva lista compuesta por la concatenación de dos listas que se pasan como argumento. La nueva lista debe ser independiente de las listas originales.
8. Implemente una función "rotacionDerecha", que rota la lista N veces para la derecha. Una rotación a la derecha mueve el último dato de la lista al inicio de la misma.
9. Implemente la función "limpiarLista" que remueve los valores identicos.
10. Implemente una función que retorne el mayor valor de una lista.

## Commands

### Other way to compile module bridgeOwn and clientBridge

firstly, go to the folder /**kernel-data-structure-lab1** inside the project, run the command **sudo ./scripts/run_it_run**, this command will create symbolic links or just links to access in an easier way to commands like: bridge_load, bridge_reset also to clientBridge executable easier.

Now, if you want execute clientBridge you just to write:

```sh
sudo clientBridge
```

Here, command list:

```sh
sudo bridge_status
```
=> show if the module is on.
```sh
sudo bridge_load
```
=> load the module.
```sh
sudo bridge_reset
```
=> reset the module, but before, unload the module, it execute the make command and finally load the module again.
```sh
sudo bridge_syslog_clean
```
=> clean the syslog in /var/log/syslog. Don't worry it will make a backup.

```sh
sudo compile_client
```
=> compile the clientBridge from **kernel-data-structure-lab1** only. It will replace with Makefile in future versions.

Note: Some of these commands work just if you are in the **kernel-data-structure-lab1** path in his terminal or console.



### To compile module bridgeOwn


Into the project folder type

```sh
make

```

### To compile client

```sh

gcc -o clientBridge clientBridge.c utilsClientBridge.c moduleImplement/bridgeLink.c moduleImplement/bridgeIO.c

```

### Up module

```sh
sudo ./bridge_load

```

### Down module

```sh
sudo ./bridge_unload

```

### Checking it

```sh
ls /dev/
lsmod | grep bridgeOwn
```
