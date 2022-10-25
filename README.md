# Qt-based object reader project 

 ## Powered by
![OpenGL](https://img.shields.io/badge/OpenGL-%23FFFFFF.svg?style=for-the-badge&logo=opengl)![Qt](https://img.shields.io/badge/Qt-%23217346.svg?style=for-the-badge&logo=Qt&logoColor=white)![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)

## Description
Project dedicated to create basic reader for ***wavefront .obj*** files with some additional features.
This is kinda my first project made with use of Qt Framework

## App build 
App could be built by using default 
```
qmake
```
command in command window
App was made with Qt 6.2.4 MinGW 64-bit

## How to use

App has three modes of usage:
- Basic actions with *.obj object
- Fitting
- Registration

## App menu buttons description

### *File*

Allows you
- Load new object to a scene, with which you could to basic actions

### *View*

Allows you
- Change viewport's background color
- Change grid color

### *Scene*

Allows you
- Delete last object from scene
- Clear scene from all objects in it

### *Shaders*

Allows you
- Change app shaders to change how object looks in viewport

### *Object*

Allows you 
- Change last object color
- Find nearest point in that object according to inputted data, which uses **KD-Tree**, point will be shown on object after window is closed

### *Fitting*

For this app *fitting* means using two identical objects:
- **First object** is loaded from user's system
- **Second object** is made from loaded objects

To perform fitting app uses **Gradient descent** optimization method or **KD-Tree** 
- To use **Gradient descent** method click *Perform fitting for object* button
- To use **KD-Tree**  method click *Perform fitting based on closest points* button

### *Registration*

For this app *registration* means using two similar objects to match first object into second one.
To perform *registration* app uses ICP algorythm in order two merge two objects into each other.
Currently (first release) app doesn't support any method to load result of *registration* into file.
 Those two objects should be similar to each other. For example, you can use *sphere-shape object* to registrate it with *baloon-shape* object

To use registration load first object by clicking **Load base mesh** in *Registration menu*, then load second object by clicking **load target mesh** in *Registration menu* and press *Perform rigid registration* button to launch the process of registration.