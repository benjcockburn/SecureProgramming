#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "JsonHandler.h"

class controller_sp
{
public:
    controller_sp(JsonHandler * jsonInput);

    JsonHandler * jsonHandler;
};

#endif // CONTROLLER_H
