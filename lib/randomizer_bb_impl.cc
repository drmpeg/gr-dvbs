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
#include "randomizer_bb_impl.h"

namespace gr {
  namespace dvbs {

    randomizer_bb::sptr
    randomizer_bb::make()
    {
      return gnuradio::get_initial_sptr
        (new randomizer_bb_impl());
    }

    /*
     * The private constructor
     */
    randomizer_bb_impl::randomizer_bb_impl()
      : gr::sync_block("randomizer_bb",
              gr::io_signature::make(1, 1, sizeof(unsigned char)),
              gr::io_signature::make(1, 1, sizeof(unsigned char)))
    {
        init_rand();
        counter = 0;
        set_output_multiple(188);
    }

    /*
     * Our virtual destructor.
     */
    randomizer_bb_impl::~randomizer_bb_impl()
    {
    }

    void randomizer_bb_impl::init_rand() {
        // Generate and store the PRBS
        unsigned int state = PRELOAD_VALUE;
        unsigned char bit, byte;
        for (int i = 1; i < DVB_RANDOMIZER_PERIOD; ++i)
        {
            assert(state < 1 << 15);
            byte = 0;
            // Calculate the next byte output
            for (int j = 0; j < 8; ++j)
            {
                // Append the next LFSR output to LSB of output
                byte <<= 1;
                bit = (state & 0x0002) >> 1 ^ (state & 0x0001);
                byte |= bit;
                // Shift the LFSR bits by 1 and feed output bit back to the start
                state >>= 1;
                state |= bit << 14;
            }
            sequence[i] = byte;
        }
        // First SYNC is inverted
        sequence[0] = 0xFF;
        // Disable PRBS for subsequent SYNCs to leave untouched
        for (int i = MPEG_TS_PKT_LENGTH; i < DVB_RANDOMIZER_PERIOD; i += MPEG_TS_PKT_LENGTH)
        {
            sequence[i] = 0x00;
        }
    }

    int
    randomizer_bb_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const unsigned char *in = (const unsigned char *) input_items[0];
        unsigned char *out = (unsigned char *) output_items[0];

        for (int i = 0; i < noutput_items; i += 188)
        {
            // Sanity check on incoming data
            assert(in[i].data[0] == MPEG_TS_SYNC_BYTE);
            assert((in[i].data[1] & MPEG_TS_ERROR_BIT) == 0);

            for (int j = 0; j < MPEG_TS_PKT_LENGTH; ++j)
            {
                out[i + j] = in[i + j] ^ sequence[counter];
                counter = (counter + 1) % DVB_RANDOMIZER_PERIOD;
            }
        }

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace dvbs */
} /* namespace gr */

