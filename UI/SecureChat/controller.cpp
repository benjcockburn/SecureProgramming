#include "controller.h"


controller_sp::controller_sp(JsonHandler * jsonInput){
    this->jsonHandler=jsonInput;
};

// handle counter updates and client updates in this from the python
