#pragma once

#include <cstdint>
#include <vector>

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace storage
    {
      namespace binary_helper
      {
        template < class T >
        auto to_vector_uint8( const T& input )
          -> std::vector< std::uint8_t >
        {
          const auto begin = reinterpret_cast< const std::uint8_t* >( &input );
          const auto end   = begin + sizeof( T );
          return std::vector< std::uint8_t >( begin, end );
        }
      }
    }
  }
}