///////////////////////////////////////////////////////////////
//  Copyright Beman Dawes 1994-99.
//  Copyright 2020 John Maddock.
//  Copyright 2022 Christopher Kormanyos.
//  Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_MP_TIMER_HPP
#define BOOST_MP_TIMER_HPP

#include <algorithm>
#include <ctime>
#include <limits>

// This file now reflects the 2022 work-over. We have replaced the
// original timer facility (which had been based on std::clock())
// with C++11's equivalent <chrono> counterparts.

template <class ClockType = std::chrono::high_resolution_clock>
struct stopwatch;

template <class ClockType>
struct stopwatch
{
public:
   using clock_type = ClockType;

   using duration_type = typename clock_type::duration;

   constexpr stopwatch() : m_start(clock_type::now()) { }

   constexpr stopwatch(const stopwatch& other) : m_start(other.m_start) { }
   constexpr stopwatch(stopwatch&& other) noexcept : m_start(other.m_start) { }

   auto operator=(const stopwatch& other) -> stopwatch&
   {
      if(this != &other)
      {
         m_start = other.m_start;
      }

      return *this;
   }

   auto operator=(stopwatch&& other) noexcept -> stopwatch&
   {
      m_start = other.m_start;

      return *this;
   }

   ~stopwatch() { }

   constexpr auto elapsed() const -> duration_type
   {
      return (clock_type::now() - m_start);
   }

   auto elapsed_max() const -> duration_type
   {
      constexpr auto latest_possible_time_point = (std::chrono::time_point<clock_type>::max)();

      return latest_possible_time_point - m_start;
   }

   static constexpr auto elapsed_min() -> duration_type
   {
      return (std::chrono::time_point<clock_type>::min)() -  std::chrono::time_point<clock_type>();
   }

   void reset()
   {
      m_start = clock_type::now();
   }

private:
   typename clock_type::time_point m_start;
};

class timer : public stopwatch<>
{
private:
   using base_class_type = stopwatch<>;

public:
   constexpr timer() { }

   void restart() { base_class_type::reset(); }

   constexpr auto elapsed() const -> double
   {
      // Return the elapsed time in seconds.
      return std::chrono::duration_cast<std::chrono::duration<double>>(base_class_type::elapsed()).count();
   }

   constexpr auto elapsed_max() const -> double
   {
      return std::chrono::duration_cast<std::chrono::duration<double>>(base_class_type::elapsed_max()).count();
   }

   static constexpr auto elapsed_min() -> double
   {
      return std::chrono::duration_cast<std::chrono::duration<double>>(base_class_type::elapsed_min()).count();
   }
};

#endif // BOOST_MP_TIMER_HPP
