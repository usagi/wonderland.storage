#pragma once

#include <cstdint>
#include <fstream>
#include <vector>

#ifdef EMSCRIPTEN
  #include <emscripten/emscripten.h>
#endif

#include "wonderland.storage.detail/base64_helper.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace storage
    {
      
      template < class T = void >
      auto save( const std::string& path, const std::vector< char >& data )
        -> void
      {
#ifndef EMSCRIPTEN
        // PC: standard filesystem
        std::ofstream f;
        f.exceptions( std::ofstream::failbit | std::ofstream::badbit );
        f.open( path, std::ofstream::binary );
        f.write( data.data(), data.size() );
#else
        const auto base64_data = base64_helper::base64_encode( data );
        // Emscripten: Web Storage
        std::string asm_code;
        asm_code += "try {";
        asm_code += "localStorage.setItem( '";
        asm_code += path;
        asm_code += "', '";
        asm_code += base64_data;
        asm_code += "' );";
        asm_code += "} catch ( e ) { 1; } 0;";
        const auto result = emscripten_run_script_int( asm_code.data() );
        if ( result != 0 )
          throw std::ios_base::failure("fail writing(on the Web Local Storage)");
#endif
        
      }
      
      template < class T = void >
      auto save( const std::string& path, const std::string& data )
        -> void
      { save( path, std::vector< char >( std::begin( data ), std::end( data ) ) ); }
      
      template < class T = void >
      auto save( const std::string& path, const char* data )
        -> void
      {
        std::string buffer( data );
        save( path, buffer );
      }
      
      template < class T = void >
      auto save( const std::string& path, const void* data, std::size_t size )
        -> void
      {
        const auto begin = reinterpret_cast< const char* >( data );
        const auto end   = begin + size;
        save( path, std::vector< char >( begin, end ) );
      }
      
      template < class T >
      auto save( const std::string& path, const T& data )
        -> void
      {
        const auto begin = reinterpret_cast< const char* >( &data );
        const auto end   = begin + sizeof( T );
        save( path, std::vector< char >( begin, end ) );
      }
      
      template < class T = std::string >
      auto load( const std::string& path )
        -> T
      {
        const auto buffer = load< std::string >( path );
        
        // cannot the method, its take an unexptected value on Emscripten-1.16.0.
        //  https://github.com/kripken/emscripten/issues/2346
        //return *reinterpret_cast< const T* >( buffer.data() );
        
        // the alternative method is ok on Emscripten-1.16.0.
        T result;
        std::copy( std::begin( buffer ), std::end( buffer ), reinterpret_cast<char*>(&result) );
        return result;
      }
      
      template < >
      auto load< std::string >( const std::string& path )
        -> std::string
      {
#ifndef EMSCRIPTEN
        // PC: standard filesystem
        std::ifstream f;
        f.exceptions( std::ifstream::failbit | std::ifstream::badbit );
        f.open( path, std::ifstream::binary );
        f.seekg( 0, std::ifstream::end );
        const auto size = f.tellg();
        f.seekg( 0, std::ifstream::beg );
        std::string buffer;
        buffer.resize( size );
        f.read( const_cast< char* >( buffer.data() ), size );
#else
        // Emscripten: Web Storage
        std::string asm_code;
        asm_code += "localStorage.getItem( '";
        asm_code += path;
        asm_code += "' );";
        std::string buffer = emscripten_run_script_string( asm_code.data() );
        if ( buffer == "null" )
          throw std::ios_base::failure("file not found(on the Web Local Storage)");
        buffer = base64_helper::base64_decode( buffer );
#endif
        return buffer;
      }
      
      template < >
      auto load< std::vector< std::uint8_t > > ( const std::string& path )
        -> std::vector< std::uint8_t >
      {
        const auto buffer = load< std::string >( path );
        return { buffer.cbegin(), buffer.cend() };
      }
      
    }
  }
}