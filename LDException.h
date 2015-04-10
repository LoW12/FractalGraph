#ifndef LDEXEPTION_H
#define LDEXEPTION_H

#include <string>
#include <exception>

class LDException : public std::exception
{
public:
  LDException(std::string str) throw()
  {
      this->m_ErrorMsg = str; 
  }
  virtual const char* what() const throw()
  {
    return this->m_ErrorMsg.c_str();
  } 
  ~LDException() throw()  {}
  
private:
  std::string m_ErrorMsg;
};


 #endif