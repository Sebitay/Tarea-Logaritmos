# Ejecución de los experimentos:

1. Compilar el archivo main utilizando

```bash
g++ -g main.cpp 
```

2. Llamar al archivo ejecutable.

Este ejecutara el codigo para varios tamaños de N comparando ambos resultados.

```bash
./a.out
```

# Graficar resultados

### Los graficos iniciales se crearon usando el archivo output-final.txt si desea graficar estos resultados cambie

```python
#Access the output file
file = open('output.txt', 'r')
lines = file.readlines()
```

### por

```python
#Access the output file
file = open('output-final.txt', 'r')
lines = file.readlines()
```

1. (Opcional) Es recomendado crear un ambiente virtual de python con el comando:

```bash
python -m venv venv
```

2. Instalar dependencias 

```bash
pip install -r requirements.txt
```

3. Entrar al archivo graph.ipynb y correr todos los codigos en orden