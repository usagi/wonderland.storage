wonderland.storage
==================

R/W storage wrapper C++ header-only library for PC-native and Web (Emscripten) designed for wonderland.

- It designed to unifying interface and easy for use.
- It use standard filesystem for PC-native.
- It use Web storage ( localStorage ) for Web (Emscripten).
- It can use standalone.( It is not depend wonderland.)

## Supports

- for PC
    - g++-4.8.1
    - clang++-3.3
- for Web
    - em++-1.16.0 (Emscripten)

## Example

- [example-1](example/example-1/main.cxx)

## Features

in `wonder_rabbit_project::wonderland::storage` namespace free functions:

- save ( `template < class T = std::string > auto save( const std::string& path, const T& data ) -> void` )
    - call `save( "something-1.data", "Hello, string data!" );`
        - PC : write out to the file `something-1.data` with text data.
        - Web: write out to the Web Storage (Local) key `something-1.data` with BASE64 encoded text data.
    - call `save( "something-2.data", 3.14159265358979f );`
        - PC : write raw binary byte sequence data.
        - Web: write text data encoded by BASE64 with raw binary byte sequence data. 

- load ( `template < class T = std::string > auto load( const std::string& path ) -> T` )
    - call `const auto something1 = load( "something-1.data" );`
        - PC : read from the file `something-1.data` with return std::string data.
        - Web: read from the Web Storage (Local) key `something-1.data` with BASE64 decoded std::string data.
    - call `const auto something2 = load< float >( "something-2.data" );`
        - PC : read raw binary byte sequence data and return restored `float` type value
        - Web: read text data encoded by BASE64 with raw binary byte sequence data and return restored `float` type value.

## Dependency

- [Boost.Archives](http://www.boost.org/doc/libs/1_55_0/libs/serialization/doc/archives.html)

## License

- [MIT](LICENSE)

## Author

Usagi Ito <usagi@WonderRabbitProject.net>