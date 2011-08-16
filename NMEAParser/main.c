//
//  main.c
//  NMEAParser
//
//  Created by Jayant Sai on 8/9/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include "Connection.h"

#define kReadCount 20

int main (int argc, const char * argv[])
{
    bool connection;
    
    connection = connectSerial("/dev/tty.SmartBlueMiniGPSe644-GP");
    
    if (connection) {
        printf("connected\n");
        
        readNMEA(kReadCount);
        
        disconnectSerial();
    }
    else {
        printf("not connected\n");
    }
    
    return 0;
}

