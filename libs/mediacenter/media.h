/*
 *
 */

#ifndef MEDIA_H
#define MEDIA_H

#include <string>

#include <odb/core.hxx>

#pragma db object
class Media
{
public:
  Media (const std::string& first,
          const std::string& last,
          unsigned short age)
        : first_(first)
        , last_(last)
        , age_(age)
        {}

  const std::string& first () const;
  const std::string& last () const;

  unsigned short age () const;
  void age (unsigned short);

private:
  Media() {}
  friend class odb::access;

  #pragma db id auto
  unsigned long id_;

  std::string first_;
  std::string last_;
  unsigned short age_;
};

#endif // MEDIA_H
