# LIBV

This library is a side project that contains some of the reusable codebase of a closed source project.
This library is not complete yet and developed parallel.
Documentation will follow after some degree of feature-completeness occurs.

## Incoming features

- ECS - Entity Component System (framework)
- ECUI - An UI framework using OpenGL and ECS

## Install

NOTE: The cmake script is not setup for installation of the library

Build the project:
```
git clone https://github.com/cpplibv/libv.git
mkdir libv/build && cd libv/build
cmake ..
make ext # Only needed during first build
make all
```

## License

LIBV is licensed under the [zlib/libpng license](https://opensource.org/licenses/zlib-license.php).