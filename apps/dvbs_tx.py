#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Dvbs Tx
# Generated: Wed Jun 18 01:23:58 2014
##################################################

from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import filter
from gnuradio import gr
from gnuradio import trellis
from gnuradio import wxgui
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from gnuradio.wxgui import fftsink2
from grc_gnuradio import wxgui as grc_wxgui
from optparse import OptionParser
import dvbs
import osmosdr
import wx

class dvbs_tx(grc_wxgui.top_block_gui):

    def __init__(self):
        grc_wxgui.top_block_gui.__init__(self, title="Dvbs Tx")

        ##################################################
        # Variables
        ##################################################
        self.symbol_rate = symbol_rate = 4500000
        self.samp_rate = samp_rate = symbol_rate * 2
        self.rrc_taps = rrc_taps = 20

        ##################################################
        # Blocks
        ##################################################
        self.wxgui_fftsink2_0 = fftsink2.fft_sink_c(
        	self.GetWin(),
        	baseband_freq=435000000,
        	y_per_div=10,
        	y_divs=10,
        	ref_level=0,
        	ref_scale=2.0,
        	sample_rate=samp_rate,
        	fft_size=1024,
        	fft_rate=15,
        	average=True,
        	avg_alpha=None,
        	title="FFT Plot",
        	peak_hold=False,
        )
        self.Add(self.wxgui_fftsink2_0.win)
        self.trellis_encoder_xx_0 = trellis.encoder_bb(trellis.fsm(1, 2, (0171, 0133)), 0)
        self.osmosdr_sink_0 = osmosdr.sink( args="numchan=" + str(1) + " " + "" )
        self.osmosdr_sink_0.set_sample_rate(samp_rate)
        self.osmosdr_sink_0.set_center_freq(435e6, 0)
        self.osmosdr_sink_0.set_freq_corr(0, 0)
        self.osmosdr_sink_0.set_gain(2, 0)
        self.osmosdr_sink_0.set_if_gain(0, 0)
        self.osmosdr_sink_0.set_bb_gain(-4, 0)
        self.osmosdr_sink_0.set_antenna("", 0)
        self.osmosdr_sink_0.set_bandwidth(6000000, 0)
          
        self.fft_filter_xxx_0 = filter.fft_filter_ccc(1, (firdes.root_raised_cosine(1.79, samp_rate, samp_rate/2, 0.35, rrc_taps)), 1)
        self.fft_filter_xxx_0.declare_sample_delay(0)
        self.dvbs_reed_solomon_enc_bb_0 = dvbs.reed_solomon_enc_bb()
        self.dvbs_randomizer_bb_0 = dvbs.randomizer_bb()
        self.dvbs_puncture_bb_0 = dvbs.puncture_bb(dvbs.C1_2)
        self.dvbs_modulator_bc_0 = dvbs.modulator_bc()
        self.dvbs_interleaver_bb_0 = dvbs.interleaver_bb()
        self.blocks_unpack_k_bits_bb_0 = blocks.unpack_k_bits_bb(2)
        self.blocks_packed_to_unpacked_xx_0 = blocks.packed_to_unpacked_bb(1, gr.GR_MSB_FIRST)
        self.blocks_pack_k_bits_bb_0 = blocks.pack_k_bits_bb(2)
        self.blocks_file_source_0 = blocks.file_source(gr.sizeof_char*1, "/home/re/xfer/adv.ts", False)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_file_source_0, 0), (self.dvbs_randomizer_bb_0, 0))
        self.connect((self.dvbs_randomizer_bb_0, 0), (self.dvbs_reed_solomon_enc_bb_0, 0))
        self.connect((self.dvbs_reed_solomon_enc_bb_0, 0), (self.dvbs_interleaver_bb_0, 0))
        self.connect((self.dvbs_interleaver_bb_0, 0), (self.blocks_packed_to_unpacked_xx_0, 0))
        self.connect((self.blocks_packed_to_unpacked_xx_0, 0), (self.trellis_encoder_xx_0, 0))
        self.connect((self.trellis_encoder_xx_0, 0), (self.blocks_unpack_k_bits_bb_0, 0))
        self.connect((self.blocks_unpack_k_bits_bb_0, 0), (self.dvbs_puncture_bb_0, 0))
        self.connect((self.dvbs_puncture_bb_0, 0), (self.blocks_pack_k_bits_bb_0, 0))
        self.connect((self.dvbs_modulator_bc_0, 0), (self.fft_filter_xxx_0, 0))
        self.connect((self.blocks_pack_k_bits_bb_0, 0), (self.dvbs_modulator_bc_0, 0))
        self.connect((self.fft_filter_xxx_0, 0), (self.osmosdr_sink_0, 0))
        self.connect((self.fft_filter_xxx_0, 0), (self.wxgui_fftsink2_0, 0))


# QT sink close method reimplementation

    def get_symbol_rate(self):
        return self.symbol_rate

    def set_symbol_rate(self, symbol_rate):
        self.symbol_rate = symbol_rate
        self.set_samp_rate(self.symbol_rate * 2)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.wxgui_fftsink2_0.set_sample_rate(self.samp_rate)
        self.fft_filter_xxx_0.set_taps((firdes.root_raised_cosine(1.79, self.samp_rate, self.samp_rate/2, 0.35, self.rrc_taps)))
        self.osmosdr_sink_0.set_sample_rate(self.samp_rate)

    def get_rrc_taps(self):
        return self.rrc_taps

    def set_rrc_taps(self, rrc_taps):
        self.rrc_taps = rrc_taps
        self.fft_filter_xxx_0.set_taps((firdes.root_raised_cosine(1.79, self.samp_rate, self.samp_rate/2, 0.35, self.rrc_taps)))

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"
    parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
    (options, args) = parser.parse_args()
    tb = dvbs_tx()
    tb.Start(True)
    tb.Wait()

