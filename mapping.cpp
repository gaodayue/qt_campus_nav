#include "mapping.h"

Mapping::Mapping()
{
}

Mapping::~Mapping()
{
}

int Mapping::xPos(double longitude){
    double x = (longitude-LON_0)*(X_RATIO);
    return (int)round(x);
}

int Mapping::yPos(double latitude){
    double y = (latitude-LAT_0)*(Y_RATIO);
    return (int)round(y);
}

double Mapping::latitude(int y){
    double latitude = (y * LAT_RATIO) + (LAT_0);
    return latitude;
}

double Mapping::longitude(int x){
    double longitude = (x * LON_RATIO) + (LON_0);
    return longitude;
}
