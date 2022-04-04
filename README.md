# LIBV

LIBV is a library collection that contains a lot of reusable code.  

The library collection is not complete and based on its structure and objective it most likely will always keep evolving.
But, many of the contained libraries are design complete and their high quality API can be considered stable.  
Documentation is sparse, but for most cases the strict conventions and code quality should be sufficiently high enough to be self documenting.  
Feature development is mostly on-demand, but most library were designed in a bottom-top manner.  

## Libraries

 Library      | Status       | Description
:------------ |--------------|:--------------------------------------------------------------
algo          | stable       |
arg           | stable       | Light command line argument parser
color         | -            | Early idea
container     | stable       |
ctrl          | stable       | Input event processing and routing library
diff          | stable       | Patch creator and applier library
ecs           | experimental | Entity component system
frame         | stable       | A Frame handler library for OpenGL contexts and event handling
fsw           | stable       | File system watcher
gl            | stable       | A C++ OpenGL API wrapper library
glr           | experimental | A Remote / Deferred OpenGL renderer library
hash          | stable       | Various hash function library
img           | experimental | Image loader and saver library
input         | stable       | Light input enum and input event type container library
log           | stable       | Logging library
lua           | stable       | Lua binding for libv types and utilities
math          | stable       |
meta          | stable       | Meta programming utilities and reflection facilities
mt            | experimental | Various thread utility classes and functions
net           | experimental |
parse         | stable       | Various parsers based on boost::spirit::x3
process       | experimental | Inter-process utilities
range         | stable       | Various range views based on range::v3
rev           | experimental | Higher level render engine above on libv.glr
security      | stable       | Some hashing and RSA related wrapper over OpenSSL
serial        | stable       | Serialization library
sys           | stable       | Poratable wrapper for some OS features
sig           | deprecated   | A Signal-Slot paradigm implementation
sig2          | stable       | Not published yet
state         | experimental | State machine library / wrapper
ui            | experimental | GUI library above openGL
update        | experimental | Application (self) updater library for both client, server and master side
utility       | stable       | Various utility classes and functions
vm4           | unstable     | VM4 model and animation format
vm4imp        | stable       | VM4 model format importer

## Install

The cmake script is not setup for proper installation of the library yet, only for development builds.  
To create a proper packaging I am waiting on the C++ packaging and dependency management ecosystem to evolve,
but even without fully adopting the whole library collection, I made it available and open source, so it might help out some.

Building the libraries is possible with the usual CMake method except 
fetching the dependencies the `ext` target has to be built first (This step is only required for the first time and when the dependencies change).

## License

LIBV is licensed under the [zlib/libpng license](https://opensource.org/licenses/zlib-license.php).
