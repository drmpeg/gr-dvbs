/* -*- c++ -*- */

#define DVBS_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "dvbs_swig_doc.i"

%{
#include "dvbs/randomizer_bb.h"
#include "dvbs/reed_solomon_enc_bb.h"
#include "dvbs/interleaver_bb.h"
#include "dvbs/puncture_bb.h"
#include "dvbs/dvbs_config.h"
#include "dvbs/modulator_bc.h"
%}


%include "dvbs/dvbs_config.h"
%include "dvbs/randomizer_bb.h"
GR_SWIG_BLOCK_MAGIC2(dvbs, randomizer_bb);
%include "dvbs/reed_solomon_enc_bb.h"
GR_SWIG_BLOCK_MAGIC2(dvbs, reed_solomon_enc_bb);

%include "dvbs/interleaver_bb.h"
GR_SWIG_BLOCK_MAGIC2(dvbs, interleaver_bb);

%include "dvbs/puncture_bb.h"
GR_SWIG_BLOCK_MAGIC2(dvbs, puncture_bb);
%include "dvbs/modulator_bc.h"
GR_SWIG_BLOCK_MAGIC2(dvbs, modulator_bc);
