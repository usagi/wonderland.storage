#pragma once

#include <cstdint>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/remove_whitespace.hpp>
#include <boost/archive/iterators/transform_width.hpp>

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace storage
    {
      namespace base64_helper
      {
        template < class T_input_iterator >
        inline auto base64_impl_copy
        ( const T_input_iterator& begin
        , const T_input_iterator& end
        )
          -> std::string
        {
          std::ostringstream result( std::stringstream::out | std::stringstream::binary );
          
          std::copy
          ( begin
          , end
          , std::ostream_iterator< char >( result )
          );
          
          return result.str();
        }
        
        template < class T >
        inline auto base64_encode_impl( const std::vector< T >& input )
          -> std::string
        {
          using base64_encode_iterator
            = boost::archive::iterators::base64_from_binary
              < boost::archive::iterators::transform_width
                < typename std::vector< T >::const_iterator
                , 6
                , 8 * sizeof( T )
                >
              >
            ;
          
          const auto begin = base64_encode_iterator( std::begin( input ) );
          const auto end   = base64_encode_iterator( std::end  ( input ) );
          
          return base64_impl_copy( begin, end );
        }
        
        template < class T = char >
        inline auto base64_encode_impl( const std::basic_string< T >& input )
          -> std::string
        {
          using base64_encode_iterator
            = boost::archive::iterators::base64_from_binary
              < boost::archive::iterators::transform_width
                < typename std::basic_string< T >::const_iterator
                , 6
                , 8 * sizeof( T )
                >
              >
            ;
          
          const auto begin = base64_encode_iterator( std::begin( input ) );
          const auto end   = base64_encode_iterator( std::end  ( input ) );
          
          return base64_impl_copy( begin, end );
        }
        
        template < class T = char >
        inline auto base64_encode_impl( const T* input )
          -> std::string
        {
          return base64_encode_impl( std::string( input ) );
        }
        
        template < class T = char >
        auto base64_encode_impl( std::basic_istream< T >& input )
          -> std::string
        {
          using base64_encode_iterator
            = boost::archive::iterators::base64_from_binary
              < boost::archive::iterators::transform_width
                < std::istream_iterator< T >
                , 6
                , 8 * sizeof( T )
                >
              >
            ;
          
          const auto begin = base64_encode_iterator( std::istream_iterator< T >( input ) );
          const auto end   = base64_encode_iterator( std::istream_iterator< T >() );
          
          return base64_impl_copy( begin, end );
        }
        
        template < class T >
        inline auto base64_encode( T&& value, const bool padding = true )
          -> std::string
        {
          auto result = base64_encode_impl( value );
          
          if ( padding )
            switch( result.size() % 3 )
            {
              case 1: result += "=";
              case 2: result += "=";
            }
          return result;
        }
        
        template < class T_out = std::string >
        auto base64_decode( const std::string& input )
          -> T_out
        {
          using out_value_type = typename T_out::value_type;
          
          const auto result = base64_decode< std::string >( input );
          
          const auto result_begin = reinterpret_cast< const out_value_type* >( result.data() );
          const auto result_end   = result_begin + result.size() / sizeof( out_value_type );
          
          return T_out( result_begin, result_end );
        }
        
        template < >
        auto base64_decode< std::string >( const std::string& input )
          -> std::string
        {
          using base64_decode_iterator
            = boost::archive::iterators::transform_width
              < boost::archive::iterators::binary_from_base64
                < boost::archive::iterators::remove_whitespace
                  < std::string::const_iterator
                  >
                >
              , 8 * sizeof( char )
              , 6
              >
            ;
          
          const auto begin = base64_decode_iterator( std::begin( input ) );
          const auto end   = base64_decode_iterator( std::end  ( input ) );
          
          return base64_impl_copy( begin, end );
        }
        
      }
    }
  }
}
