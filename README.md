# Tarea-Logaritmos

Crear un archivo para cada metodo

Crear un archivo para la estructura del M-Tree

Crear un archivo para generar los puntos


# Instrucciones para ejecutar los experimentos:

1. Compilar el archivo main utilizando

```bash
gcc main.c -o out
```

2. Llamar al archivo ejecutable considerando 2 parámetros:

- construction_function: corresponde al método con que se construye el árbol. Puede ser cp o ss.
- p: corresponde a la potencia a la que se eleva 2 para generar los datos (N=2^p)

```bash
./out <construction_function> <p>
```