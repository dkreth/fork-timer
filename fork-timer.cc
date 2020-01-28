/*
 * Filename       fork-timer.cc
 * Date           1/27/2020
 * Author         Dylan Kreth
 * Email          dylan.kreth@utdallas.edu
 * Course         CS 4348.001 Spring 2020
 * Copyright      2020, All Rights Reserved
 */

#include <iostream>
#include <fstream>
//#include <string>
#include "fork-timer.h"

#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
// #include <sys/types.h>
#include <sys/wait.h>

#define DEBUG 0
#define READ 0
#define WRITE 1
#define NUM_RUNS 100

using namespace std;

int main()
{
  cout << "Hello, World!" << endl;
  struct timeval startTime;
  struct timeval endTime;
  int fd[2];
  struct timeval duration;
  struct timeval durationFromChild;
  int *stat_loc;
  ofstream filestream;
  filestream.open("results.csv");
  filestream << "run number, parent duration, child duration" << endl;

  for (int runCounter = 0; runCounter < NUM_RUNS; runCounter++)
  {
    //open a pipe for data transfer from child to parent
    if (pipe(fd) == -1) //pipe() returns -1 on error
    {
      perror("fork"); //TODO
      exit(EXIT_FAILURE);
    }

    // start timer
    if (gettimeofday(&startTime, NULL))
    {
      cerr << "gettimeofday() returned -1. " << endl;
    }
    if (DEBUG)
      cout << "Start Time: " << startTime.tv_sec << endl;

    pid_t pid = fork();

    //get end time
    if (gettimeofday(&endTime, NULL))
    {
      cerr << "gettimeofday() returned -1. " << endl;
    }

    timersub(&endTime, &startTime, &duration); // duration = endTime - startTime
    if (DEBUG)
    {
      char output[50];
      sprintf(output, "fork took %ld sec and %ld us", (long)duration.tv_sec, (long)duration.tv_usec);
      cout << output << endl;
    }

    if (pid == 0) //child process
    {
      close(fd[READ]); //we don't need the read end of pipe in child

      write(fd[WRITE], &duration, sizeof(struct timeval)); //send child duration to the parent for processing
      return 0;
    }
    else //parent process
    {
      close(fd[WRITE]); //we don't need the write end of pipe in parent
      //TODO receive pipe from the child
      waitpid(pid, stat_loc, 0); // wait for child (pid) to finish, and store status info in stat_loc. no special options (0)
      read(fd[READ], &durationFromChild, sizeof(struct timeval));
      // TODO compare results
      filestream << runCounter << "," << convertToMicroseconds(duration) << "," << convertToMicroseconds(durationFromChild) << endl;
      // TODO write results to csv
    }
  }

  filestream.close();
  return 0;
}

uint64_t convertToMicroseconds(struct timeval time)
{
  uint64_t millis = (time.tv_sec * (uint64_t)1000.0 * 1000) + (time.tv_usec);
}
