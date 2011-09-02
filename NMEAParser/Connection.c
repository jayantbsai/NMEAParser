/*
 The MIT License (MIT)
 Copyright (c) 2011 Jayant Sai (@j6y6nt)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>

#include "Connection.h"

#define kDataSize 512
#define kReadSize 255
#define kSentenceSize 255

static int fd;

/* util functions */

static int strchari(const char *haystack, char needle, int beg) {
    int i;
    size_t il;
    
    i = (int) beg;
    il = strlen(haystack);
    for (; i<il; i++) {
        if (haystack[i] == needle) {
            return i;
        }
    }
    
    return -1;
}

/* public functions */

bool connectSerial(const char *filename) {
    unsigned count = 1;
    
    fd = open(filename, O_RDONLY | O_NOCTTY | O_NDELAY);
    
    while (fd == -1) {
        fd = open(filename, O_RDONLY | O_NOCTTY | O_NDELAY);
        printf("count = %u, fd = %i\n", count, fd);
        
        if (fd != -1 || count >= 3) {
            break;
        }
        
        close(fd);
        sleep(1);
        count++;
    }
    
    printf("fd = %i\n", fd);
    
    if (fd != -1) {
        return true;
    }
    
    return false;
}

void disconnectSerial(void) {
    close(fd);
}

void readNMEA(unsigned count) {
    unsigned i = 0;
    char data[kDataSize], indata[kReadSize], sentence[kSentenceSize];
    char dollar, eol;
    int beg, end;
    size_t charsize = sizeof(char);
    
    dollar = '$';
    eol = '\r';
    
    fcntl(fd, F_SETFL, 0);
    
    while (i < count) { // while (true)
        read(fd, indata, kReadSize);
        printf("\n<io>\n%s\n</io>\n", indata);
        
        beg = 0;
        memset(data, 0, kDataSize);
        strcat(data, sentence);
        strcat(data, indata);
        memset(sentence, 0, kSentenceSize);
        
        while (true) {
            beg = strchari(data, dollar, beg);
            end = strchari(data, eol, beg+1);
            
            if (beg == -1) {
                break;
            }
            else if (end == -1) {
                end = (int) strlen(data);
                memset(sentence, 0, kSentenceSize);
                memcpy(sentence, data + (beg * charsize), end - beg);
                break;
            }
            else {
                memcpy(sentence, data + (beg * charsize), end - beg);
                printf("<sentence>%s</sentence>\n", sentence);
                memset(sentence, 0, kSentenceSize);
            }
            
            beg++;
        }
        
        i++;
    }
}
