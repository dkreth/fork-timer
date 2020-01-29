/*
 * Filename       fork-timer.cc
 * Date           1/27/2020
 * Author         Dylan Kreth
 * Email          dylan.kreth@utdallas.edu
 * Course         CS 4348.001 Spring 2020
 * Copyright      2020, All Rights Reserved
 * Procedures:
 * main - program that times how long a fork() takes and writes to a file. optional command line argument
 *        overrides the number of times the test is performed (default = 1000)
 * convertToMicroseconds - takes a struct timeval and returns the number of microseconds as an int
 */

#include <iostream>
#include <fstream>
#include "fork-timer.h"

#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <algorithm>

using namespace std;

/***************************************************************************
* int main( int argc, char **argv )
* Author: Dylan Kreth
* Date: 1/29/2020
* Description: times how long a fork() takes and writes to a file. optional command line argument
*        overrides the number of times the test is performed (default = 1000)
*
* Parameters:
* argc I/P int The number of arguments on the command line
* argv I/P char *[] The arguments on the command line
* main O/P int Status code (not currently used)
**************************************************************************/
int main(int argc, char **argv)
{
  struct timeval startTime;
  struct timeval endTime;
  int fd[2]; //for pipe
  struct timeval duration;
  struct timeval durationFromChild;
  int *stat_loc;
  int numRuns;
  ofstream filestream;
  filestream.open(RESULTS_FILE_NAME);
  filestream << "run number, parent duration, child duration" << endl;
  if (argc > 1)
  {
    numRuns = max((int)strtol(argv[1], NULL, 10), MIN_NUM_RUNS); // updates the number of samples to collect, if needed
  }
  else
  {
    numRuns = MIN_NUM_RUNS; //if no extra command line args, use default number of samples (defaults to 1000)
  }
  cout << "Running fork-timer with " << numRuns << " runs." << endl;

  for (int runCounter = 0; runCounter < numRuns; runCounter++)
  {
    //open a pipe for data transfer from child to parent
    if (pipe(fd) == -1) //pipe() returns -1 on error
    {
      cerr << "pipe() returned -1. " << endl;
      perror("fork");
      exit(EXIT_FAILURE);
    }

    // start timer
    if (gettimeofday(&startTime, NULL)) // gettimeofday returns -1 on error
    {
      cerr << "gettimeofday() returned -1. " << endl;
    }
    if (DEBUG)
      cout << "Start Time: " << startTime.tv_sec << endl;

    pid_t pid = fork(); // store the pid returned by fork so we can differentiate between parent and child

    //get end time
    if (gettimeofday(&endTime, NULL)) // gettimeofday returns -1 on error
    {
      cerr << "gettimeofday() returned -1. " << endl;
    }

    timersub(&endTime, &startTime, &duration); // duration = endTime - startTime

    if (DEBUG)
    {
      char output[50]; // buffer for printing
      sprintf(output, "fork took %ld sec and %ld us", (long)duration.tv_sec, (long)duration.tv_usec);
      cout << output << endl;
    }

    if (pid == 0) //child process
    {
      close(fd[READ]); //we don't need the read end of pipe in child

      write(fd[WRITE], &duration, sizeof(struct timeval)); //send child duration to the parent
      close(fd[READ]); //close pipe when we're done with it
      break; //stop the child. next loop iteration takes place ONLY in parent
    }
    else //parent process
    {
      close(fd[WRITE]); //we don't need the write end of pipe in parent
      waitpid(pid, stat_loc, 0); // wait for child (pid) to finish, and store status info in stat_loc. no special options (0)
      read(fd[READ], &durationFromChild, sizeof(struct timeval)); //receive pipe from the child
      close(fd[READ]); //close pipe when we're done with it

      filestream << runCounter << "," << convertToMicroseconds(duration) << "," << convertToMicroseconds(durationFromChild) << endl;
    }
  }

  filestream.close();

  return 0;
}

/***************************************************************************
* int convertToMicroseconds(struct timeval time)
* Author: Dylan Kreth
* Date: 1/29/2020
* Description: takes a struct timeval and returns the number of microseconds as an int
* Parameters:
* time I/P struct timeval  the struct timeval of the time to be converted
* convertToMicroseconds O/P int number of microseconds
**************************************************************************/
int convertToMicroseconds(struct timeval time)
{
  return (time.tv_sec * 1000.0 * 1000) + (time.tv_usec);
}
