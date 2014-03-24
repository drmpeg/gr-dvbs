/* -*- c++ -*- */
/* 
 * Copyright 2010 Edmund Tse, 2014 Ron Economos.
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

#ifndef INCLUDED_DVBS_REED_SOLOMON_ENC_BB_IMPL_H
#define INCLUDED_DVBS_REED_SOLOMON_ENC_BB_IMPL_H

#include <dvbs/reed_solomon_enc_bb.h>
#include "dvb_consts.h"

extern "C"
{
#include <gnuradio/fec/rs.h>
}

namespace gr {
  namespace dvbs {

    class reed_solomon_enc_bb_impl : public reed_solomon_enc_bb
    {
     private:
      void *rs;    // RS characteristics structure
      void encode(const unsigned char *in, unsigned char *out);

     public:
      reed_solomon_enc_bb_impl();
      ~reed_solomon_enc_bb_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace dvbs
} // namespace gr

#endif /* INCLUDED_DVBS_REED_SOLOMON_ENC_BB_IMPL_H */

