//  GLAL.h - UniversalUI
//  Created by Seb H on 31/01/2024
//
//  GLAL - Graphics Library Abstraction Layer -
//  is as the name suggests an abstraction layer
//  over various underlying graphics APIs, initially
//  targeting legacy OpenGL 1.1 (for compatibility),
//  Vulkan (preferred on Windows, Linux and Android)
//  and Metal (preferred on Apple/Darwin platforms).

#ifndef GLAL_H
#define GLAL_H

// initialise graphics
int Glal_Init();

// initialise for a specific window
int Glal_InitForWindow(unsigned int windowHandle);


unsigned int Glal_AllocateTexture();


int Glal_SwapBuffers(unsigned int windowHandle);


// free resources
int Glal_Shutdown();



#endif // GLAL_H