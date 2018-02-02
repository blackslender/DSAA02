/*
 * =========================================================================================
 * Name        : eventLib.cpp
 * Author      : Duc Dung Nguyen
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - Bach Khoa University
 * Description : library for Assignment 2 - Data structures and Algorithms - Fall 2017
 *               This library contains functions used for event management
 * =========================================================================================
 */

#include "requestLib.h"


void loadRequests(char* fName, L1List<VM_Request> &rList) {
	ifstream inputStream(fName);
	if (inputStream) {
		while (!inputStream.eof()) {
			bool isEof = false;
			string tmp;
			inputStream >> tmp;
			if (tmp == "") break;
			while (tmp.length() > 0 && tmp.back() == ';') {
				isEof = true;
				tmp.pop_back();
			}
			VM_Request *tmpR = new VM_Request(tmp);
			rList.insertHead(*tmpR);
		}
	}
	else cout << "The file is not found!";
	rList.reverse();
   
}
