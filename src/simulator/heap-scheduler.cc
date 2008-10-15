/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
 * Copyright (c) 2005 Mathieu Lacage
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 *
 */

#include "heap-scheduler.h"
#include "event-impl.h"
#include "ns3/assert.h"

#include <string>
#define noTRACE_HEAP 1

#ifdef TRACE_HEAP
#include <iostream>
# define TRACE(x) \
std::cout << "HEAP TRACE " << x << std::endl;
#else /* TRACE_HEAP */
# define TRACE(format,...)
#endif /* TRACE_HEAP */




namespace ns3 {


HeapScheduler::HeapScheduler ()
{
  // we purposedly waste an item at the start of
  // the array to make sure the indexes in the
  // array start at one.
  Scheduler::Event empty = {0,{0,0}};
  m_heap.push_back (empty);
}

HeapScheduler::~HeapScheduler ()
{}

uint32_t 
HeapScheduler::Parent (uint32_t id) const
{
  return id / 2;
}
uint32_t 
HeapScheduler::Sibling (uint32_t id) const
{
  return id + 1;
}
uint32_t 
HeapScheduler::LeftChild (uint32_t id) const
{
  return id * 2;
}
uint32_t 
HeapScheduler::RightChild (uint32_t id) const
{
  return id * 2 + 1;
}

uint32_t
HeapScheduler::Root (void) const
{
  return 1;
}

bool
HeapScheduler::IsRoot (uint32_t id) const
{
  return (id == Root ())?true:false;
}

uint32_t
HeapScheduler::Last (void) const
{
  return m_heap.size () - 1;
}


bool
HeapScheduler::IsBottom (uint32_t id) const
{
  return (id >= m_heap.size ())?true:false;
}

void
HeapScheduler::Exch (uint32_t a, uint32_t b) 
{
  NS_ASSERT (b < m_heap.size () && a < m_heap.size ());
  TRACE ("Exch " << a << ", " << b);
  Event tmp (m_heap[a]);
  m_heap[a] = m_heap[b];
  m_heap[b] = tmp;
}

bool
HeapScheduler::IsLessStrictly (uint32_t a, uint32_t b) const
{
  return m_heap[a] < m_heap[b];
}

uint32_t 
HeapScheduler::Smallest (uint32_t a, uint32_t b) const
{
  return IsLessStrictly (a,b)?a:b;
}

bool
HeapScheduler::IsEmpty (void) const
{
  return (m_heap.size () == 1)?true:false;
}

void
HeapScheduler::BottomUp (void)
{
  uint32_t index = Last ();
  while (!IsRoot (index) && 
         IsLessStrictly (index, Parent (index))) 
    { 
      Exch(index, Parent (index)); 
      index = Parent (index); 
    }
}

void
HeapScheduler::TopDown (uint32_t start)
{
  uint32_t index = start;
  uint32_t right = RightChild (index);
  while (!IsBottom (right)) 
    {
      uint32_t left = LeftChild (index);
      uint32_t tmp = Smallest (left, right);
      if (IsLessStrictly (index, tmp)) 
        {
          return;
        }
      Exch (index, tmp);
      index = tmp;
      right = RightChild (index);
    }
  if (IsBottom (index)) 
    {
      return;
    }
  NS_ASSERT (!IsBottom (index));
  uint32_t left = LeftChild (index);
  if (IsBottom (left)) 
    {
      return;
    }
  if (IsLessStrictly (index, left)) 
    {
      return;
    }
  Exch (index, left);
}


void
HeapScheduler::Insert (const Event &ev)
{
  // acquire single ref
  ev.impl->Ref ();
  m_heap.push_back (ev);
  BottomUp ();
}

EventId
HeapScheduler::PeekNext (void) const
{
  Event next = m_heap[Root ()];
  return EventId (next.impl, next.key.m_ts, next.key.m_uid);
}
EventId
HeapScheduler::RemoveNext (void)
{
  Event next = m_heap[Root ()];
  Exch (Root (), Last ());
  m_heap.pop_back ();
  TopDown (Root ());
  return EventId (Ptr<EventImpl> (next.impl, false), next.key.m_ts, next.key.m_uid);
}


bool
HeapScheduler::Remove (const EventId &id)
{
  uint32_t uid = id.GetUid ();
  for (uint32_t i = 1; i < m_heap.size (); i++)
    {
      if (uid == m_heap[i].key.m_uid)
        {
          NS_ASSERT (m_heap[i].impl == id.PeekEventImpl ());
          // release single ref
          m_heap[i].impl->Unref ();
          Exch (i, Last ());
          m_heap.pop_back ();
          TopDown (i);
          return true;
        }
    }
  NS_ASSERT (false);
  // quiet compiler
  return false;
}

} // namespace ns3

