// backtracking_bt.cpp
// Modulo BT - Backtracking sobre una politica de contrasenas (CSP)
// Practica 1 - Fuerza Bruta y Backtracking - Analisis de Algoritmos
//
// Genera (parcialmente, con muestra acotada), cuenta y mide el numero de
// nodos visitados/podados al construir contrasenas de longitud n de forma
// incremental caracter por caracter, aplicando backtracking con poda.
//
// Variables (CSP): un caracter por posicion (0..n-1)
// Dominio: el alfabeto (minusculas + mayusculas + digitos + simbolos)
// Restricciones verificables sobre un prefijo parcial:
//   - conteo minimo de minusculas, mayusculas, digitos y simbolos
//   - prohibicion de dos caracteres identicos consecutivos
//
// Condicion de factibilidad (funcion de poda):
//   dado un prefijo de longitud pos con counts[] acumulados, el prefijo
//   solo puede extenderse a una solucion valida si, con las posiciones
//   restantes (n - pos), todavia es posible cubrir los minimos que faltan
//   por cada categoria. Si la suma de "faltantes" excede las posiciones
//   restantes, se abandona la rama sin generar ninguna extension (poda).

#include <bits/stdc++.h>
using namespace std;

// ---------------------------------------------------------------------
// Alfabeto base (69 simbolos segun el enunciado; aqui se usan los 5
// simbolos confirmados por el usuario: ! @ # $ %  -> 67 simbolos totales.
// Si el enunciado oficial trae 2 simbolos adicionales, agreguelos aqui.)
// ---------------------------------------------------------------------
static const string LOWER   = "abcdefghijklmnopqrstuvwxyz";
static const string UPPER   = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const string DIGITS  = "0123456789";
static const string SYMBOLS = "!@#$%";   // <-- editar aqui si hay mas simbolos

static string ALPHABET; // se arma en main()

enum Category { CAT_LOWER = 0, CAT_UPPER = 1, CAT_DIGIT = 2, CAT_SYMBOL = 3 };

static inline Category categoryOf(char c) {
    if (c >= 'a' && c <= 'z') return CAT_LOWER;
    if (c >= 'A' && c <= 'Z') return CAT_UPPER;
    if (c >= '0' && c <= '9') return CAT_DIGIT;
    return CAT_SYMBOL;
}

struct Policy {
    int n = 0;
    int minLower = 0, minUpper = 0, minDigit = 0, minSymbol = 0;
    bool noRepeatConsecutive = true;
};

// ---------------------------------------------------------------------
// Metricas globales (lo que el modulo pide cuantificar)
// ---------------------------------------------------------------------
static long long g_nodesVisited     = 0; // cada llamada a backtrack()
static long long g_leavesReached    = 0; // pos == n
static long long g_solutionsFound   = 0; // hojas que cumplen la politica
static long long g_prunedByPolicy   = 0; // veces que la poda por factibilidad actuo
static long long g_skippedConsec    = 0; // ramas no generadas por regla de no-consecutivos

static Policy g_pol;
static string g_prefix;
static string g_example;
static bool   g_haveExample = false;
static long long g_maxSample = 0;
static vector<string> g_sample;

// Corte de seguridad por tiempo (para instancias donde el arbol es
// astronomicamente grande, p. ej. n=8 con alfabeto de 67-69 simbolos).
static double g_maxSeconds = 0.0; // 0 = sin limite
static chrono::high_resolution_clock::time_point g_t0;
static bool g_timedOut = false;

static inline bool timeUp() {
    if (g_maxSeconds <= 0.0) return false;
    if ((g_nodesVisited & 0xFFFFF) != 0) return false; // revisar cada ~1M nodos (costo bajo)
    double elapsed = chrono::duration<double>(chrono::high_resolution_clock::now() - g_t0).count();
    if (elapsed > g_maxSeconds) { g_timedOut = true; return true; }
    return false;
}

// ---------------------------------------------------------------------
// Backtracking con poda
// ---------------------------------------------------------------------
void backtrack(int pos, array<int,4>& counts, char lastChar) {
    if (g_timedOut) return;
    g_nodesVisited++;
    if (timeUp()) return;

    if (pos == g_pol.n) {
        g_leavesReached++;
        bool ok = counts[CAT_LOWER]  >= g_pol.minLower  &&
                  counts[CAT_UPPER]  >= g_pol.minUpper  &&
                  counts[CAT_DIGIT]  >= g_pol.minDigit  &&
                  counts[CAT_SYMBOL] >= g_pol.minSymbol;
        if (ok) {
            g_solutionsFound++;
            if (!g_haveExample) { g_example = g_prefix; g_haveExample = true; }
            if ((long long)g_sample.size() < g_maxSample) g_sample.push_back(g_prefix);
        }
        return;
    }

    // --- Condicion de factibilidad (funcion de poda) ---
    int remaining = g_pol.n - pos;
    int needed = max(0, g_pol.minLower  - counts[CAT_LOWER])
               + max(0, g_pol.minUpper  - counts[CAT_UPPER])
               + max(0, g_pol.minDigit  - counts[CAT_DIGIT])
               + max(0, g_pol.minSymbol - counts[CAT_SYMBOL]);
    if (needed > remaining) {
        g_prunedByPolicy++;
        return; // PODA: ninguna extension de este prefijo puede ser valida
    }

    for (char c : ALPHABET) {
        if (g_pol.noRepeatConsecutive && c == lastChar) {
            g_skippedConsec++;
            continue;
        }
        Category cat = categoryOf(c);
        counts[cat]++;
        g_prefix.push_back(c);
        backtrack(pos + 1, counts, c);
        g_prefix.pop_back();
        counts[cat]--;
        if (g_timedOut) return;
    }
}

int main(int argc, char** argv) {
    ALPHABET = LOWER + UPPER + DIGITS + SYMBOLS;

    if (argc < 6) {
        cerr << "Uso: " << argv[0]
             << " n minLower minUpper minDigit minSymbol [maxSample=0] [maxSeconds=0]\n"
             << "Ejemplo (instancia de referencia comun, Seccion 9.2):\n"
             << "  " << argv[0] << " 6 2 1 1 1 5 0\n";
        return 1;
    }

    g_pol.n         = atoi(argv[1]);
    g_pol.minLower  = atoi(argv[2]);
    g_pol.minUpper  = atoi(argv[3]);
    g_pol.minDigit  = atoi(argv[4]);
    g_pol.minSymbol = atoi(argv[5]);
    g_pol.noRepeatConsecutive = true;
    g_maxSample  = (argc > 6) ? atoll(argv[6]) : 0;
    g_maxSeconds = (argc > 7) ? atof(argv[7])  : 0.0;

    cout << "=== Modulo BT - Backtracking sobre politica de contrasenas ===\n";
    cout << "Alfabeto (" << ALPHABET.size() << " simbolos): " << ALPHABET << "\n";
    cout << "Politica: n=" << g_pol.n
         << " minLower=" << g_pol.minLower
         << " minUpper=" << g_pol.minUpper
         << " minDigit=" << g_pol.minDigit
         << " minSymbol=" << g_pol.minSymbol
         << " sinConsecutivosIguales=true\n";
    if (g_maxSeconds > 0)
        cout << "Limite de tiempo de ejecucion: " << g_maxSeconds << " s (corte de seguridad)\n";
    cout << "\nEjecutando...\n";

    array<int,4> counts = {0,0,0,0};
    g_prefix.reserve(g_pol.n);

    g_t0 = chrono::high_resolution_clock::now();
    backtrack(0, counts, '\0');
    auto t1 = chrono::high_resolution_clock::now();
    double elapsed = chrono::duration<double>(t1 - g_t0).count();

    long double theoreticalSize = powl((long double)ALPHABET.size(), (long double)g_pol.n);

    cout << "\n--- METRICAS ---\n";
    if (g_timedOut) cout << "*** EJECUCION INTERRUMPIDA POR LIMITE DE TIEMPO (resultados parciales) ***\n";
    cout << "Nodos visitados (todas las llamadas a backtrack): " << g_nodesVisited << "\n";
    cout << "Hojas alcanzadas (cadenas completas evaluadas):   " << g_leavesReached << "\n";
    cout << "Ramas podadas por infactibilidad de politica:      " << g_prunedByPolicy << "\n";
    cout << "Transiciones evitadas por regla no-consecutivos:   " << g_skippedConsec << "\n";
    cout << "Soluciones encontradas (contrasenas validas):      " << g_solutionsFound << "\n";
    cout << "Tiempo de ejecucion: " << elapsed << " s\n";
    cout << "Tamano teorico del espacio de fuerza bruta |Sigma|^n = "
         << ALPHABET.size() << "^" << g_pol.n << " = "
         << fixed << setprecision(0) << (double)theoreticalSize << "\n";
    if (theoreticalSize > 0 && !g_timedOut) {
        long double reduccion = 100.0L * (1.0L - (long double)g_nodesVisited / theoreticalSize);
        cout << "Reduccion de nodos visitados vs espacio teorico: "
             << setprecision(6) << (double)reduccion << " %\n";
    }
    if (g_haveExample) cout << "\nEjemplo de contrasena valida: " << g_example << "\n";
    else                cout << "\nNo se encontro ninguna contrasena valida (politica infactible o no completado).\n";

    if (g_maxSample > 0 && !g_sample.empty()) {
        cout << "\nMuestra de soluciones (hasta " << g_maxSample << "):\n";
        for (auto &s : g_sample) cout << "  " << s << "\n";
    }
    return 0;
}