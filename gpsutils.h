#ifndef GPSUTILS_H
#define GPSUTILS_H

typedef struct GPSData
{
    char	utc_time[11];	/* hhmmss.sss */
    char	utc_day[7];	/* ddmmyy */
    char	is_active;
    double	latitude;
    double	longitude;
    char	latitude_dir;   /* 'N' or 'S' */
    char	longitude_dir;	/* 'E' or 'W' */
    double	ground_speed;	/* speed over ground in knots */
} GPSData;

class GPSUtils
{
public:
    GPSUtils();
    static void get_location(double *longitude, double *latitude);
    static void record_location(double longitude, double latitude);
    static void read_location(double *longitude, double *latitude);

private:
    static bool parse_line(char *line, GPSData *data);
    static int set_opt(int fd, int nSpeed, int nBits, char nEvent, int nStop);
};

#endif // GPSUTILS_H
