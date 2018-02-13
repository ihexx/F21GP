# Games programming Lab01:

# Getting started with C++/OpenGL

## Lab 1: Objectives

1. Remember how to setup a project in Visual Studio (linking, compiling and debugging).
2. Set up helper libraries for OpenGL (if you did not do our 3D Graphics & Animation
    course).
3. Understand a template OpenGL project and how to add your algorithms to it for
    visualisation.
4. Write C++ routines to handle vector/matrix operations.

## Warning note here

If you are using Google to search for examples, please be careful you only search for the newer
OpenGL code. If you see functions like _glRotate_ , _glBegin_ , _glEnd_ then you are most likely reading
old code that is not used anymore. Same applies to C++, please use the new functionality only.
(* So, that solution from StackOverflow that you just found might not be the best solution as it
can be using really old code.)

## Lab 1.1: Visual Studio and a simple text game

In this course we are going to be using Visual Studio 2015, C++ and limited amounts of OpenGL.

We also need to use some open-source libraries to help us load our context (GLFW), maths
(GLM), images (GLI), and extensions (GLEW).

Visual Studio 2015 and 2017 is already installed in the MACS lab machines. We are going to use
2015 as the libraries work better with this version. (e.g. auto-complete).

If you want to download it for your own laptop just go to https://www.visualstudio.com/ and
select the community edition (free). Do not forget to select to install the C++ components and
Windows SDKs.


Task for everyone: Try to create a small game with no graphics, just text. Do not forget to allocate
memory and use pointers if needed. Instruction below should get you started.

To create a new project click on:
_File >> New >> Project and then
Templates >> Visual C++ >> Empty Project_

Visual Studio just created an empty project. To start let’s by creating a simple C++ source file.

**Note: there was a catch up lecture on C++ and it should be available in Vision** ... for now, if
you missed that lecture, just create a simple source file in the _Source Files_ directory with this
code:

#include <iostream>
using namespace std;
int main()
{
cout << "I'm alive!\n";
cout << "\nPress any key to continue...\n";
cin.ignore(); cin.get(); // delay closing console to read debugging errors.
return 0;
}


Right click on _Source Files_ folder to add C++ file...


Compile and run your code in debug mode (►) :

If everything was fine, you should see this window:

Debug using the usual methods and strategies:

If something went wrong, then you can check the Error List. If you can’t see this window then
press CTRL+\ and e.

Notes:

```
 Visual Studio C++ Intellisense is access with CTRL+Space
 Reset Visual Studio Layout using ALT+w and r
```

## Lab 1.2: A game with graphics (Window Context)

We first need to create a window and add an OpenGL context (aka somewhere to render) to it.

To start, download the template from GitHub link below and unzip the files in your machine.
https://github.com/StfnoPad/GPGameTemplate

We are going to use GLFW for our programs. This is an Open Source,
multi-platform library for creating windows, contexts and handling
inputs and events. This library was added to the GitHub Template. You can also download the
files here [http://www.glfw.org/download.html](http://www.glfw.org/download.html) ( **32 - bit Windows Binaries** ). I will show you the
basic setup in the pages but please read the documentation if you want to know more about the
library or if you are stuck.

In addition, we are going to be using the OpenGL Extension Wrangler Library (GLEW)
to provide us with the newest functions for our rendering context. We need GLEW
on Windows as it only provides functionality for OpenGL 1.x. This library was also
added to the GitHub file. You can download the library also from [http://glew.sourceforge.net](http://glew.sourceforge.net) (32-
bit and 64-bit Windows Binaries).

Finally, to do some of the graphics rotations, translations and projections, we
use the OpenGL Mathematics Library from G-Truc (https://glm.g-truc.net). This
library is also included in the GitHub files and only requires to be header linked.


## Lab 1.2: Correctly linking GLFW & GLEW & GLM

Now, we are going to link these libraries to our project:

1) Right click on the Project name (see below) and open properties.


2) Make sure you select All Configurations and Active(Win32).

3) Update the links to the libraries (Use the appropriate path depending on where you saved
your files).

**VC++ Directories >> Include Directories:**

```
 D:\GPGameTemplate-master\glfw-3.2.1.bin.WIN32\include
 D:\GPGameTemplate-master\glew-2.1.0\include
 D:\GPGameTemplate-master\glm-0.9.9-a2\
```
**VC++ Directories >> Reference Directories:**

```
 D:\GPGameTemplate-master\glfw-3.2.1.bin.WIN32\lib-vc
 D:\GPGameTemplate-master\glew-2.1.0\bin\Release\Win
```
**VC++ Directories >> Library Directories:**

```
 D:\GPGameTemplate-master\glfw-3.2.1.bin.WIN32\lib-vc
 D:\GPGameTemplate-master\glew-2.1.0\lib\Release\Win
```
**C/C++ >> Additional Include Directories:**

```
 D:\GPGameTemplate-master\glfw-3.2.1.bin.WIN32\include
```

```
 D:\GPGameTemplate-master\glew-2.1.0\include
```
**Linker >> Additional Library Directories:**

```
 D:\GPGameTemplate-master\glfw-3.2.1.bin.WIN32\lib-vc
 D:\GPGameTemplate-master\glew-2.1.0\lib\Release\Win
```
**Linker >> Input >> Additional Dependencies:**

opengl32.lib
glew32.lib
glew32s.lib
glfw3.lib
glfw3dll.lib

## Lab 1.2: A simple template game program

The purpose of the skeleton template program is to bring up a window and run the main
rendering loop.

Note that it:

```
 includes GLFW and GLEW
 starts GLFW, checks for errors and inserts an error handler
 uses a hints function to specify OpenGL context and debug
 creates a window and attaches the context
 produces a rendering loop
 Uses a skeleton render function to define a viewport and clear the screen
```
## Lab 1.2: The rendering loop

Most operating systems work using events loops (https://en.wikipedia.org/wiki/Event_loop),
basically a loop handles all the events for a window. These events include window (resizing,
exiting), mouse and keyboard events. In template program includes a loop to poll events,
update, and render our graphics -- until the user decides to exit the application.

Try running the program. If it crashed, see below:


GLEW uses dynamic loaded DLLs instead of static loaded DLLs (ones that we can define in the
project properties) - as so we need to add them these DLLs to your program.

The easiest way to do it is just to copy glew32.dll (glew-2.0.0\bin\Release\Win32) to your project
folder (ProjectName/ProjectName). If you see your source.cpp file then you are in the right
folder.

Everything should be running perfectly now.


The template programme code follows the essential structure of most games (setup, a loop, and
exit):

Try experimenting and understanding the different functions and how they affect the template
program.

**Advance materials:** Game engines also follow a standard order of execution, here is the one for
Unity https://docs.unity3d.com/Manual/ExecutionOrder.html


## Lab 1.3: A few more exercises

Task 1: try to add code to rotate the cube when pressing a key or moving your mouse.

Hint: the rotation and displacement is calculated in these lines:

glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -6.0f));

mv_matrix = glm::rotate(mv_matrix, 10.0f, glm::vec3(-1.0f, -1.0f, 0.0f));

Task 2: can you replace the cube with a figure consisting of more elements?

Task 3: although we could use the GLM library for vector and matrix ops, you are asked to write
your own mini-library in C++ for the basic operations (see lecture slides for ops needed).

This way your maths can be independent of your graphics


