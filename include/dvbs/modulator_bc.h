/* -*- c++ -*- */
/* 
 * Copyright 2014 <+YOU OR YOUR COMPANY+>.
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


#ifndef INCLUDED_DVBS_MODULATOR_BC_H
#define INCLUDED_DVBS_MODULATOR_BC_H

#include <dvbs/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace dvbs {

    /*!
     * \brief <+description of block+>
     * \ingroup dvbs
     *
     */
    class DVBS_API modulator_bc : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<modulator_bc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of dvbs::modulator_bc.
       *
       * To avoid accidental use of raw pointers, dvbs::modulator_bc's
       * constructor is in a private implementation
       * class. dvbs::modulator_bc::make is the public interface for
       * creating new instances.
       */
      static sptr make();
    };

  } // namespace dvbs
} // namespace gr

#endif /* INCLUDED_DVBS_MODULATOR_BC_H */

