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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "puncture_bb_impl.h"

namespace gr {
  namespace dvbs {

    puncture_bb::sptr
    puncture_bb::make(dvbs_code_rate_t rate)
    {
      return gnuradio::get_initial_sptr
        (new puncture_bb_impl(rate));
    }

    /*
     * The private constructor
     */
    puncture_bb_impl::puncture_bb_impl(dvbs_code_rate_t rate)
      : gr::block("puncture_bb",
              gr::io_signature::make(1, 1, sizeof(unsigned char)),
              gr::io_signature::make(1, 1, sizeof(unsigned char))),
      index(0)
      {
          int j;
          switch (rate)
          {
              case gr::dvbs::C1_2:
                  nbits_keep = 2;
                  nbits_total = 2;
                  P[0][0] = 1;
                  P[1][0] = 1;
                  break;
              case gr::dvbs::C2_3:
                  nbits_keep = 3;
                  nbits_total = 4;
                  P[0][0] = 1; P[2][0] = 0;
                  P[1][0] = 1; P[3][0] = 1;
                  break;
              case gr::dvbs::C3_4:
                  nbits_keep = 4;
                  nbits_total = 6;
                  P[0][0] = 1; P[2][0] = 0; P[4][0] = 1;
                  P[1][0] = 1; P[3][0] = 1; P[5][0] = 0;
                  break;
              case gr::dvbs::C5_6:
                  nbits_keep = 6;
                  nbits_total = 10;
                  P[0][0] = 1; P[2][0] = 0; P[4][0] = 1; P[6][0] = 0; P[8][0] = 1;
                  P[1][0] = 1; P[3][0] = 1; P[5][0] = 0; P[7][0] = 1; P[9][0] = 0;
                  break;
              case gr::dvbs::C7_8:
                  nbits_keep = 8;
                  nbits_total = 14;
                  P[0][0] = 1; P[2][0] = 0; P[4][0] = 0; P[6][0] = 0; P[8][0] = 1; P[10][0] = 0; P[12][0] = 1;
                  P[1][0] = 1; P[3][0] = 1; P[5][0] = 1; P[7][0] = 1; P[9][0] = 0; P[11][0] = 1; P[13][0] = 0;
                  break;
          }
          j = 1;
          for (int i = 0; i < 14; i++)
          {
              P[i][1] = j;
              j = (j + 1) % nbits_total;
          }
      }

    /*
     * Our virtual destructor.
     */
    puncture_bb_impl::~puncture_bb_impl()
    {
    }

    void
    puncture_bb_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        // e.g. for 3/4 code, we need 4 bits input for every 3 bits punctured output
        std::fill(ninput_items_required.begin(), ninput_items_required.end(),
                        noutput_items * nbits_total / nbits_keep);
    }

    int
    puncture_bb_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const unsigned char *in = (const unsigned char *) input_items[0];
        unsigned char *out = (unsigned char *) output_items[0];

        int ni = 0;
        int no = 0;
        unsigned int j;

        // Terminate when we run out of either input data or output capacity
        j = index;
        while (ni < ninput_items[0] && no < noutput_items)
        {
            if (P[j][0] != 0)    // Not punctured, so copy across
            {
                out[no] = in[ni];
                ++no;
            }
            ++ni;
            j = P[j][1];
        }
        index = j;

        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each (ni);

        // Tell runtime system how many output items we produced.
        return no;
    }

  } /* namespace dvbs */
} /* namespace gr */

