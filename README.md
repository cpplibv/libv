# LIBV

This library contains some reusable code, enjoy.  
This library is not complete yet.  
Documentation is unlikely, but for most cases the code quality should be sufficently high enough to be self documenting.  
Feature development is mostly on-demand, but most library are designed in a buttom-top manner.

## Libraries

 Library      | API          | Feature Set  | Note
:------------ | ------------ | ------------ |:--------------------------------------------------------------
algo          | stable       | lazy eval    |
arg           | stable       | lazy eval    | Light command line argument parser 
color         | -            | -            | Not published yet
concept       | -            | -            | Not published yet
container     | stable       | lazy eval    |
ctrl          | stable       | complete*    | Input event processing and routing library
diff          | stable       | complete     | Patch creator and applier library
ecs           | experimental | lazy eval    | Entity component system
img           | experimental | lazy eval    | Image loader and saver library
input         | stable       | complete     | Light input enum and input event type container library
frame         | stable       | complete*    | A Frame handler library for OpenGL contexts and event handling
fsw           | stable       | complete     | File system watcher
hash          | stable       | lazy eval    | Various hash function library
gl            | experimental | lazy eval    | A C++ OpenGL API wrapper library
glr           | experimental | under dev    | A Remote Deferred OpenGL renderer library
log           | stable       | partial      | Logging library
lua           | stable       | partial      | Lua binding for libv types and utilities
math          | stable       | partial      |
meta          | stable       | lazy eval    | Meta programming utilities and reflection facilities
mt            | experimental | lazy eval    | Various thread utility classes and functions
net           | experimental | under dev    |
parse         | stable       | lazy eval    | Various parsers based on boost::spirit::x3
range         | stable       | lazy eval    | Various range views based on range::v3
rev           | -            | -            | Not published yet
serial        | stable       | lazy eval    |
sig           | deprecated   | complete     | A Signal-Slot paradigm implementation
sig2          | stable       | complete     | Not published yet
state         | experimental | experimental | State machine library / wrapper
ui            | experimental | experimental | GUI library above openGL
utility       | stable       | lazy eval    | Various utility classes and functions
update        | experimental | experimental | Application (self) updater library for both client, server and master side
process       | experimental | lazy eval    | Inter process utilities
vm4           | unstable     | incomplete   | VM4 Model and animation format
vm4imp        | stable       | complete     | VM4 model format importer

## Install

The cmake script is not setup for proper installation of the library yet, only for decelopment builds.  
Building the libraries is possilbe with the usual CMake method.  
To fetch the dependencies build the `ext` target. This is only reqired for the first time or anytime the dependencies change.

## License

LIBV is licensed under the [zlib/libpng license](https://opensource.org/licenses/zlib-license.php).
