// exercise1.cxx (Andrea Rueda)

// Main program that uses the functionality of rectangle.h

#include <iostream>
#include "rectangle.h"

using namespace std;

int main () {

	Rectangle rect1;
	int a, b;

	cout << "Ingrese coordenada X de la posicion del rectangulo: ";
	cin >> rect1.posX;
	cout << "Ingrese coordenada Y de la posicion del rectangulo: ";
	cin >> rect1.posY;
	cout << "Ingrese ancho del rectangulo: ";
	cin >> rect1.width;
	cout << "Ingrese alto del rectangulo: ";
	cin >> rect1.height;

	cout << "\nPerimetro del rectangulo: " << perimeterRect( rect1 ) << endl;
	cout << "Area del rectangulo: " << areaRect( rect1 ) << endl;
	cout << "Distancia del rectangulo al origen de coordenadas: " << distOriginRect( rect1 ) << endl;

	return 0;
}
