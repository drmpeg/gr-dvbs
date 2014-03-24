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
#include "reed_solomon_enc_bb_impl.h"

namespace gr {
  namespace dvbs {

    reed_solomon_enc_bb::sptr
    reed_solomon_enc_bb::make()
    {
      return gnuradio::get_initial_sptr
        (new reed_solomon_enc_bb_impl());
    }

    /*
     * The private constructor
     */
    reed_solomon_enc_bb_impl::reed_solomon_enc_bb_impl()
      : gr::block("reed_solomon_enc_bb",
              gr::io_signature::make(1, 1, sizeof(unsigned char)),
              gr::io_signature::make(1, 1, sizeof(unsigned char)))
    {
        rs = init_rs_char(DVB_RS_SYMSIZE, DVB_RS_GFPOLY, DVB_RS_FCR, DVB_RS_PRIM, DVB_RS_NROOTS);
        set_output_multiple(204);
    }

    /*
     * Our virtual destructor.
     */
    reed_solomon_enc_bb_impl::~reed_solomon_enc_bb_impl()
    {
        free_rs_char(rs);
    }

    void
    reed_solomon_enc_bb_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        ninput_items_required[0] = (noutput_items / 204) * 188;
    }

void reed_solomon_enc_bb_impl::encode(const unsigned char *in, unsigned char *out)
{
	assert(sizeof(in.data) == MPEG_TS_PKT_LENGTH);
	unsigned char data[DVB_RS_PAD_LENGTH + MPEG_TS_PKT_LENGTH];

	// Shortened RS: prepend zero bytes to message (discarded after encoding)
	std::memset(data, 0, DVB_RS_PAD_LENGTH);
	std::memcpy(&data[DVB_RS_PAD_LENGTH], in, MPEG_TS_PKT_LENGTH);

	// Copy input message to output then append RS bits
	std::memcpy(out, in, MPEG_TS_PKT_LENGTH);
	encode_rs_char(rs, data, &out[MPEG_TS_PKT_LENGTH]);
}

    int
    reed_solomon_enc_bb_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const unsigned char *in = (const unsigned char *) input_items[0];
        unsigned char *out = (unsigned char *) output_items[0];
        int j = 0;

        for (int i = 0; i < noutput_items; i += 204) {
            // Check sync byte
            assert(in[i].data[0] == MPEG_TS_SYNC_BYTE || in[i].data[0] == MPEG_TS_SYNC_BYTE_INV);

            encode(in + j, out + i);
            j += 188;
        }

        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each (j);

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace dvbs */
} /* namespace gr */

