/*
 * =========================================================================================
 * Name        : dbLib.h
 * Author      : Duc Dung Nguyen
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - Bach Khoa University
 * Description : library for Assignment 2 - Data structures and Algorithms - Fall 2017
 *               This library contains functions used for database management
 * =========================================================================================
 */

#ifndef DSA171A2_DBLIB_H
#define DSA171A2_DBLIB_H

#include <string>
#include <string.h>
#include <time.h>
#include <iostream>
#include <iomanip>
#include <functional>

#include "dsaLib.h"
#include "requestLib.h"

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define ID_MAX_LENGTH 16
#define MAX_DIS 900000000.0

struct VM_Record
{
	char id[ID_MAX_LENGTH];
	time_t timestamp;
	double longitude, latitude;

	// default constructor
	VM_Record()
	{
		id[0] = 0;
	}
	VM_Record(const char *busID)
	{
		strcpy(id, busID);
	}
	// copy constructor
	VM_Record(VM_Record &bus) : timestamp(bus.timestamp), longitude(bus.longitude), latitude(bus.latitude)
	{
		strcpy(id, bus.id);
	}

	bool operator<(VM_Record &obj) { return timestamp < obj.timestamp; }
	bool operator<=(VM_Record &obj) { return timestamp <= obj.timestamp; }
	bool operator>(VM_Record &obj) { return timestamp > obj.timestamp; }
	bool operator>=(VM_Record &obj) { return timestamp >= obj.timestamp; }
	bool operator==(VM_Record &obj) { return timestamp == obj.timestamp; }

	void operator=(VM_Record &obj)
	{
		strcpy(id, obj.id);
		timestamp = obj.timestamp;
		longitude = obj.longitude;
		latitude = obj.latitude;
	}
};
struct Timehms
{
  public:
	int hour = 0, min = 0, sec = 0;

	bool operator>(time_t &obj)
	{
		tm *cvTime = localtime(&obj);
		return (hour * 3600 + min * 60 + sec > cvTime->tm_hour * 3600 + cvTime->tm_min * 60 + cvTime->tm_sec);
	};
	bool operator<(time_t &obj)
	{
		tm *cvTime = localtime(&obj);
		return (hour * 3600 + min * 60 + sec < cvTime->tm_hour * 3600 + cvTime->tm_min * 60 + cvTime->tm_sec);
	}
	bool operator>=(time_t &obj)
	{
		tm *cvTime = localtime(&obj);
		return (hour * 3600 + min * 60 + sec >= cvTime->tm_hour * 3600 + cvTime->tm_min * 60 + cvTime->tm_sec);
	};
	bool operator<=(time_t &obj)
	{
		tm *cvTime = localtime(&obj);
		return (hour * 3600 + min * 60 + sec <= cvTime->tm_hour * 3600 + cvTime->tm_min * 60 + cvTime->tm_sec);
	}
	bool operator==(time_t &obj)
	{
		tm *cvTime = localtime(&obj);
		return (hour * 3600 + min * 60 + sec == cvTime->tm_hour * 3600 + cvTime->tm_min * 60 + cvTime->tm_sec);
	}

	bool Valid()
	{
		if (hour < 0 || hour > 23)
			return false;
		if (min < 0 || min > 59)
			return false;
		if (sec < 0 || sec > 59)
			return false;
		return true;
	}
};

struct Vehicle
{
  public:
	double dis = MAX_DIS;
	char ID[10];
	AVLTree<VM_Record> *data;

	Vehicle()
	{
		strcpy(ID, "");
		data = new AVLTree<VM_Record>();
	}
	Vehicle(char *id)
	{
		strcpy(ID, id);
		data = new AVLTree<VM_Record>();
	}
	Vehicle(const Vehicle &obj)
	{
		strcpy(ID, obj.ID);
		data = obj.data;
		dis = obj.dis;
	}

	//The avltree wont be delete
	~Vehicle()
	{
		//delete data;
	}

	//In assign operator, only the address of the tree is assign
	void operator=(const Vehicle &obj)
	{
		strcpy(ID, obj.ID);
		data = obj.data;
		dis = obj.dis;
	}

	bool operator<(Vehicle &obj) { return strcmp(ID, obj.ID) < 0; }
	bool operator<=(Vehicle &obj) { return strcmp(ID, obj.ID) <= 0; }
	bool operator>(Vehicle &obj) { return strcmp(ID, obj.ID) > 0; }
	bool operator>=(Vehicle &obj) { return strcmp(ID, obj.ID) >= 0; }
	bool operator==(Vehicle &obj) { return strcmp(ID, obj.ID) == 0; }

	

	bool operator<(char *obj) { return strcmp(ID, obj) < 0; }
	bool operator<=(char *obj) { return strcmp(ID, obj) <= 0; }
	bool operator>(char *obj) { return strcmp(ID, obj) > 0; }
	bool operator>=(char *obj) { return strcmp(ID, obj) >= 0; }
	bool operator==(char *obj) { return strcmp(ID, obj) == 0; }

	//Insert a VM_Record to the vehicle's database
	void Insert(VM_Record &obj)
	{
		data->Insert(obj);
	}

	//Look for the record which has exactly timestamp
	VM_Record *Find(Timehms &obj)
	{
		if (data->IsEmpty())
			return NULL;
		tm *tmptm = localtime(&data->Root()->timestamp);
		tmptm->tm_hour = obj.hour;
		tmptm->tm_min = obj.min;
		tmptm->tm_sec = obj.sec;
		VM_Record tmpRec;
		tmpRec.timestamp = mktime(tmptm);
		VM_Record *res = data->Find(tmpRec);
		return res;
	}

	void TraverseLNR(void (*op)(VM_Record &))
	{
		data->TraverseLNR(op);
	}
	void TraverseLRN(void (*op)(VM_Record &))
	{
		data->TraverseLRN(op);
	}
	void TraverseNLR(void (*op)(VM_Record &))
	{
		data->TraverseNLR(op);
	}

	void TraverseLNR(void (*op)(VM_Record &, void *), void *param)
	{
		data->TraverseLNR(op, param);
	}
	void TraverseLRN(void (*op)(VM_Record &, void *), void *param)
	{
		data->TraverseLRN(op, param);
	}
	void TraverseNLR(void (*op)(VM_Record &, void *), void *param)
	{
		data->TraverseNLR(op, param);
	}
};

class DbTree : public AVLTree<Vehicle>
{
  public:
	Vehicle *Find(char *obj)
	{
		Vehicle tmp;
		strcpy(tmp.ID, obj);
		AVLTree<Vehicle> *base = this;
		Vehicle *res = base->Find(tmp);
		return res;
	}
};

void printVMRecord(VM_Record &);
void strPrintTime(char *des, time_t &t);
bool parseVMRecord(char *, VM_Record &);
void loadVMDB(char *, L1List<VM_Record> &);
double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d);
double distanceRecord(VM_Record &r1, VM_Record &r2);

bool processRequest(VM_Request &, L1List<VM_Record> &, void *); // from processData.cpp

/// NOTE: student may create this function to allocate some global data
bool initVMGlobalData(void **pGData);
/// NOTE: student must defined this function if they use dynamically allocated global data.
/// If student do not use any dynamic global data, please define this function as empty function
/// in your code (file processData.cpp) as follows
/// void releaseBusGlobalData() {}
void releaseVMGlobalData(void *pGData);

void process(L1List<VM_Request> &requestList, L1List<VM_Record> &recordList);

#endif //DSA171A2_DBLIB_H
