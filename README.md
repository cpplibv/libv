# LIBV

This library is a side project that contains some of the reusable codebase of a closed source project.
This library is not complete yet and developed parallel.
Documentation will follow after some degree of feature-completeness occurs.

## Upcoming features

- New library: libv.ecui - UI backend using libv.gl and libv.ecs.
- New library: libv.vm4 - Model and animation format. Replacement for the legacy codebase of vm3.
- Improvement: libv.gl - Add state caching to avoid redundant calls to the OpenGL state machine.

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