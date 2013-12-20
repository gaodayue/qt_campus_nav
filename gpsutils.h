#ifndef GPSUTILS_H
#define GPSUTILS_H

class GPSUtils
{
public:
    GPSUtils();
    static void get_location(double *longitude, double *latitude);
    static void record_location(double longitude, double latitude);
    static void read_location(double *longitude, double *latitude);
};

#endif // GPSUTILS_H
