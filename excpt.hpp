#include <exception>
#include <stdexcept>
#include <string>
using namespace std;
class Failure: public exception{
private:
  string EM;
public:
  Failure(string em) : EM(em){}
  const char * what() const noexcept{
    return EM.c_str();
  }
};
