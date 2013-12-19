#ifndef MAPPING_H
#define MAPPING_H

#include <math.h>
#include <stdio.h>

#define PI 3.14159265
#define WIDTH 960
#define HEIGHT 544
#define LON_RATIO 0.000008985640973
#define LAT_RATIO -0.000006821052714
#define X_RATIO 111288.666328474
#define Y_RATIO -146604.936492225
#define LAT_0 39.9527550177562
#define LON_0 116.337367080619

class Mapping
{
public:
    Mapping();
    ~Mapping();
    int xPos(double);
    int yPos(double);
    double latitude(int y);
    double longitude(int x);

private:

};

#endif // MAPPING_H
