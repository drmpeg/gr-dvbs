/* -*- c++ -*- */
/* 
 * Copyright 2014 Ron Economos.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */


#ifndef INCLUDED_DVBS_CONFIG_H
#define INCLUDED_DVBS_CONFIG_H

namespace gr {
  namespace dvbs {
    enum dvbs_code_rate_t {
      C1_2 = 0,
      C2_3,
      C3_4,
      C5_6,
      C7_8,
    };
  } // namespace dvbs
} // namespace gr

typedef gr::dvbs::dvbs_code_rate_t dvbs_code_rate_t;

#endif /* INCLUDED_DVBS_CONFIG_H */

