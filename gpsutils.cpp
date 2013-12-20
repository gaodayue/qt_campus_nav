#include "gpsutils.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>

#include <QtCore>
#include <QDebug>
#define GPS_RECORD_FILE "/gps/location.dat"

GPSUtils::GPSUtils()
{
}

bool GPSUtils::parse_line(char *line, GPSData *data)
{
    if (strncmp(line, "$GPRMC,", 7) != 0)
        return false;

    double dumb;
    /* line must starts with header "$GPRMC," */
    line += 7;	/* skip header */

    sscanf(line,
           "%10s,%c,%lf,%c,%lf,%c,%lf,%lf,%6s",
           data->utc_time,
           &data->is_active,
           &data->latitude,
           &data->latitude_dir,
           &data->longitude,
           &data->longitude_dir,
           &data->ground_speed,
           &dumb,
           data->utc_day);

    data->utc_time[10] = '\0';

    // latitude is of form ddmm.mmmm
    // longitude is of form dddmm.mmmm
    data->latitude = ((int) data->latitude / 100) + (data->latitude - ((int) data->latitude / 100) * 100) / 60;
    data->longitude = ((int) data->longitude / 100) + (data->longitude - ((int) data->longitude / 100) * 100) / 60;

    return (data->is_active == 'A');
}

int GPSUtils::set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios newtio,oldtio;
    if  ( tcgetattr( fd,&oldtio)  !=  0) {
        perror("SetupSerial 1");
        return -1;
    }
    bzero( &newtio, sizeof( newtio ) );
    newtio.c_cflag  |=  CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;

    switch( nBits )
    {
    case 7:
        newtio.c_cflag |= CS7;
        break;
    case 8:
        newtio.c_cflag |= CS8;
        break;
    }

    switch( nEvent )
    {
    case 'O':
        newtio.c_cflag |= PARENB;
        newtio.c_cflag |= PARODD;
        newtio.c_iflag |= (INPCK | ISTRIP);
        break;
    case 'E':
        newtio.c_iflag |= (INPCK | ISTRIP);
        newtio.c_cflag |= PARENB;
        newtio.c_cflag &= ~PARODD;
        break;
    case 'N':
        newtio.c_cflag &= ~PARENB;
        break;
    }

    switch( nSpeed )
    {
    case 2400:
        cfsetispeed(&newtio, B2400);
        cfsetospeed(&newtio, B2400);
        break;
    case 4800:
        cfsetispeed(&newtio, B4800);
        cfsetospeed(&newtio, B4800);
        break;
    case 9600:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    case 115200:
        cfsetispeed(&newtio, B115200);
        cfsetospeed(&newtio, B115200);
        break;
    case 460800:
        cfsetispeed(&newtio, B460800);
        cfsetospeed(&newtio, B460800);
        break;
    default:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    }
    if( nStop == 1 )
        newtio.c_cflag &=  ~CSTOPB;
    else if ( nStop == 2 )
    newtio.c_cflag |=  CSTOPB;
    newtio.c_cc[VTIME]  = 0;
    newtio.c_cc[VMIN] = 0;
    tcflush(fd,TCIFLUSH);
    if((tcsetattr(fd,TCSANOW,&newtio))!=0)
    {
        perror("com set error");
        return -1;
    }
    printf("set done!\n\r");
    return 0;
}

void GPSUtils::get_location(double *longitude, double *latitude)
{
    GPSData data;
    int fd, count;
    char line[200];
    char c;

    if ((fd = open( "/dev/ttySAC1", O_RDWR)) == -1)
        exit(1);

    if (set_opt(fd, 4800, 8, 'N', 1) == -1)
    {
        exit(1);
    }

    count = 0;
    while (1)
    {
        if (read(fd, &c, 1) > 0)
        {
            if (c != '\n')
            {
                line[count++] = c;
            }
            else
            {
                line[count] = '\0';

                if (parse_line(line, &data))
                {
                    qDebug() << "GPS data:" << line;
                    qDebug() << "longitude:" << data.longitude;
                    qDebug() << "latitude:" << data.latitude;

                    *longitude = data.longitude;
                    *latitude  = data.latitude;
                    return;
                }
                else
                {
                    count = 0;
                }

            }
        }
    }

    close(fd);
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

int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios newtio,oldtio;
    if  ( tcgetattr( fd,&oldtio)  !=  0) {
        perror("SetupSerial 1");
        return -1;
    }
    bzero( &newtio, sizeof( newtio ) );
    newtio.c_cflag  |=  CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;

    switch( nBits )
    {
    case 7:
        newtio.c_cflag |= CS7;
        break;
    case 8:
        newtio.c_cflag |= CS8;
        break;
    }

    switch( nEvent )
    {
    case 'O':
        newtio.c_cflag |= PARENB;
        newtio.c_cflag |= PARODD;
        newtio.c_iflag |= (INPCK | ISTRIP);
        break;
    case 'E':
        newtio.c_iflag |= (INPCK | ISTRIP);
        newtio.c_cflag |= PARENB;
        newtio.c_cflag &= ~PARODD;
        break;
    case 'N':
        newtio.c_cflag &= ~PARENB;
        break;
    }

    switch( nSpeed )
    {
    case 2400:
        cfsetispeed(&newtio, B2400);
        cfsetospeed(&newtio, B2400);
        break;
    case 4800:
        cfsetispeed(&newtio, B4800);
        cfsetospeed(&newtio, B4800);
        break;
    case 9600:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    case 115200:
        cfsetispeed(&newtio, B115200);
        cfsetospeed(&newtio, B115200);
        break;
    case 460800:
        cfsetispeed(&newtio, B460800);
        cfsetospeed(&newtio, B460800);
        break;
    default:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    }
    if( nStop == 1 )
        newtio.c_cflag &=  ~CSTOPB;
    else if ( nStop == 2 )
    newtio.c_cflag |=  CSTOPB;
    newtio.c_cc[VTIME]  = 0;
    newtio.c_cc[VMIN] = 0;
    tcflush(fd,TCIFLUSH);
    if((tcsetattr(fd,TCSANOW,&newtio))!=0)
    {
        perror("com set error");
        return -1;
    }
//	printf("set done!\n\r");
    return 0;
}
