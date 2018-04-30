//Code adapted from GEODATASOURCE website.
/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::                                                                         :*/
/*::           GeoDataSource.com (C) All Rights Reserved 2017                :*/
/*::                                                                         :*/
/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

#include <math.h>
#include "gps_calc.h"

/*​ ​@brief​ calculate angle clockwise from north between two decimal location points in degrees
​ ​*
​ ​*​ ​@param lat1, lon1 lat and long of source point
 * @param lat2, lon2 lat and long of target point
​ ​*
​ ​*/
double angle(double lat1, double lon1, double lat2, double lon2)
{

    double dLon = deg2rad(lon2 - lon1);

    double y = sin(dLon) * cos(deg2rad(lat2));
    double x = cos(deg2rad(lat1)) * sin(deg2rad(lat2)) - sin(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(dLon);

    double brng = atan2(y, x);

    brng = rad2deg(brng);
    brng = fmodf((brng + 360), 360); //make it positive
    //brng = 360 - brng; // count degrees counter-clockwise - remove to make clockwise

    return brng;
}


/*​ ​@brief​ calculate distance between two decimal location points in a chosen distance formula
​ ​*
​ ​*​ ​@param lat1, lon1, alt1 lat and long and altitude of source point
 * @param lat2, lon2, alt2 lat and long and altitude of target point
 * @param unit choose miles or kilometers for distance unit
 * @param factor_in_alt choose whether to include altitude in calculation. Usually makes only a small difference and adds calculation time
​ ​*
​ ​*/
double distance(double lat1, double lon1, double alt1, double lat2, double lon2, double alt2, char unit, char factor_in_alt)
{
  double theta, dist;
  theta = lon1 - lon2;
  dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta));
  dist = acos(dist);
  dist = rad2deg(dist);
  dist = dist * 60 * 1.853159616;

  //factors in altitude in addition to lon/lat. This is really only significant for sudden altitude changes (like climbing the Flatirons)
  if(factor_in_alt) dist = sqrt(pow(dist*1000,2)+pow((alt2-alt1),2))/1000;

  switch(unit) {
    case MILES:
      dist = dist * 0.71550892786;
      break;
    case KILOM:
      break;
  }
  return (dist);
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  This function converts decimal degrees to radians             :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
double deg2rad(double deg) {
  return (deg * pi / 180);
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  This function converts radians to decimal degrees             :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
double rad2deg(double rad) {
  return (rad * 180 / pi);
}
