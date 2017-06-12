#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h> //exit
#include <pthread.h>

#include "auxFcn.h"
#include "lidarFcn.h"
#include "keyboardbreak.h"

//(UN)ABLE LIDAR 
#define USE_LIDAR_FRONT 1
#define USE_DATA_LOGGING 1

vector<long> lidarFront;

//DATA_LOGGING
int countNewData = 0, logTime = 0, maxLogTime = 0, loop = 1;

struct timeval tStart;
void dataLogging(void *ptr);

int main(int argc, char *argv[])
{
	//Time
	struct timeval t1;
	int loopTime = 0, maxLoopTime = 0, totalTime = 0, previousLoopTime = 0, time1;
	
	//Loop
	int loop1=1;

	//LIDAR
	char *arr0[] = { "pruebalidar", "-s", "/dev/serial/by-path/pci-0000:00:14.0-usb-0:1:1.0" };
	Urg_driver urg0;

	if(USE_DATA_LOGGING)
	{			
			pthread_t thread;
			pthread_create(&thread, NULL, (void *)dataLogging, (void *) NULL);        
	}    				
	if(USE_LIDAR_FRONT)
	{
		lidar_init(3, arr0, urg0);
		lidarFront  =  lidar_lecture(urg0);
		std::cout << "Front Lidar samples: ";
		for (int i = 0; i< 1080; i+=216) cout << lidarFront[i] << "\t|\t";	
	}
		
	gettimeofday(&tStart,NULL);
	gettimeofday(&t1,NULL); 

	//#############################################	
	//#############################################	
	//
	//					LOOP CODE 
	//
	//#############################################	
	//#############################################	
	while(loop == 1) //começo do loop do experimento
	{	
		try
		{
			if(USE_LIDAR_FRONT) lidarFront  =  lidar_lecture(urg0);
			for (int i = 0; i< 1080; i+=216) cout << lidarFront[i] << "\t|\t";	
		}
		catch (...)
		{
			if(USE_LIDAR_FRONT && urg0.max_data_size() < 0) 
					std::cout << "\n\n\n!!!ERROR LIDAR!!!\n\n\n"; 
			loop = 1;        
		}   
		
		if (kbhit() && getchar()=='s') loop=0;
		previousLoopTime = loopTime;
		loopTime = micros(t1);
		gettimeofday(&t1,NULL);
		totalTime = micros(tStart);
		maxLoopTime = (loopTime > maxLoopTime) ? loopTime : maxLoopTime;			
		countNewData++;
		

	}
	//#############################################	
	//#############################################	
	//
	//				LOOP CODE (END)
	//
	//#############################################	
	//#############################################	
	std::cout << "Max loop time: " << maxLoopTime << "us" << endl; 
	if(loop == -1) cout << "\n###################################\n \n \t\tCHECK SENSORS \n\n###################################\n";    
}

//###############################################
//FUNCTIONS
//###############################################
void dataLogging(void *ptr)
{
	int oldCountNewData = 0;
	struct timeval tLoop;
	std::string filename;
	std::fstream logFile;

	filename.append(currentDateTime());
	filename.append("_lidar.txt");
	logFile.open(filename.c_str(),std::fstream::out);
	std::cout << filename << std::endl;
	if (!logFile.is_open())
	{
			std::cout << "Failed to open file " << filename << std::endl;
			exit(1);
	}

	while(loop == 1)
	{   
			gettimeofday(&tLoop,NULL);
			if(oldCountNewData != countNewData)
			{
					
					for(int i =0; i < lidarFront.size(); i++)
					{
							logFile << lidarFront[i] << "|";
					}
					logFile << std::endl;
					std::cout << "DATA_LOGGING: " << millis(tLoop) << "\tmaxDataLogging: " << maxLogTime << endl;
					oldCountNewData = countNewData;
			}                
			logTime = millis(tLoop);
		maxLogTime = (logTime > maxLogTime) ? logTime : maxLogTime;
	}
	std::cout << "\n###################################\n\nmaxLogTime: " << maxLogTime<< "ms\n###################################\n\n";
	logFile.close();	
	pthread_exit(0);
}