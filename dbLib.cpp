/*
 * =========================================================================================
 * Name        : dbLib.cpp
 * Author      : Duc Dung Nguyen
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - Bach Khoa University
 * Description : library for Assignment 2 - Data structures and Algorithms - Fall 2017
 *               This file implements functions used for database management
 * =========================================================================================
 */

#include "dbLib.h"

#include <time.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <cmath>
#define __PI 3.14159265358979323846
#define earthRadiusKm 6371.0

using namespace std;

void    strPrintTime(char* des, time_t& t) {
    tm *pTime = localtime(&t);
    strftime(des, 26, "%Y-%m-%d %H:%M:%S", pTime);
}
L1List<VM_Record> *dbList;
void loadVMDB(char* fName, L1List<VM_Record> &db) {
    ifstream inFile(fName);

    if (inFile) {
        string line;
        getline(inFile , line);// skip the first line
        VM_Record record;

        db.insertHead(record);/// add dummy object

        while (getline(inFile , line)) {
            /// On Windows, lines on file ends with \r\n. So you have to remove \r
            if (line[line.length() - 1] == '\r')
                line.erase(line.length() - 1);
            if (line.length() > 0) {
                if (parseVMRecord((char*)line.data(), db[0]))/// parse and store data directly
                    db.insertHead(record);/// add dummy object for next turn
            }
        }
        db.removeHead();/// remove the first dummy

        db.reverse();
        inFile.close();
    }
    else {
        cout << "The file is not found!";
    }
	dbList = &db;
}

bool parseVMRecord(char *pBuf, VM_Record &bInfo) {
    // 1526,12/05/2016 00:41:04,01,-122.41204,37.79473,0.833,294.0,,0

	stringstream ss; string tmp;
	bool valid = true;
	ss << pBuf;
	
	valid = valid && getline(ss, tmp, ','); //REV

	valid = valid && getline(ss, tmp, ','); //Date & time
	tm time;
	sscanf(tmp.c_str(), "%d/%d/%d %d:%d:%d", &time.tm_mon, &time.tm_mday, &time.tm_year, &time.tm_hour, &time.tm_min, &time.tm_sec);
	time.tm_mon--;
	time.tm_year -= 1900;
	bInfo.timestamp = mktime(&time);

	valid = valid && getline(ss, tmp, ',');//ID
	while (tmp.length() < 4) tmp = "0" + tmp;
	strcpy(bInfo.id, tmp.c_str());

	valid = valid && getline(ss, tmp , ',');//Longitude
	bInfo.longitude = stod(tmp.c_str());

	valid = valid && getline(ss, tmp, ',');//Latitude
	bInfo.latitude = stod(tmp.c_str());

	//Check if the record is valid

	return valid;
}

void process(L1List<VM_Request>& requestList, L1List<VM_Record>& rList) {
    void*   pGData = NULL;
    initVMGlobalData(&pGData);

    while (!requestList.isEmpty()) {
        if(!processRequest(requestList[0], rList, pGData))
            cout << requestList[0].code << " is an invalid event\n";
        requestList.removeHead();
    }

    releaseVMGlobalData(pGData);
}

void printVMRecord(VM_Record &b) {
    printf("%s: (%0.5f, %0.5f), %s\n", b.id, b.longitude, b.latitude, ctime(&b.timestamp));
}

/// This function converts decimal degrees to radians
inline double deg2rad(double deg) {
    return (deg * __PI / 180);
}

///  This function converts radians to decimal degrees
inline double rad2deg(double rad) {
    return (rad * 180 / __PI);
}

/**
 * Returns the distance between two points on the Earth.
 * Direct translation from http://en.wikipedia.org/wiki/Haversine_formula
 * @param lat1d Latitude of the first point in degrees
 * @param lon1d Longitude of the first point in degrees
 * @param lat2d Latitude of the second point in degrees
 * @param lon2d Longitude of the second point in degrees
 * @return The distance between the two points in kilometers
 */
double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d) {
    double lat1r, lon1r, lat2r, lon2r, u, v;
    lat1r = deg2rad(lat1d);
    lon1r = deg2rad(lon1d);
    lat2r = deg2rad(lat2d);
    lon2r = deg2rad(lon2d);
    u = sin((lat2r - lat1r)/2);
    v = sin((lon2r - lon1r)/2);
    return 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}
double distanceRecord(VM_Record &r1, VM_Record &r2) {
	return distanceEarth(r1.latitude, r1.longitude, r2.latitude, r2.longitude);
}