//
//  Connection.h
//  NMEAParser
//
//  Created by Jayant Sai on 8/9/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef _Connection_h
#define _Connection_h

#include <stdbool.h>
#include <time.h>


/* connection functions */
bool connectSerial(const char *filename);
void disconnectSerial(void);

/* nmea functions */
void readNMEA(unsigned count);

#endif
