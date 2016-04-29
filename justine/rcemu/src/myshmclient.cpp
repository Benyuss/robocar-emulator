/**
 * @brief Justine - this is a rapid prototype for development of Robocar City Emulator
 *
 * @file myshmclient.cpp
 * @author  Norbert Bátfai <nbatfai@gmail.com>
 * @version 0.0.10
 *
 * @section LICENSE
 *
 * Copyright (C) 2014 Norbert Bátfai, batfai.norbert@inf.unideb.hu
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 * Robocar City Emulator and Robocar World Championship
 *
 * desc
 *
 */
#include <iostream>
#include <ctime>
#include <ratio>
#include <myshmclient.hpp>
#include <chrono>
#include <clocale>
#include <cstdio>
//#include <trafficlexer.hpp>

char data[524288];

std::vector<justine::sampleclient::MyShmClient::Gangster> justine::sampleclient::MyShmClient::gangsters ( boost::asio::ip::tcp::socket & socket, int id,
    osmium::unsigned_object_id_type cop )
{

  boost::system::error_code err;

  size_t length = std::sprintf ( data, "<gangsters " );
  length += std::sprintf ( data+length, "%d>", id );

  socket.send ( boost::asio::buffer ( data, length ) );

  length = socket.read_some ( boost::asio::buffer ( data ), err );

  if ( err == boost::asio::error::eof )
    {

      // TODO

    }
  else if ( err )
    {

      throw boost::system::system_error ( err );
    }

  /* reading all gangsters into a vector */
  int idd {0};
  unsigned f, t, s;
  int n {0};
  int nn {0};
  std::vector<Gangster> gangsters;

  while ( std::sscanf ( data+nn, "<OK %d %u %u %u>%n", &idd, &f, &t, &s, &n ) == 4 )
    {
      nn += n;
      gangsters.push_back ( Gangster {idd, f, t, s} );
    }

  std::sort ( gangsters.begin(), gangsters.end(), [this, cop] ( Gangster x, Gangster y )
  {
    return dst ( cop, x.to ) < dst ( cop, y.to );
  } );

  std::cout.write ( data, length );
  std::cout << "Command GANGSTER sent." << std::endl;

  return gangsters;
}

std::vector<justine::sampleclient::MyShmClient::Cop> justine::sampleclient::MyShmClient::initcops ( boost::asio::ip::tcp::socket & socket )
{

  boost::system::error_code err;

  size_t length = std::sprintf ( data, "<init guided %s 10 c>", m_teamname.c_str() );

  socket.send ( boost::asio::buffer ( data, length ) );

  length = socket.read_some ( boost::asio::buffer ( data ), err );


  if ( err == boost::asio::error::eof )
    {

      // TODO

    }
  else if ( err )
    {

      throw boost::system::system_error ( err );
    }

  /* reading all gangsters into a vector */
  int idd {0};
  int f, t;
  char c;
  int n {0};
  int nn {0};
  std::vector<Cop> cops;

  while ( std::sscanf ( data+nn, "<OK %d %d/%d %c>%n", &idd, &f, &t, &c, &n ) == 4 )
    {
      nn += n;
      cops.push_back ( idd );
    }

  std::cout.write ( data, length );
  std::cout << "Command INIT sent." << std::endl;

  return cops;
}


int justine::sampleclient::MyShmClient::init ( boost::asio::ip::tcp::socket & socket )
{

  boost::system::error_code err;

  size_t length = std::sprintf ( data, "<init guided %s 1 c>", m_teamname.c_str() );

  socket.send ( boost::asio::buffer ( data, length ) );

  length = socket.read_some ( boost::asio::buffer ( data ), err );

  if ( err == boost::asio::error::eof )
    {

      // TODO

    }
  else if ( err )
    {

      throw boost::system::system_error ( err );

    }

  int id {0};
  std::sscanf ( data, "<OK %d", &id );

  std::cout.write ( data, length );
  std::cout << "Command INIT sent." << std::endl;

  return id;

}

void justine::sampleclient::MyShmClient::pos ( boost::asio::ip::tcp::socket & socket, int id )
{

  boost::system::error_code err;

  size_t length = std::sprintf ( data, "<pos " );
  length += std::sprintf ( data+length, "%d %u %u>", id, 2969934868u, 651365957u );

  socket.send ( boost::asio::buffer ( data, length ) );

  length = socket.read_some ( boost::asio::buffer ( data ), err );

  if ( err == boost::asio::error::eof )
    {

      // TODO

    }
  else if ( err )
    {

      throw boost::system::system_error ( err );

    }

  std::cout.write ( data, length );
  std::cout << "Command POS sent." << std::endl;
}

void justine::sampleclient::MyShmClient::car ( boost::asio::ip::tcp::socket & socket, int id, unsigned *f, unsigned *t, unsigned* s )
{

  boost::system::error_code err;

  size_t length = std::sprintf ( data, "<car " );
  length += std::sprintf ( data+length, "%d>", id );

  socket.send ( boost::asio::buffer ( data, length ) );

  length = socket.read_some ( boost::asio::buffer ( data ), err );

  if ( err == boost::asio::error::eof )
    {

      // TODO

    }
  else if ( err )
    {

      throw boost::system::system_error ( err );
    }

  int idd {0};
  std::sscanf ( data, "<OK %d %u %u %u", &idd, f, t, s );

  std::cout.write ( data, length );
  std::cout << "Command CAR sent." << std::endl;

}

void justine::sampleclient::MyShmClient::route (
  boost::asio::ip::tcp::socket & socket,
  int id,
  std::vector<osmium::unsigned_object_id_type> & path
)
{

  boost::system::error_code err;

  size_t length = std::sprintf ( data,
                                 "<route %d %d", path.size(), id );

  for ( auto ui: path )
    length += std::sprintf ( data+length, " %u", ui );

  length += std::sprintf ( data+length, ">" );

  socket.send ( boost::asio::buffer ( data, length ) );

  length = socket.read_some ( boost::asio::buffer ( data ), err );

  if ( err == boost::asio::error::eof )
    {

      // TODO

    }
  else if ( err )
    {

      throw boost::system::system_error ( err );

    }

  std::cout.write ( data, length );
  std::cout << "Command ROUTE sent." << std::endl;

}

void justine::sampleclient::MyShmClient::start ( boost::asio::io_service& io_service, const char * port )
{

#ifdef DEBUG
  foo();
#endif

  boost::asio::ip::tcp::resolver resolver ( io_service );
  boost::asio::ip::tcp::resolver::query query ( boost::asio::ip::tcp::v4(), "localhost", port );
  boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve ( query );

  boost::asio::ip::tcp::socket socket ( io_service );
  boost::asio::connect ( socket, iterator );

  int id = init ( socket );

  pos ( socket, id );

  unsigned int g {0u};
  unsigned int f {0u};
  unsigned int t {0u};
  unsigned int s {0u};

  std::vector<Gangster> gngstrs;

  for ( ;; )
    {
      std::this_thread::sleep_for ( std::chrono::milliseconds ( 200 ) );

      car ( socket, id, &f, &t, &s );

      gngstrs = gangsters ( socket, id, t );

      if ( gngstrs.size() > 0 )
        g = gngstrs[0].to;
      else
        g = 0;
      if ( g > 0 )
        {

          std::vector<osmium::unsigned_object_id_type> path = hasDijkstraPath ( t, g );

          if ( path.size() > 1 )
            {

              std::copy ( path.begin(), path.end(),
                          std::ostream_iterator<osmium::unsigned_object_id_type> ( std::cout, " -> " ) );

              route ( socket, id, path );
            }
        }
    }
}

void justine::sampleclient::MyShmClient::start10 ( boost::asio::io_service& io_service, const char * port )

{

#ifdef DEBUG
  foo();
#endif

  boost::asio::ip::tcp::resolver resolver ( io_service );
  boost::asio::ip::tcp::resolver::query query ( boost::asio::ip::tcp::v4(), "localhost", port );
  boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve ( query );

  boost::asio::ip::tcp::socket socket ( io_service );
  boost::asio::connect ( socket, iterator );

  std::vector<Cop> cops = initcops ( socket );

  unsigned int g {0u};
  unsigned int f {0u};
  unsigned int t {0u};
  unsigned int s {0u};

  bool elso = false;
  bool masodik = false;

  std::fstream fs;
  fs.open ("check.txt", std::fstream::out);
  fs << 1 << std::endl;
  
  std::vector<Gangster> gngstrs;
  //nodes nevű vektorban van tárolva a 10 csomópont
     std::vector<unsigned int> nodes={651615380,651615380,651615380,651615380,651615380,651615380,651615380,651615380,651615380,651615380};
    std::vector<unsigned int> nodess={651613292,243202123,207427447,207427447,651612875,1348431279,3247363059,2526444608,320766589,651613292};
  
  auto a = std::chrono::high_resolution_clock::now();//a változóban letároltam az indításkori aktuális időt, ami auto típusú
  for ( ;; )
    {
      std::this_thread::sleep_for ( std::chrono::milliseconds ( 200 ) );

      for ( auto cop:cops )//for-each,aminél valami olyasmi történik a háttérben:for(int i=0;i<cop.size;i++),csak nincs i ciklusváltozó
        {
          car ( socket, cop, &f, &t, &s ); //mindig a következőre mutat cops...

          gngstrs = gangsters ( socket, cop, t );

          if ( gngstrs.size() > 0 ){
            g = gngstrs[0].to; //amennyi van
            fs << 1 <<std::endl;}
          else
            g = 0; //lenullázza

          if ( g == 0 )
            {
              fs << 0 << std::endl;
             auto b= std::chrono::high_resolution_clock::now();//itt folyamatosan tárolom az aktuális időt
        if(std::chrono::duration_cast<std::chrono::milliseconds>(b-a).count()>1000 && std::chrono::duration_cast < std::chrono::milliseconds>(b-a).count() < 60000000)//ha az aktuális idő és az indításkori idő különbsége nagyobb mint 2 perc(120000millisec)
    {
     if(masodik == false)
      {for(std::vector<int>::size_type i = 0; i != cops.size(); i++)//ezt a Bagossy féle 2.meetup egyik programjában találtam
          {
          car (socket, cops[i], &f, &t, &s);//átadom a szokásos paramétereket a car-nak,plussz cops[]vektor i-edik  elemét   
          std::vector<osmium::unsigned_object_id_type> path = hasDijkstraPath ( t, nodess[i] );//útvonalat készít

          if ( path.size() > 1 )
            {std::copy ( path.begin(), path.end(),
            std::ostream_iterator<osmium::unsigned_object_id_type> ( std::cout, " -> " ) );
            route ( socket, cops[i], path );// felküldjük a szerverre az i-edik coppal
            }
          }
      masodik = true;         
      }
    }
       auto c= std::chrono::high_resolution_clock::now();//itt folyamatosan tárolom az aktuális időt      
        if(std::chrono::duration_cast<std::chrono::milliseconds>(c-a).count()>180000)
    {if(elso == false)
       {system("/home/rcvedes/robocar-emulator/rcrc");
       elso = true;
       }
     if (g == 0) 
      {for(std::vector<int>::size_type i = 0; i != cops.size(); i++)//ezt a Bagossy féle 2.meetup egyik programjában találtam
          {
          car (socket, cops[i], &f, &t, &s);//átadom a szokásos paramétereket a car-nak,plussz cops[]vektor i-edik  elemét   
          std::vector<osmium::unsigned_object_id_type> path = hasDijkstraPath ( t, nodes[i] );//útvonalat készít

          if ( path.size() > 1 )
      {std::copy ( path.begin(), path.end(),
                         std::ostream_iterator<osmium::unsigned_object_id_type> ( std::cout, " -> " ) );
       route ( socket, cops[i], path );// felküldjük a szerverre az i-edik coppal
      }
          }
      }
    }
            }
            else
        {std::vector<osmium::unsigned_object_id_type> path = hasDijkstraPath ( t, g );
        if ( path.size() > 1 )
          { std::copy ( path.begin(), path.end(),
                  std::ostream_iterator<osmium::unsigned_object_id_type> ( std::cout, " -> " ) );
                  route ( socket, cop, path ); 
    }
        }  
        }
    }fs.close();

}
