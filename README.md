# BruteForceVsBacktracking

Este repositorio contiene la implementación, validación empírica y análisis comparativo de dos enfoques de diseño algorítmico: **Fuerza Bruta (Búsqueda Exhaustiva)** y **Backtracking (Búsqueda con Poda de Estados)**. El objetivo principal es evaluar la eficiencia y escalabilidad de ambos métodos al resolver el problema de recuperación de contraseñas a partir de sus valores hash criptográficos SHA-256.

---

##  Descripción del Proyecto

El proyecto explora la resistencia de esquemas de autenticación basados en funciones de dispersión unidireccionales ($SHA-256$). A partir de un conjunto de instancias sintéticas de prueba generadas secuencialmente con un Generador Congruencial Lineal (LCG), se evalúa cómo el incremento en la longitud de la clave ($n$) y la expansión del tamaño del alfabeto ($\Sigma$) impactan el tiempo de cómputo y el espacio de búsqueda.

###  Generación de Instancias Sintéticas
Las contraseñas de prueba fueron generadas determinísticamente utilizando la semilla $2320$ (derivada de la suma de códigos ASCII del grupo de trabajo) y los siguientes conjuntos de caracteres:
* **Alfabeto A1 (|A| = 26$):** Letras minúsculas del alfabeto inglés (`a-z`).
* **Alfabeto A2 (|A| = 36$):** Letras minúsculas y dígitos numéricos (`a-z`, `0-9`).

---

##  Tecnologías y Requisitos

* **Lenguaje de Programación:** C++17
* **Compilador:** `g++` (GCC bajo MSYS2 / UCRT64)
* **Biblioteca Hash:** `picosha2.h` (*Header-only implementation* para SHA-256)
* **Medición de Tiempo:** `std::chrono::high_resolution_clock`

---

##  Compilación y Ejecución
Para compilar y ejecutar los módulos, utiliza la terminal de comandos:

**1. Compilar el módulo de Fuerza Bruta**
g++ -std=c++17 FuerzaBruta.cpp -o FuerzaBruta.exe

**2. Ejecutar Fuerza Bruta**
.\FuerzaBruta.exe

**3. Compilar el módulo de Backtracking**
g++ -std=c++17 Backtracking.cpp -o Backtracking.exe

**4. Ejecutar Backtracking**
.\Backtracking.exe
