/*
 * Filename       fork-timer.cc
 * Date           1/27/2020
 * Author         Dylan Kreth
 * Email          dylan.kreth@utdallas.edu
 * Course         CS 4348.001 Spring 2020
 * Copyright      2020, All Rights Reserved
 */


#include <iostream>
//#include <fstream>
//#include <string>
#include "fork-timer.h"

#include <sys/time.h>
#include <unistd.h>

using namespace std;

int main() {
  cout << "Hello, World!" << endl;
  struct timeval startTime;
  struct timeval endTime;
  if(!gettimeofday(&startTime, NULL)){
    cerr << "gettimeofday() returned -1. "
  }
  cout << "Start Time: " << startTime.tv_sec << endl;
  //call fork


  //handle parent

  //handle child

  //get end time

  //write results to a csv


}
