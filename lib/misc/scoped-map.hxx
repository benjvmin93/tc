/** \file misc/scoped-map.hxx
 ** \brief Implementation of misc::scoped_map.
 */

#pragma once

#include <sstream>
#include <stdexcept>
#include <type_traits>

#include <misc/algorithm.hh>
#include <misc/contract.hh>
#include <misc/indent.hh>
#include <range/v3/view/reverse.hpp>

namespace misc
{
  template <typename Key, typename Data>
  scoped_map<Key, Data>::scoped_map()
    : nb_scope(0),
    maps_({})
    {}

  template <typename Key, typename Data>
  void scoped_map<Key, Data>::put(const Key& key, const Data& value)
  {
      maps_[nb_scope - 1].insert({key, value});
  }

  template <typename Key, typename Data>
  Data scoped_map<Key, Data>::get(const Key& key) const
  {

    for (auto e = maps_.begin(); e != maps_.end(); e++)
    {
      if ((*e).find(key) != (*e).end())
      {
        return (*e).at(key);
      }
    }

    throw std::range_error("No value matches the given key.");
  }

  template <typename Key, typename Data>
  std::ostream& scoped_map<Key, Data>::dump(std::ostream& ostr) const
  {
    for (auto i = maps_.begin(); i != maps_.end(); i++)
    {
      ostr << "{ " << (*i).first << ", " << (*i).second << " }";
      if (i + 1 != maps_.end())
      {
        ostr << '\n';
      }
    }
  }

  template <typename Key, typename Data>
  void scoped_map<Key, Data>::scope_begin()
  {
    maps_.push_back({});
    nb_scope++;
  }

  template <typename Key, typename Data>
  void scoped_map<Key, Data>::scope_end()
  {
    maps_.pop_back();
    nb_scope--;
  }

  template <typename Key, typename Data>
  inline std::ostream& operator<<(std::ostream& ostr,
                                  const scoped_map<Key, Data>& tbl)
  {
    return tbl.dump(ostr);
  }

} // namespace misc
