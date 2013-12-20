#include "gpsutils.h"

#include <QtCore>
#define GPS_RECORD_FILE "/gps/location.dat"

GPSUtils::GPSUtils()
{
}

void GPSUtils::get_location(double *longitude, double *latitude)
{

}

void GPSUtils::record_location(double longitude, double latitude)
{
    QFile file(GPS_RECORD_FILE);
    file.open(QIODevice::WriteOnly);

    QDataStream out(&file);
    out << (qreal) longitude << (qreal) latitude;
}

void GPSUtils::read_location(double *longitude, double *latitude)
{
    QFile file(GPS_RECORD_FILE);
    file.open(QIODevice::ReadOnly);

    QDataStream in(&file);
    in >> *longitude >> *latitude;
}
