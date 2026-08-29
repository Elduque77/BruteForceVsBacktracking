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
        {1, 4, A1, "A1", "cd1874ea9fc4124aaff12e67c1a63d1ca1026b7ff0fa8233de38f2136dd856d7"},
        {2, 4, A2, "A2", "ca53174dd43dfeef5f6fcfbc0dc581221c7bcde985dc12eb2587cc325d4d0d45"},
        {3, 5, A1, "A1", "642727eb47a75fb412674579ddb4dda9f0e955b0cae3e8bddd7a328711344b61"},
        {4, 5, A2, "A2", "45de79e862967e4cd518ceb85ff70cead32426b4fc2e87939df1e07a45c38671"},
        {5, 6, A1, "A1", "cf616a632b59ea488a6bee1b24749a83018d57807f4c09b4bad88272a0fc023b"}
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