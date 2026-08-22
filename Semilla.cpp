#include <iostream>
#include <string>
#include <vector>
//HOLA MUNDO
int main() {
    // 1. Ingrese la cadena con los apellidos unidos en orden alfabético
    std::string apellidos = "duquegallegohenaomejia"; // <-- Reemplaza con los apellidos de tu equipo
    
    // 2. Cálculo de la semilla
    long long suma_ascii = 0;
    for (char c : apellidos) {
        suma_ascii += static_cast<int>(c);
    }
    long long semilla = suma_ascii % 100000;
    std::cout << "Cadena de apellidos: " << apellidos << std::endl;
    std::cout << "Suma ASCII: " << suma_ascii << std::endl;
    std::cout << "Semilla generada: " << semilla << std::endl << std::endl;

    // Defino alfabetos
    std::string A1 = "abcdefghijklmnopqrstuvwxyz";
    std::string A2 = "abcdefghijklmnopqrstuvwxyz0123456789";

    // Configuración de las 5 contraseñas (Longitud y Alfabeto)
    struct Config {
        int longitud;
        std::string alfabeto;
        std::string nombre_alfabeto;
    };

    std::vector<Config> configs = {
        {4, A1, "A1"},
        {4, A2, "A2"},
        {5, A1, "A1"},
        {5, A2, "A2"},
        {6, A1, "A1"}
    };

    // 3. Generador Congruencial Lineal (LCG)
    long long xi = semilla;
    const long long m = 2147483648LL; // 2^31

    for (size_t k = 0; k < configs.size(); ++k) {
        std::string pwd = "";
        for (int i = 0; i < configs[k].longitud; ++i) {
            xi = (1103515245 * xi + 12345) % m;
            int idx = xi % configs[k].alfabeto.length();
            pwd += configs[k].alfabeto[idx];
        }
        std::cout << "Contrasena " << (k + 1) << " (n=" << configs[k].longitud 
                  << ", Alfabeto=" << configs[k].nombre_alfabeto << "): " << pwd << std::endl;
    }

    return 0;
}