# LIBV

LIBV is a library collection that contains a lot of reusable code.  

Feature development is mostly on-demand, but most library were designed in a bottom-top manner.  
Documentation is sparse, but for most cases the strict conventions and code quality should be sufficiently high enough to be self documenting.  

## Libraries

Most of the libraries in the collection are design complete and can be considered stable, but the collection
prioritizes research and velocity over API stability.  

| Library   | Status / API | Description                                                                |
|:----------|--------------|:---------------------------------------------------------------------------|
| algo      | Stable       |                                                                            |
| arg       | Stable       | Light command line argument parser                                         |
| color     | Idea         | Early idea                                                                 |
| container | Stable       |                                                                            |
| ctrl      | Stable       | Input event processing and routing library                                 |
| diff      | Stable       | Patch creator and applier library                                          |
| ecs       | Experimental | Entity component system                                                    |
| frame     | Stable       | A Frame handler library for OpenGL contexts and event handling             |
| fsw       | Stable       | File system watcher                                                        |
| gl        | Stable       | A C++ OpenGL API wrapper library                                           |
| glr       | Experimental | A Remote / Deferred OpenGL renderer library                                |
| hash      | Stable       | Various hash function library                                              |
| img       | Experimental | Image loader and saver library                                             |
| input     | Stable       | Light input enum and input event type container library                    |
| log       | Stable       | Logging library                                                            |
| lua       | Stable       | Lua binding for libv types and utilities                                   |
| math      | Stable       |                                                                            |
| meta      | Stable       | Meta programming utilities and reflection facilities                       |
| mt        | Experimental | Various thread utility classes and functions                               |
| net       | Experimental |                                                                            |
| parse     | Stable       | Various parsers based on boost::spirit::x3                                 |
| process   | Experimental | Inter-process utilities                                                    |
| range     | Stable       | Various range views based on range::v3                                     |
| rev       | Experimental | Higher level render engine above on libv.glr                               |
| security  | Stable       | Some hashing and RSA related wrapper over OpenSSL                          |
| serial    | Stable       | Serialization library. Light-weight wrapper over Vide                      |
| sys       | Stable       | Portable wrapper for some OS features                                      |
| sig       | Deprecated   | A Signal-Slot paradigm implementation                                      |
| sig2      | Stable       | Not published yet                                                          |
| state     | Experimental | State machine library / wrapper                                            |
| ui        | Experimental | GUI library above openGL                                                   |
| update    | Experimental | Application (self) updater library for both client, server and master side |
| utility   | Stable       | Various utility classes and functions                                      |
| vm4       | Unstable     | VM4 model and animation format                                             |
| vm4imp    | Stable       | VM4 model format importer                                                  |

## Install

The cmake script is not setup for proper installation of the library yet, only for development builds.  
To create a proper packaging I am waiting on the C++ packaging and dependency management ecosystem to evolve,
but even without fully adopting the whole library collection, I made it available and open source, so it might help out some.

Building the libraries is possible with the usual CMake method except 
during the first build the `ext` target has to be built to fetch the dependencies 
(This step is only required for the first time and when the dependencies change).

## License

LIBV is licensed under the [zlib/libpng license](https://opensource.org/licenses/zlib-license.php).
