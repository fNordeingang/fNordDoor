#include <cstdio>
#include <csignal>
#include <unistd.h>
#include <ctime>
#include <mutex>
#include <thread>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <exception>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>

#include "irc.h"

#ifdef armv6l
#include "wiringPi/wiringPi/wiringPi.h"
#else
#define OUTPUT 0
#define INPUT  1
int wiringPiSetup();
void pinMode(int,int);
void digitalWrite(int,int);
int digitalRead(int);
#endif


#define BTNOPEN     7
#define BTNCLOSE   15
#define BTNDOOR    11

#define LEDRED      8
#define LEDBLUE     9

#define RED         1
#define BLUE        2
#define OFF         3


void init(int noInitLock);
void initPins();
void initLock();
void setButtonMode(int mode);
void signalHandler(int sig);
void setLED(int mode);
void pressButton(int btn);

int isOpenPressed();
int isClosePressed();
int isDoorClosed();

void openLock();
void closeLock();


static irc_session_t* door_session;

namespace timer {

typedef std::chrono::high_resolution_clock clock;
typedef std::chrono::microseconds microseconds;
typedef std::chrono::milliseconds milliseconds;
typedef std::chrono::seconds seconds;

clock::time_point now(){return clock::now();}

microseconds intervalUs(const clock::time_point& t1, const clock::time_point& t0)
{
  return std::chrono::duration_cast<microseconds>(t1 - t0);
}

milliseconds intervalMs(const clock::time_point& t1,const clock::time_point& t0)
{
  return std::chrono::duration_cast<milliseconds>(t1 - t0);
}

seconds intervalS(const clock::time_point& t1,const clock::time_point& t0)
{
  return std::chrono::duration_cast<seconds>(t1 - t0);
}

class StopWatch
{
  clock::time_point start_;
  public:
  StopWatch() : start_(clock::now()){}
  clock::time_point restart() { start_ = clock::now(); return start_;}
  microseconds elapsedUs()    { return intervalUs(now(), start_);}
  milliseconds elapsedMs()    { return intervalMs(now(), start_);}
  seconds elapsedS()    { return intervalS(now(), start_);}
};

}

std::string get_http(const std::string &server,const std::string &path)
{
    using namespace std;
    using boost::asio::ip::tcp;
    stringstream result;
    boost::asio::io_service io_service;

    // Get a list of endpoints corresponding to the server name.
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(server, "http");
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    // Try each endpoint until we successfully establish a connection.
    tcp::socket socket(io_service);
    boost::asio::connect(socket, endpoint_iterator);

    // Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream << "GET " << path << " HTTP/1.0\r\n";
    request_stream << "Host: " << server << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

    // Send the request.
    boost::asio::write(socket, request);

    // Read the response status line. The response streambuf will automatically
    // grow to accommodate the entire line. The growth may be limited by passing
    // a maximum size to the streambuf constructor.
    boost::asio::streambuf response;
    boost::asio::read_until(socket, response, "\r\n");

    // Check that response is OK.
    std::istream response_stream(&response);
    std::string http_version;
    response_stream >> http_version;
    unsigned int status_code;
    response_stream >> status_code;
    std::string status_message;
    std::getline(response_stream, status_message);
    if (!response_stream || http_version.substr(0, 5) != "HTTP/")
    {
      throw runtime_error("Invalid response");
    }
    if (status_code != 200)
    {
      stringstream ss;
      ss<< "Response returned with status code " << status_code << "\n";
      throw runtime_error(ss.str());
    }
        // Read the response headers, which are terminated by a blank line.
    boost::asio::read_until(socket, response, "\r\n\r\n");

    // Process the response headers.
    std::string header;
    while (std::getline(response_stream, header) && header != "\r");
    //  std::cout << header << "\n";
    //std::cout << "\n";

    // Write whatever content we already have to output.
    if (response.size() > 0)
        result << &response;

    // Read until EOF, writing data to output as we go.
    boost::system::error_code error;
    while (boost::asio::read(socket, response,
          boost::asio::transfer_at_least(1), error))
      result << &response;
    if (error != boost::asio::error::eof)
      throw boost::system::system_error(error);
    return result.str();
}


std::string get_wan_ip() {
  return get_http("ipwhats.appspot.com","/"); 
}


using namespace timer;
namespace po = boost::program_options;

int main (int ac, char** av)
{

  po::options_description desc("Allowed options");
  desc.add_options()
    ("help", "produce help message")
    ("no-lock-init", "don't call lock init")
    ;

  po::variables_map vm;
  po::store(po::parse_command_line(ac, av, desc), vm);
  po::notify(vm);    

  if (vm.count("help")) {
    std::cout << desc << "\n";
    return 1;
  }


  printf ("fNordDoor started!\n");

  if (wiringPiSetup () == -1)
    return 1;

  std::string wan_ip = get_wan_ip();

  door_session = init_irc_session();
  std::thread t(connect_irc, door_session);

  init(vm.count("no-lock-init"));

  StopWatch measure;

  for (;;)
  {
    if((door_session != 0 && irc_is_connected(door_session) == 0) || measure.elapsedS().count() > 285) {
      printf("timeout/disconnect\n");
      if(door_session !=  0) {
        printf("checking wan ip\n");
        if(irc_is_connected(door_session) == 0 || wan_ip != get_wan_ip()) {
          printf("reconnecting\n");
          wan_ip = get_wan_ip();
          irc_disconnect(door_session);
          t.join();
          irc_destroy_session(door_session);
          door_session = init_irc_session();
          t = std::thread(connect_irc, door_session);
        }
      }
      measure.restart();
    }
    else {
      printf("connected\n");
    }
    /*if (isDoorClosed())
    {
      if(isOpenPressed()) {
        setLED(BLUE);
        printf("opening door\n");
      } else if(isClosePressed()) {
        setLED(RED);
        printf("closing door\n");
      }
    }
    #ifdef NOTPI
    usleep(1000000);
    #else
    usleep(10000);
    #endif*/
    usleep(1000000);
  }
  return 0 ;
}

void init(int noInitLock) {
  initPins();
  if(!noInitLock)
    initLock();

  signal(SIGUSR1,
        signalHandler);
  signal(SIGUSR2, signalHandler);
}

void initPins() {
  printf("initializing pins...");
  fflush(stdout);
  pinMode(LEDRED, OUTPUT);
  pinMode(LEDBLUE, OUTPUT);

  setLED(OFF);

  setButtonMode(OUTPUT);

  digitalWrite(BTNOPEN, 1);
  digitalWrite(BTNCLOSE, 1);
  printf("done\n");
}

void initLock() {
  printf("initializing doorlock...");
  fflush(stdout);
  digitalWrite(BTNOPEN, 0);
  usleep(500000);
  digitalWrite(BTNOPEN, 1);
  usleep(500000);
  digitalWrite(BTNOPEN, 0);
  usleep(4500000);
  digitalWrite(BTNOPEN, 1);
  usleep(10000000);
  pressButton(BTNCLOSE);
  printf("done\n");
}

void signalHandler(int sig) {
  if(sig == SIGUSR1) {
    openLock();
    setLED(BLUE);
  } else if(sig == SIGUSR2) {
    closeLock();
    setLED(RED);
  }
}

void setButtonMode(int mode) {
  pinMode(BTNOPEN, mode);
  pinMode(BTNCLOSE, mode);
}

void setLED(int mode) {
  if (mode == OFF) {
    digitalWrite(LEDRED, 1);
    digitalWrite(LEDBLUE, 1);
  } else if(mode == BLUE) {
    digitalWrite(LEDRED, 1);
    digitalWrite(LEDBLUE, 0);
  } else if(mode == RED) {
    digitalWrite(LEDRED, 0);
    digitalWrite(LEDBLUE, 1);
  }
}

void openLock() {
  printf("open door command\n");
  pressButton(BTNOPEN);
  irc_cmd_msg (door_session, "#fnorddoor", "someone has openened the door.");
}

void closeLock() {
  printf("close door command\n");
  pressButton(BTNCLOSE);
  irc_cmd_msg (door_session, "#fnorddoor", "someone has closed the door.");
}

int isOpenPressed() {
  return digitalRead(BTNOPEN);
}

int isClosePressed() {
  return digitalRead(BTNCLOSE);
}

int isDoorClosed() {
  return !digitalRead(BTNDOOR);
}

void pressButton(int btn) {
  digitalWrite(btn, 0);
  usleep(500000);
  digitalWrite(btn, 1);
}


#ifndef armv6l

int wiringPiSetup() { return 0; }
void pinMode(int p, int m) { printf("pinMode\n"); }
void digitalWrite(int p, int v) { printf("digitalWrite\n"); }
int digitalRead(int p) { printf("digitalRead\n"); return 1; }

#endif
