#!/usr/bin/env /usr/bin/python

# Copyright 2014 Ron Economos (w6rz@comcast.net)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

from gnuradio import blocks
from gnuradio import digital
from gnuradio import filter
from gnuradio import gr
from gnuradio import trellis
from gnuradio.filter import firdes
import dvbs
import osmosdr
import sys

def main(args):
    nargs = len(args)
    if nargs == 1:
        infile  = args[0]
        outfile = None
    elif nargs == 2:
        infile  = args[0]
        outfile  = args[1]
    else:
        sys.stderr.write("Usage: dvbs-blade.py input_file [output_file]\n");
        sys.exit(1)

    symbol_rate = 4500000
    samp_rate = symbol_rate * 2
    code_rate = dvbs.C1_2
    rrc_taps = 20
    center_freq = 435000000
    txvga1_gain = -4
    txvga2_gain = 1
    bandwidth = 6000000

    tb = gr.top_block()

    src = blocks.file_source(gr.sizeof_char, infile, True)

    dvbs_randomizer = dvbs.randomizer_bb()
    dvbs_reed_solomon_enc = dvbs.reed_solomon_enc_bb()
    dvbs_interleaver = dvbs.interleaver_bb()
    blocks_packed_to_unpacked = blocks.packed_to_unpacked_bb(1, gr.GR_MSB_FIRST)
    trellis_encoder = trellis.encoder_bb(trellis.fsm(1, 2, (0171, 0133)), 0)
    blocks_unpack_k_bits = blocks.unpack_k_bits_bb(2)
    dvbs_puncture = dvbs.puncture_bb(code_rate)
    blocks_pack_k_bits = blocks.pack_k_bits_bb(2)
    dvbs_modulator = dvbs.modulator_bc()
    fft_filter = filter.fft_filter_ccc(1, (firdes.root_raised_cosine(1.79, samp_rate, samp_rate/2, 0.35, rrc_taps)), 1)
    fft_filter.declare_sample_delay(0)

    out = osmosdr.sink(args="bladerf=0,buffers=128,buflen=32768")
    out.set_sample_rate(samp_rate)
    out.set_center_freq(center_freq, 0)
    out.set_freq_corr(0, 0)
    out.set_gain(txvga2_gain, 0)
    out.set_bb_gain(txvga1_gain, 0)
    out.set_bandwidth(bandwidth, 0)

    tb.connect(src, dvbs_randomizer)
    tb.connect(dvbs_randomizer, dvbs_reed_solomon_enc)
    tb.connect(dvbs_reed_solomon_enc, dvbs_interleaver)
    tb.connect(dvbs_interleaver, blocks_packed_to_unpacked)
    tb.connect(blocks_packed_to_unpacked, trellis_encoder)
    tb.connect(trellis_encoder, blocks_unpack_k_bits)
    tb.connect(blocks_unpack_k_bits, dvbs_puncture)
    tb.connect(dvbs_puncture, blocks_pack_k_bits)
    tb.connect(blocks_pack_k_bits, dvbs_modulator)
    tb.connect(dvbs_modulator, fft_filter)
    tb.connect(fft_filter, out)

    if outfile:
        dst = blocks.file_sink(gr.sizeof_gr_complex, outfile)
        tb.connect(fft_filter, dst)

    tb.run()


if __name__ == '__main__':
    main(sys.argv[1:])

