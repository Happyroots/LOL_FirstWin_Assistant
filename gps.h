#ifndef GPS_H
#define GPS_H

#define PI 3.14159265358979323846
#define EARTH_RADIUS 6371 // 地球半径，单位：公里


class Gps
{
public:
    Gps();
    double calculateDistance(double lat1, double lon1, double lat2, double lon2);
    double toRadians(double degree);

};

#endif // GPS_H
