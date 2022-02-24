#ifndef FILES
#define FILES

#include <CoreServices/CoreServices.h>
#include <string>

std::string filePath(std::string relativePath);

std::string applicationSupportPath(std::string relativePath);

#endif