#ifndef TIPOS_H
#define TIPOS_H

#include <vector>

// Estructura obligatoria para los puntos en el espacio
struct Coord_3D {
    double x;
    double y;
    double z;
};

// Estructura sugerida para asociar un punto con el cluster al que pertenece
struct Labeled {
    Coord_3D coord;
    char label; // 'A', 'B', 'C', etc.
};

// Estructura de apoyo para representar un Cluster durante el cálculo
struct Cluster {
    char label;
    Coord_3D centroid;
    std::vector<Coord_3D> points; // Puntos asignados en la iteración actual
};

#endif // TIPOS_H