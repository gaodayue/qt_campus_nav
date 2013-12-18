#include "mapping.h"

Mapping::Mapping()
{
}

Mapping::~Mapping()
{
}

int Mapping::xPos(double longitude){
    double x = (longitude-LON_0)/(X_RATIO);
    return (int)round(x);
}

int Mapping::yPos(double latitude){
    double y = (latitude-LAT_0)/(Y_RATIO);
    return (int)round(y);
}
