#include <iostream>
#include <string>
#include <vector>
#include <fstream> // Necesario para crear y escribir en archivos
#include "Tipos.h"
#include "CargadorDatos.h"
#include "KMeans.h" // ¡Ahora sí conectamos el cerebro matemático!

using namespace std;

int main(int argc, char* argv[]) {
    // Validar argumentos de entrada requeridos por NagySoft
    if (argc != 3) {
        cerr << "Uso incorrecto. Debe ser: cluster <k> <datos.csv>" << endl;
        return 1;
    }

    int k = stoi(argv[1]);
    string archivoDatos = argv[2];

    if (k <= 0 || k > 6) {
        cerr << "El numero de clusters (k) debe estar entre 1 y 6." << endl;
        return 1;
    }

    cout << "Iniciando clasificacion K-Means..." << endl;
    cout << "Clusters a buscar (k): " << k << endl;
    cout << "Archivo de origen: " << archivoDatos << endl;

    // 1. Cargar los datos (vector inmutable)
    const vector<Coord_3D> datos = cargarDatos(archivoDatos);

    if (datos.empty()) {
        cerr << "No se pudieron cargar datos para procesar. Abortando." << endl;
        return 1;
    }

    cout << "Se cargaron exitosamente " << datos.size() << " puntos." << endl;

    // 2. Ejecutar el algoritmo KMeans
    vector<Cluster> resultados = ejecutarKMeans(datos, k);

    // 3. Generar el archivo clasificados.csv
    ofstream fileClasificados("clasificados.csv");
    if (fileClasificados.is_open()) {
        for (const auto& cluster : resultados) {
            for (const auto& p : cluster.points) {
                // Escribimos: x,y,z,Etiqueta
                fileClasificados << p.x << "," << p.y << "," << p.z << "," << cluster.label << "\n";
            }
        }
        fileClasificados.close();
        cout << ">> Archivo clasificados.csv generado exitosamente." << endl;
    } else {
        cerr << "Error: No se pudo crear clasificados.csv" << endl;
    }

    // 4. Generar el archivo summary.txt
    ofstream fileSummary("summary.txt");
    if (fileSummary.is_open()) {
        for (const auto& cluster : resultados) {
            double md = calcularDispersion(cluster); // Calculamos la dispersión de este cluster
            
            // Formato estricto: A: N, (x, y, z), MD
            fileSummary << cluster.label << ": " 
                        << cluster.points.size() << ", (" 
                        << cluster.centroid.x << ", " 
                        << cluster.centroid.y << ", " 
                        << cluster.centroid.z << "), " 
                        << md << "\n";
        }
        fileSummary.close();
        cout << ">> Archivo summary.txt generado exitosamente." << endl;
    } else {
        cerr << "Error: No se pudo crear summary.txt" << endl;
    }

    cout << "Proceso completado." << endl;
    return 0;
}