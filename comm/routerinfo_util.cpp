//
// Created by jackson on 17-3-17.
//

#include "routerinfo_util.h"

bool RouterInfoUtil::Equals(const RouterInfo& left, const RouterInfo& right)
{
  bool b_ip = left.alloc_ip().compare(right.alloc_ip()) == 0;
  bool b_port = left.alloc_port() == right.alloc_port();
  bool b_sect_id = left.sect_id() ==  right.sect_id();
  bool b_inure = left.inure() ==  right.inure();
  bool b_n = left.n() ==  right.n();

  bool equals = b_ip && b_port && b_sect_id && b_inure && b_n;
  return equals;
}

bool RouterInfoUtil::Later(const RouterInfo& left, const RouterInfo& right)
{
  bool later;
  if (left.inure() > right.inure()) {
	later = true;
  } else if (left.inure() < right.inure()) {
	later = false;
  } else {
	later = left.n() > right.n() ? true : false;
  }

  return later;
}


bool RouterInfoUtil::SameIpPort(const RouterInfo& left, const RouterInfo& right)
{
  bool b_ip = left.alloc_ip().compare(right.alloc_ip()) == 0;
  bool b_port = left.alloc_port() == right.alloc_port();

  bool equals = b_ip && b_port;
  return equals;
}

bool RouterInfoUtil::SameSectId(const RouterInfo& left, const RouterInfo& right)
{
  bool b_sect_id = left.sect_id() ==  right.sect_id();

  bool equals = b_sect_id;
  return equals;
}
