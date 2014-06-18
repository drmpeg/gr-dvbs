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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "modulator_bc_impl.h"

namespace gr {
  namespace dvbs {

    modulator_bc::sptr
    modulator_bc::make()
    {
      return gnuradio::get_initial_sptr
        (new modulator_bc_impl());
    }

    /*
     * The private constructor
     */
    modulator_bc_impl::modulator_bc_impl()
      : gr::block("modulator_bc",
              gr::io_signature::make(1, 1, sizeof(unsigned char)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {
        qpsk[0].real() = (cos(M_PI / 4.0));
        qpsk[0].imag() = (sin(M_PI / 4.0));
        qpsk[1].real() = (cos(7 * M_PI / 4.0));
        qpsk[1].imag() = (sin(7 * M_PI / 4.0));
        qpsk[2].real() = (cos(3 * M_PI / 4.0));
        qpsk[2].imag() = (sin(3 * M_PI / 4.0));
        qpsk[3].real() = (cos(5 * M_PI / 4.0));
        qpsk[3].imag() = (sin(5 * M_PI / 4.0));
        qpsk[4].real() = 0.0;
        qpsk[4].imag() = 0.0;
        set_output_multiple(2);
    }

    /*
     * Our virtual destructor.
     */
    modulator_bc_impl::~modulator_bc_impl()
    {
    }

    void
    modulator_bc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        ninput_items_required[0] = noutput_items / 2;
    }

    int
    modulator_bc_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const unsigned char *in = (const unsigned char *) input_items[0];
        gr_complex *out = (gr_complex *) output_items[0];
        int index;

        for (int i = 0; i < noutput_items / 2; i++)
        {
            index = *in++;
            *out++ = qpsk[index & 0x3];
            *out++ = qpsk[4];            
        }

        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each (noutput_items / 2);

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace dvbs */
} /* namespace gr */

