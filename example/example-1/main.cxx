#include <iostream>
#include <stdexcept>
#include <wonder_rabbit_project/wonderland.storage.hxx>

auto main()
  -> int
try
{
  using wonder_rabbit_project::wonderland::storage::save;
  using wonder_rabbit_project::wonderland::storage::load;
  
  save( "test-1.data", "hogefuga123" );
  save( "test-2.data", 3.14159265358979f);
  
  std::cout << load( "test-1.data" ) << "\n";
  std::cout << load<float>( "test-2.data" ) << "\n";  
}
catch( const std::exception& e )
{
  std::cerr << e.what();
  return 1;
}