#include "CargadorDatos.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<Coord_3D> cargarDatos(const std::string& nombreArchivo) {
    std::vector<Coord_3D> datos;
    std::ifstream archivo(nombreArchivo);

    // Verificamos si el archivo existe y se puede abrir
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << std::endl;
        return datos; // Retorna un vector vacío
    }

    std::string linea;
    
    // Leemos el archivo línea por línea
    while (std::getline(archivo, linea)) {
        // Ignoramos líneas vacías que puedan existir al final del archivo
        if (linea.empty()) continue; 

        std::stringstream ss(linea);
        std::string valor;
        Coord_3D punto;

        try {
            // Extraemos los valores separados por coma y los convertimos a double (stod)
            if (std::getline(ss, valor, ',')) punto.x = std::stod(valor);
            if (std::getline(ss, valor, ',')) punto.y = std::stod(valor);
            if (std::getline(ss, valor, ',')) punto.z = std::stod(valor);

            datos.push_back(punto);
        } catch (const std::exception& e) {
            std::cerr << "Advertencia: Se encontro una linea con formato incorrecto y fue omitida." << std::endl;
        }
    }

    archivo.close();
    return datos;
}