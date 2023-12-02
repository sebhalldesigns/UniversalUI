//
// Created by sebam on 11/11/2023.
//

#include "Core/UniversalUI.h"
#include "Native/Application/nApplication.h"


#include <stdio.h>


int UniversalUI(uApplication* application) {
   
    return nApplication::Run(application);
    
}