# LIBV

This library contains some reusable code, enjoy.
This library is not complete yet.
Documentation will follow after some degree of feature-completeness occurs.

## Libraries

 Library      | API          | Feature Set  | Note
:------------ | ------------ | ------------ |:--------------------------------------------------------------
algorithm     | stable       | lazy eval    |
concept       | -            | -            | Not published yet
container     | stable       | lazy eval    |
ecs           | experimental | lazy eval    | Entity component system
frame         | stable       | complete*    | A Frame handler library for OpenGL contexts and event handling
gl            | experimental | partial      | A C++ OpenGL API wrapper library
glr           | experimental | under dev    | A Remote Deferred OpenGL renderer library
log           | stable       | partial      | Logging library
lua           | stable       | partial      | Lua binding for libv types and utilities
math          | stable       | partial      |
meta          | stable       | lazy eval    | Meta programming utilities and reflection facilities
net           | internal use | under dev    |
parse         | experimental | lazy eval    | Parsers based on boost::spirit::x3
range         | stable       | lazy eval    | Various range views based on range::v3
serialization | stable       | lazy eval    |
sig           | deprecated   | complete     | A Signal-Slot paradigm implementation
sig2          | stable       | complete     | Not published yet
thread        | experimental | lazy eval    | Various thread utility classes and functions
ui            | deprecated   | experimental |
ui4           | temporary    | under dev    | GUI library above openGL
utility       | stable       | lazy eval    | Various utility classes and functions
vm4           | unstable     | incomplete   | VM4 Model and animation format
vm4imp        | stable       | complete     | VM4 model format importer

## Upcoming features

- TODO

## Install

NOTE: The cmake script is not setup for proper installation of the library only for build.

Building the libraries:
```
git clone https://github.com/cpplibv/libv.git
mkdir libv/build && cd libv/build
cmake ..
make ext # Only needed during first build
make all
```

## License

LIBV is licensed under the [zlib/libpng license](https://opensource.org/licenses/zlib-license.php).
