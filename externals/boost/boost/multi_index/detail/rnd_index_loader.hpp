/* Copyright 2003-2006 Joaqu?n M L?pez Mu?oz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/multi_index for library home page.
 */

#ifndef BOOST_MULTI_INDEX_DETAIL_RND_INDEX_LOADER_HPP
#define BOOST_MULTI_INDEX_DETAIL_RND_INDEX_LOADER_HPP

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include <boost/config.hpp> /* keep it first to prevent nasty warns in MSVC */
#include <algorithm>
#include <boost/multi_index/detail/auto_space.hpp>
#include <boost/multi_index/detail/rnd_index_ptr_array.hpp>
#include <boost/noncopyable.hpp>
#include <cstddef>

namespace boost{

namespace multi_index{

namespace detail{

/* This class implements a serialization rearranger for random access
 * indices. In order to achieve O(n) performance, the following strategy
 * is followed: the nodes of the index are handled as if in a bidirectional
 * list, where the next pointers are stored in the original
 * random_access_index_ptr_array and the prev pointers are stored in
 * an auxiliary array. Rearranging of nodes in such a bidirectional list
 * is constant time. Once all the arrangements are performed (on destruction
 * time) the list is traversed in reverse order and
 * pointers are swapped and set accordingly so that they recover its
 * original semantics ( *(node->up())==node ) while retaining the
 * new order.
 */

template<typename Allocator>
class random_access_index_loader_base:private noncopyable
{
protected:
  typedef random_access_index_node_impl            node_type;
  typedef random_access_index_ptr_array<Allocator> ptr_array_type;

  random_access_index_loader_base(const Allocator& al_,ptr_array_type& ptrs_):
    al(al_),
    ptrs(ptrs_),
    header(*ptrs.end()),
    prev_spc(al,0),
    preprocessed(false)
  {}

  ~random_access_index_loader_base()
  {
    if(preprocessed)
    {
      node_type* n=header;
      next(n)=n;

      for(std::size_t i=ptrs.size();i--;){
        n=prev(n);
        std::size_t d=position(n);
        if(d!=i){
          node_type* m=prev(next_at(i));
          std::swap(m->up(),n->up());
          next_at(d)=next_at(i);
          std::swap(prev_at(d),prev_at(i));
        }
        next(n)=n;
      }
    }
  }

  void rearrange(node_type* position,node_type *x)
  {
    preprocess(); /* only incur this penalty if rearrange() is ever called */
    if(!position)position=header;
    next(prev(x))=next(x);
    prev(next(x))=prev(x);
    prev(x)=position;
    next(x)=next(position);
    next(prev(x))=prev(next(x))=x;
  }

private:
  void preprocess()
  {
    if(!preprocessed){
      /* get space for the auxiliary prev array */
      auto_space<node_type*,Allocator> tmp(al,ptrs.size()+1);
      prev_spc.swap(tmp);

      /* prev_spc elements point to the prev nodes */
      std::rotate_copy(ptrs.begin(),ptrs.end(),ptrs.end()+1,prev_spc.data());

      /* ptrs elements point to the next nodes */
      std::rotate(ptrs.begin(),ptrs.begin()+1,ptrs.end()+1);

      preprocessed=true;
    }
  }

  std::size_t position(node_type* x)const
  {
    return (std::size_t)(x->up()-ptrs.begin());
  }

  node_type*& next_at(std::size_t n)const
  {
    return *ptrs.at(n);
  }

  node_type*& prev_at(std::size_t n)const
  {
    return prev_spc.data()[n];
  }

  node_type*& next(node_type* x)const
  {
    return *(x->up());
  }

  node_type*& prev(node_type* x)const
  {
    return prev_at(position(x));
  }

  Allocator                        al;
  ptr_array_type&                  ptrs;
  node_type*                       header;
  auto_space<node_type*,Allocator> prev_spc;
  bool                             preprocessed;
};

template<typename Node,typename Allocator>
class random_access_index_loader:
  private random_access_index_loader_base<Allocator>
{
  typedef random_access_index_loader_base<Allocator> super;
  typedef typename super::ptr_array_type             ptr_array_type;

public:
  random_access_index_loader(const Allocator& al_,ptr_array_type& ptrs_):
    super(al_,ptrs_)
  {}

  void rearrange(Node* position,Node *x)
  {
    super::rearrange(position?position->impl():0,x->impl());
  }
};

} /* namespace multi_index::detail */

} /* namespace multi_index */

} /* namespace boost */

#endif
