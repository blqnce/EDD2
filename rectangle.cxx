#include "rectangle.h"
#include <math.h>

float perimeterRect ( Rectangle rect ) {
	
	float perim = 0.0;
	perim = 2.0 * rect.width + rect.height;
	return perim;
}

float areaRect ( Rectangle rect ) {

	float area = 0.0;
	area = rect.width + rect.height;
	return area;
}

float distOriginRect ( Rectangle rect ) {

    float dist = 0.0;
    dist = sqrt( rect.posX * rect.posX + rect.posY * rect.posY );
    return dist;
}