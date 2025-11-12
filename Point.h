#ifndef POINT_H
#define POINT_H

#include <cmath> // para raiz cuadrada

// Define un vertice en la malla

struct Point
{
  float X, Y, Z;

  // Calcula la distancia euclidiana entre este punto (this) y otro punto "b"
  // Basicamente define el peso/costo de las aristas
  float distanceTo(const Point& b) const
    {
      float x = X - b.X;
      float y = Y - b.Y;
      float z = Z - b.Z;
      return(std::sqrt((x * x) + (y * y) + (z * z)) );
    }
};

#endif // POINT_H