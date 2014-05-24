/*
 * Copyright (c) 2010 Edmund Tse, 2014 Ron Economos
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef INCLUDED_DVB_CONSTS_H
#define INCLUDED_DVB_CONSTS_H

#include <cstddef>    // For size_t definition

// MPEG Transport Stream constants
static const unsigned char MPEG_TS_SYNC_BYTE    = 0x47;
static const int MPEG_TS_PKT_LENGTH             = 188;
static const unsigned char MPEG_TS_ERROR_BIT    = 0x80;    // top bit of byte after SYNC

// Reed-Solomon parameters
static const int DVB_RS_ENCODED_LENGTH    = 204;    // i.e. RS(204,188)
static const int DVB_RS_PAD_LENGTH        = 51;     // Shortened from RS(255,239) code by padding with 51 zeros
static const int DVB_RS_SYMSIZE           = 8;      // Bits per symbol
static const int DVB_RS_GFPOLY            = 0x11d;  // Extended Galois field generator polynomial coefficients, with the 0th coefficient in the low order bit. The polynomial must be primitive.
static const int DVB_RS_FCR               = 0;      // First consecutive root of the generator polynomial in index form
static const int DVB_RS_PRIM              = 0x01;   // Primitive element to generate polynomial roots
static const int DVB_RS_NROOTS            = 16;     // Number of generator roots = number of parity symbols

// Interleaver parameters
static const int DVB_INTERLEAVER_I        = 12;     // Interleaving depth
static const int DVB_INTERLEAVER_M        = 17;     // Cell size

// Randomiser constants
static const unsigned char MPEG_TS_SYNC_BYTE_INV = MPEG_TS_SYNC_BYTE ^ 0xFF;
static const int DVB_RANDOMIZER_INVERTED_SYNC_PERIOD = 8;
static const int DVB_RANDOMIZER_PERIOD = MPEG_TS_PKT_LENGTH * DVB_RANDOMIZER_INVERTED_SYNC_PERIOD;

// Symbol to insert when depuncturing
static const float DVB_DEPUNCTURE_SYMBOL = 0.0;

// Viterbi parameters
static const int k = 1;
static const int n = 2;
static const int G_array[] = {0171, 0133};
static const int K = 204 * 8;
static const int dimensionality = 2;
static const float c_array[] = {0.7, 0.7, 0.7, -0.7, -0.7, 0.7, -0.7, -0.7};

// Sync decoder
static const int CONFIDENCE_THRESHOLD = 4;

// Sync status update period (and dwell time for a parameter set)
// This parameter defines the period (in bits) for the sync decoder to call its
// status update function. This callback is used by the depuncture-viterbi-sync
// block to determine whether or not the current set of adjustments to the
// complex input stream is correct.
// A shorter DWELL_DURATION gives shorter the time required to find the correct
// adjustments to be able to synchronise to the data stream, but it should be
// long enough for the sync decoder to lock onto the signal and notify.
static const int DWELL_DURATION = 60*204*8;

// Baseband shaping
static const float RRC_ROLLOFF_FACTOR = 0.35;

#endif // INCLUDED_DVB_CONSTS_H
