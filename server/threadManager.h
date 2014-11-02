/*
 * threadManager.h
 *
 *  Created on: 20-Oct-2014
 *      Author: kspviswa
 */

#ifndef THREADMANAGER_H_
#define THREADMANAGER_H_

#include <pthread.h>
#include <iostream>

using namespace std;

class CThread
{
public:
	unsigned long nThreadID;
	pthread_mutex_t _mutex;
};



#endif /* THREADMANAGER_H_ */
