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
#include <stdio.h>

#define DEBUG 0

using namespace std;

int main()
{
  cout << "Hello, World!" << endl;
  struct timeval startTime;
  struct timeval endTime;
  if (gettimeofday(&startTime, NULL))
  {
    cerr << "gettimeofday() returned -1. " << endl;
  }
  cout << "Start Time: " << startTime.tv_sec << endl;
  //call fork
  pid_t pid = fork();

  //get end time
  if (gettimeofday(&endTime, NULL))
  {
    cerr << "gettimeofday() returned -1. " << endl;
  }

  struct timeval duration;
  timersub(&endTime, &startTime, &duration);
  if (DEBUG)
  {
    char output[50];
    sprintf(output, "fork took %ld sec and %ld us", (long)duration.tv_sec, (long)duration.tv_usec);
    cout << output << endl;
  }

  if (pid == 0)
  { //child process
    //TODO open pipe to parent and send
  }
  else
  { //parent process
    //TODO receive pipe from the child
    // TODO open file
    // TODO compare results
    // TODO write results to csv
  }

  return 0;
}
