#ifndef HQF_LOG_H
#define HQF_LOG_H

#include <iostream>
#include <stdlib.h>
#include <string>

void check(std::string str, int ret); 
void check(std::string str, void* ret, void* errnum);

#endif
