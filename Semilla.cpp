#include <iostream>
#include <string>
#include <vector>
#include "src/third_party/picosha2.h"

int main() {
    // Aqui defino la cadena de los apellidos en orden
    std::string apellidos = "duquegallegohenaomejia";
    
    // Aqui llevo a cabo el Cálculo de la semilla
    long long suma_ascii = 0;
    for (char c : apellidos) {
        suma_ascii += static_cast<int>(c);
    }
    long long semilla = suma_ascii % 100000;
    std::cout << "Cadena de apellidos: " << apellidos << std::endl;
    std::cout << "Suma ASCII: " << suma_ascii << std::endl;
    std::cout << "Semilla generada: " << semilla << std::endl << std::endl;

    // Aqui defino los dos alfabetos que utilizaremos
    std::string A1 = "abcdefghijklmnopqrstuvwxyz";
    std::string A2 = "abcdefghijklmnopqrstuvwxyz0123456789";

    // Aqui defino las variables de configuración para las contraseñas
    struct Config {
        int longitud;
        std::string alfabeto;
        std::string nombre_alfabeto;
    };

    // Aqui defini las longitudes que van a tener las contraseñas dependiendo del alfabeto que se utilice
    std::vector<Config> configs = {
        {4, A1, "A1"},
        {4, A2, "A2"},
        {5, A1, "A1"},
        {5, A2, "A2"},
        {6, A1, "A1"}
    };

    // Aqui defini el generador congruencial final para generar las contraseñas
    long long xi = semilla;
    const long long m = 2147483648LL; // 2^31

    for (size_t k = 0; k < configs.size(); ++k) {
        long long xi = semilla; 
        std::string pwd = "";
        for (int i = 0; i < configs[k].longitud; ++i) {
            xi = (1103515245 * xi + 12345) % m;
            int idx = xi % configs[k].alfabeto.length();
            pwd += configs[k].alfabeto[idx];
        }
        std::cout << "Contrasena " << (k + 1) << " (n=" << configs[k].longitud 
                  << ", Alfabeto=" << configs[k].nombre_alfabeto << "): " << pwd << std::endl;

        // Aqui calculo el hash SHA-256 de la contraseña generada            
        std::string hash_sha256 = picosha2::hash256_hex_string(pwd);

        std::cout << "  Hash SHA-256: " << hash_sha256 << std::endl << std::endl;

    }

    return 0;
}