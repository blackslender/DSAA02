/*
 * ==========================================================================================
 * Name        : processData.cpp
 * Description : student code for Assignment 2 - Data structures and Algorithms - Fall 2017
 * ==========================================================================================
 */
#include <iostream>
#include <vector>
#include <functional>
#include <math.h>
#include "requestLib.h"
#include "dbLib.h"

using namespace std;

#define GPS_DISTANCE_ERROR 0.005

//Global variables
void PrintTime(time_t &time) {
	char c[40];
	strPrintTime(c, time);
	cout << c;
}
extern L1List<VM_Record> *dbList;
DbTree *dbTree;
DbTree *removed = new DbTree();

char h;
bool ok;
double longitude, latitude, radius;
int res;
int h1, h2;
VM_Record tmpr1, tmpr2, tmpr;
VM_Record *tmppr;
char id[MAX_PARAM_SIZE];
Timehms rTime;

bool initVMGlobalData(void** pGData) {
    // TODO: allocate and initialize global data
    // return false if failed
	dbTree = new DbTree;
	VM_Record *obj = &dbList->Head()->data;

	while (!dbList->isEmpty()){
		Vehicle *vh = dbTree->Find(obj->id);
		if (vh == NULL) { // New vehicle
			vh = new Vehicle(obj->id);
			dbTree->Insert(*vh);
		}
		vh->Insert(*obj);

		dbList->removeHead();
		obj = &dbList->Head()->data;
	};

	//This part is for debugging
	/*
	dbTree->TraverseLNR([](Vehicle &x) {
		cout << x.ID << ": ";
		x.data->TraverseLNR([](VM_Record &x) {
			PrintTime(x.timestamp); cout << endl;
		});
		cout << endl;
	});
	*/
    return true;
}

void releaseVMGlobalData(void* pGData) {
	// TODO: do your cleanup, left this empty if you don't have any dynamically allocated data
	dbTree->TraverseLRN([](Vehicle &x) {
		delete x.data;
	});
	delete dbTree;
	//delete dbList

}

bool processRequest(VM_Request &request, L1List<VM_Record> &recordList, void *pGData) {
	try {
		stringstream ss; ss << request.code;
		string temp;
		switch (request.code[0]) {

				  //Relative position of X to Y
		case '1': { //case 1: 1_X_Y_hhmmss
			char idX[MAX_PARAM_SIZE], idY[MAX_PARAM_SIZE];
			//Exact the request
			if (!getline(ss, temp, '_')) return false; //Request type

			if (!getline(ss, temp, '_')) return false; //IDX
			strcpy(idX, temp.c_str());

			if (!getline(ss, temp, '_')) return false; //IDY
			strcpy(idY, temp.c_str());

			if (!getline(ss, temp, '_')) return false; //hhmmss
			if (temp.length() != 6) return false;

			rTime.hour = (temp[0] - '0') * 10 + temp[1] - '0';
			rTime.min = (temp[2] - '0') * 10 + temp[3] - '0';
			rTime.sec = (temp[4] - '0') * 10 + temp[5] - '0';
			if (!rTime.Valid()) return false;

			if (getline(ss, temp)) return false;

			//Finish checking, work the request

			cout << request.code << ": ";
			Vehicle *v1 = dbTree->Find(idX);
			Vehicle *v2 = dbTree->Find(idY);

			if (v1 == NULL || v2 == NULL) { //Any of two vehicle not found
				cout << "-1\n";
				return true;
			}

			VM_Record* r1 = v1->Find(rTime);
			VM_Record* r2 = v2->Find(rTime);

			if (r1 == NULL || r2 == NULL) { //Any of two vehicle do not have data at that moment
				cout << "-1\n";
				return true;
			}

			//Vertical position
			if (r1->latitude - r2->latitude >= 0) cout << "S "; else cout << "N ";
			//Horizontal position
			if (r1->longitude - r2->longitude >= 0) cout << "E "; else cout << "W ";
			//Distance
			cout << distanceRecord(*r1, *r2) << endl;

			//Request is completed, return
			return true;
			break; }
				  //Count the number of vehicles which only travel to the East/West of a longitude
		case '2': { //Case 2: 2_longitude_H

			//Exact the request
			if (!getline(ss, temp, '_')) return false; //Request type

			if (!getline(ss, temp, '_')) return false; //Longitude
			longitude = stod(temp);

			if (!getline(ss, temp, '_')) return false; //H
			if (temp.length() > 1) return false;
			h = temp[0];
			if (h != 'W' && h != 'E') return false;

			if (getline(ss, temp)) return false;

			//Finish checking, work the request
			cout << request.code << ": ";
			res = 0;

			dbTree->TraverseLNR([](Vehicle &v) {
				ok = true;
				v.TraverseLNR([](VM_Record &r) {
					if (r.longitude >= longitude && h == 'W') { //Found an record locate to the East
						ok = false;
					}
					else if (r.longitude < longitude && h == 'E') { //Found an record locate to the West
						ok = false;
					}
				});
				if (ok) res++;
			});

			cout << res << endl;
			return true;
			break;
		}
				  //Count the number of vehicles which only travel to the North/South of a latitude
		case '3': { //Case 3: 3_latitude_H

			//Exact the request
			if (!getline(ss, temp, '_')) return false; //Request type

			if (!getline(ss, temp, '_')) return false; //Latitude
			latitude = stod(temp);

			if (!getline(ss, temp, '_')) return false; //H
			if (temp.length() > 1) return false;
			h = temp[0];
			if (h != 'S' && h != 'N') return false;

			if (getline(ss, temp)) return false;

			//Finish checking, work the request
			cout << request.code << ": ";
			res = 0;

			dbTree->TraverseLNR([](Vehicle &v) {
				ok = true;
				v.TraverseLNR([](VM_Record &r) {
					if (r.latitude >= latitude && h == 'S') { //Found an record locate to the North
						ok = false;
					}
					else if (r.latitude < latitude && h == 'N') { //Found an record locate to the South
						ok = false;
					}
				});
				if (ok) res++;
			});

			cout << res << endl;
			return true;
			break;
		}
				  //Count the number of vehicles which is in a area around a observe point in a range of time
		case '4': { //4_longitude_latitude_Radius_HourFrom_HourTo

			//Exact the request
			if (!getline(ss, temp, '_')) return false; //Request type

			if (!getline(ss, temp, '_')) return false; //Longitude
			longitude = stod(temp);

			if (!getline(ss, temp, '_')) return false; //Latitude
			latitude = stod(temp);

			if (!getline(ss, temp, '_')) return false; //Radius
			radius = stod(temp);
			if (radius < 0) return false;
			
			if (!getline(ss, temp, '_')) return false; //H1
			h1 = stoi(temp);

			if (!getline(ss, temp, '_')) return false; //H2
			h2 = stoi(temp);
			if (getline(ss, temp)) return false;
			//Finish checking, work the request
			cout << request.code << ": ";
			if (dbTree->IsEmpty()) { cout << "-1\n"; return false; }
			tm tmptm1 = *localtime(&dbTree->Root()->data->Root()->timestamp);
			tmptm1.tm_hour = h1;
			tm tmptm2 = *localtime(&dbTree->Root()->data->Root()->timestamp);
			if (h2 < 24) tmptm2.tm_hour = h2; else { tmptm2.tm_hour = 0; tmptm2.tm_mday++; };
			tmptm1.tm_min = 0;
			tmptm1.tm_sec = 0;
			tmptm2.tm_min = 0;
			tmptm2.tm_sec = 0;
			
			 //Create two dummy record to be compared
			tmpr1.timestamp = mktime(&tmptm1);
			tmpr2.timestamp = mktime(&tmptm2);
			tmpr.longitude = longitude; tmpr.latitude = latitude;
			res = 0;
			dbTree->TraverseLNR([](Vehicle &v) {
				ok = false;
				v.TraverseLNR([](VM_Record &r) {
					if (r.timestamp >= tmpr1.timestamp && r.timestamp <= tmpr2.timestamp && distanceRecord(tmpr, r) < radius) ok = true;
				});
				if (ok) res++;
			});
			cout << res << endl;
			return true;
			break;
		}
				  //Count the number of times the vehicle step in an area
		case '5': { //5_ID_longitude_latitude_radius

			//Exact the request
			if (!getline(ss, temp, '_')) return false; //Request type

			if (!getline(ss, temp, '_')) return false; //ID
			strcpy(id, temp.c_str());

			if (!getline(ss, temp, '_')) return false; //Longitude
			longitude = stod(temp);

			if (!getline(ss, temp, '_')) return false; //Latitude
			latitude = stod(temp);

			if (!getline(ss, temp, '_')) return false; //Radius
			radius = stod(temp);
			if (radius < 0) return false;

			//Finish checking, work the request
			cout << request.code << ": ";
			Vehicle *v = dbTree->Find(id);
			res = 0; tmppr = NULL;
			v->TraverseLNR([](VM_Record &r) {
				if (tmppr) {
					if (distanceEarth(latitude, longitude, tmppr->latitude, tmppr->longitude) >= radius &&
						distanceEarth(latitude, longitude, r.latitude, r.longitude) < radius) { //The vehicle just step in the area
						res++;
					}
				}
				else {
					if (distanceEarth(latitude, longitude, r.latitude, r.longitude) < radius) res++;
				}
				tmppr = &r;
			});
			cout << res << endl;
			return true;
			break;
		}
				 //Print the list of vehicles which is inside the storm at the hhmm moment
		case '8': {//8_longitude_latitude_radius_hhmm

			//Exact the request
			if (!getline(ss, temp, '_')) return false; //Request type

			if (!getline(ss, temp, '_')) return false; //Longitude
			longitude = stod(temp);

			if (!getline(ss, temp, '_')) return false; //Latitude
			latitude = stod(temp);

			if (!getline(ss, temp, '_')) return false; //Radius
			radius = stod(temp);
			if (radius < 0) return false;

			if (!getline(ss, temp, '_')) return false; //hhmmss
			if (temp.length() != 4) return false;

			rTime.hour = (temp[0] - '0') * 10 + temp[1] - '0';
			rTime.min = (temp[2] - '0') * 10 + temp[3] - '0';
			rTime.sec = 0;
			if (!rTime.Valid()) return false;

			if (getline(ss, temp)) return false;

			//Finish checking, work the request
			cout << request.code << ":";
			dbTree->TraverseLNR([](Vehicle &v) {
				ok = true;
				v.TraverseLNR([](VM_Record &r) {
					tm *tmptm = localtime(&r.timestamp);
					if (tmptm->tm_hour == rTime.hour && tmptm->tm_min == rTime.min &&
						distanceEarth(latitude, longitude, r.latitude, r.longitude) < radius) ok = false;
				});
				if (!ok) {
					removed->Insert(v);
					cout << " " << v.ID;
				}
			});

			removed->TraverseLNR([](Vehicle &v) {
				dbTree->Remove(v);
			});

			cout << endl;
			return true;
			break;

		}
				  //Reset the database of vehicles which has been delete to the database
		case '9': {

			//Exact the request
			if (!getline(ss, temp, '_')) return false; //Request type

			if (!getline(ss, temp, '_')) return false; //Longitude
			longitude = stod(temp);

			if (!getline(ss, temp, '_')) return false; //Latitude
			latitude = stod(temp);

			if (!getline(ss, temp, '_')) return false; //Radius
			radius = stod(temp);
			if (radius < 0) return false;

			if (!getline(ss, temp, '_')) return false; //hhmmss
			if (temp.length() != 4) return false;

			rTime.hour = (temp[0] - '0') * 10 + temp[1] - '0';
			rTime.min = (temp[2] - '0') * 10 + temp[3] - '0';
			rTime.sec = 0;
			if (!rTime.Valid()) return false;

			if (getline(ss, temp)) return false;

			//Finish checking, work the request
			cout << request.code << ":";
			removed->TraverseLNR([](Vehicle &v) {
				ok = true;
				v.TraverseLNR([](VM_Record &r) {
					tm *tmptm = localtime(&r.timestamp);
					if (tmptm->tm_hour == rTime.hour && tmptm->tm_min == rTime.min &&
						distanceEarth(latitude, longitude, r.latitude, r.longitude) < radius) ok = false;
				});
				if (!ok) {
					dbTree->Insert(v);
					cout << " " << v.ID;
				}
			});

			dbTree->TraverseLNR([](Vehicle &v) {
				removed->Remove(v);
			});

			cout << endl;
			return true;
			break;
		}
		default: {
			return false;
			break;
		}
		}
		return true;
	}
	catch (char*) {
		return false;
	}
}


