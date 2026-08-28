#include <iostream>
#include <string>
#include <vector>
#include <chrono> // Para medir tiempos con std::chrono
#include "src/third_party/picosha2.h" // Biblioteca de hash SHA-256

bool exhaustivoRecursivo(
    std::string cadenaActual,
    int n,
    const std::string& alfabeto,
    const std::string& hashObjetivo,
    std::string& solucionEncontrada,
    unsigned long long& candidatosEvaluados
) {
    if (cadenaActual.length() == static_cast<size_t>(n)) {
        candidatosEvaluados++;
        std::string hashActual = picosha2::hash256_hex_string(cadenaActual);
        if (hashActual == hashObjetivo) {
            solucionEncontrada = cadenaActual;
            return true;
        }
        return false;
    }

    for (char c : alfabeto) {
        if (exhaustivoRecursivo(cadenaActual + c, n, alfabeto, hashObjetivo, solucionEncontrada, candidatosEvaluados)) {
            return true;
        }
    }
    return false;
}

struct ResultadoFB {
    std::string contrasena;
    bool encontrada;
    unsigned long long candidatosEvaluados;
    double tiempoMs;
};

ResultadoFB resolverFuerzaBruta(const std::string& hashObjetivo, const std::string& alfabeto, int n) {
    ResultadoFB resultado = {"", false, 0, 0.0};
    
    // Medición de tiempo con std::chrono
    auto inicio = std::chrono::high_resolution_clock::now();
    
    // Llamada inicial
    resultado.encontrada = exhaustivoRecursivo("", n, alfabeto, hashObjetivo, resultado.contrasena, resultado.candidatosEvaluados);
    
    auto fin = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duracion = fin - inicio;
    resultado.tiempoMs = duracion.count();
    
    return resultado;
}

int main() {
    // Definición de alfabetos según los requerimientos del proyecto
    const std::string A1 = "abcdefghijklmnopqrstuvwxyz";
    const std::string A2 = "abcdefghijklmnopqrstuvwxyz0123456789";

    // Estructura para almacenar la configuración de las instancias sintéticas
    struct Instancia {
        int id;
        int n;
        std::string alfabeto;
        std::string nombreAlfabeto;
        std::string hashObjetivo;
    };

    // Lista de las 5 instancias sintéticas generadas con la semilla del grupo (2320)
    std::vector<Instancia> instancias = {
        {1, 4, A1, "A1", "34ef425a850e0ebaa20f3214da3f82522858cdbe6cfd93fef8c72cf282ae5527"},
        {2, 4, A2, "A2", "e0fb144079ca963cfa3aa8c9ee4ea09be282f67645eb4efd6cf3017cfbf6ad38"},
        {3, 5, A1, "A1", "8cd6804ff2b0bbff9ef347c6a9bb7bb6bb088bead3fec9536fa2a1ef24a10ee9"},
        {4, 5, A2, "A2", "0f207c305a0808c50273c7f4df90b5cfa934544a25b7faccf8203c3b83ab26f8"},
        {5, 6, A1, "A1", "29f086beca586851fce207aab47831ded226751a0c7317b7dc5433e4774c73dd"}
    };

    std::cout << "======================================================================\n";
    std::cout << "          EJECUCION DEL MODULO DE FUERZA BRUTA (FB)                   \n";
    std::cout << "======================================================================\n\n";

    for (const auto& inst : instancias) {
        std::cout << "--> Evaluando Instancia " << inst.id 
                  << " (n=" << inst.n << ", Alfabeto=" << inst.nombreAlfabeto << ")...\n";
        
        ResultadoFB res = resolverFuerzaBruta(inst.hashObjetivo, inst.alfabeto, inst.n);

        if (res.encontrada) {
            std::cout << "  [EXITO] Contrasena Encontrada: " << res.contrasena << "\n";
        } else {
            std::cout << "  [FALLO] No se encontro la contrasena.\n";
        }
        std::cout << "  Candidatos Evaluados : " << res.candidatosEvaluados << "\n";
        std::cout << "  Tiempo Transcurrido  : " << res.tiempoMs << " ms\n";
        std::cout << "----------------------------------------------------------------------\n";
    }

    return 0;
}