/**
 * @file adc.h
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2024-03-27
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include <stdint.h>

struct ADCRegMap {
  volatile uint32_t statr;
  volatile uint32_t ctlr1;
  volatile uint32_t ctlr2;
  volatile uint32_t samptr1;
  volatile uint32_t samptr2;
  volatile uint32_t iofr[4];
  volatile uint32_t wdhtr;
  volatile uint32_t wdltr;
  volatile uint32_t rsqr[3];
  volatile uint32_t isqr;
  volatile uint32_t idatar[4];
  volatile uint32_t rdatar;
};

#ifdef LIBCH32_HAS_ADC1
extern struct ADCRegMap adc1;
#endif

#ifdef LIBCH32_HAS_ADC2
extern struct ADCRegMap adc2;
#endif

// STATR
//------
// ADC_STATR_STRT[4]
static const uint32_t ADC_STATR_STRT = (1 << 4);
// JSTRT[3]
static const uint32_t ADC_STATR_JSTRT = (1 << 3);
// JEOC[2]
static const uint32_t ADC_STATR_JEOC = (1 << 2);
// EOC[1]
static const uint32_t ADC_STATR_EOC = (1 << 1);
// AWD[0]
static const uint32_t ADC_STATR_AWD = (1 << 0);

// CTLR1
//------
// PGA[28:27]
static const uint32_t ADC_CTLR1_PGA_MASK = (0b11 << 27);
static const uint32_t ADC_CTLR1_PGA_X1 = (0b00 << 27);
static const uint32_t ADC_CTLR1_PGA_X4 = (0b01 << 27);
static const uint32_t ADC_CTLR1_PGA_X16 = (0b10 << 27);
static const uint32_t ADC_CTLR1_PGA_X64 = (0b11 << 27);
// BUFEN[26]
static const uint32_t ADC_CTLR1_BUFEN = (1 << 26);
// TKITUNE[25]
static const uint32_t ADC_CTLR1_TKITUNE = (1 << 25);
// TKENABLE[24]
static const uint32_t ADC_CTLR1_TKENABLE = (1 << 24);
// AWDEN[23]
static const uint32_t ADC_CTLR1_AWDEN = (1 << 23);
// JAWDEN[22]
static const uint32_t ADC_CTLR1_JAWDEN = (1 << 22);
// DUALMOD[19:16]
static const uint32_t ADC_CTLR1_DUALMOD_MASK = (0b1111 << 16);
static const uint32_t ADC_CTLR1_DUALMOD_INDEPENDENT = (0b0000 << 16);
static const uint32_t ADC_CTLR1_DUALMOD_REGULAT_SIMULTANEOUS_PLUS_INJECTED_SIMULTANEOUS = (0b0001 << 16);
static const uint32_t ADC_CTLR1_DUALMOD_REGULAT_SIMULTANEOUS_PLUS_ALT_TRIG = (0b0010 << 16);
static const uint32_t ADC_CTLR1_DUALMOD_INJECTED_SIMULTANEOUS_PLUS_FAST_INTERLEAVED = (0b0011 << 16);
static const uint32_t ADC_CTLR1_DUALMOD_INJECTED_SIMULTANEOUS_PLUS_SLOW_INTERLEAVED = (0b0100 << 16);
static const uint32_t ADC_CTLR1_DUALMOD_INJECTED_SIMULTANEOUS = (0b0101 << 16);
static const uint32_t ADC_CTLR1_DUALMOD_REGULAT_SIMULTANEOUS = (0b0110 << 16);
static const uint32_t ADC_CTLR1_DUALMOD_FAST_INTERLEAVED = (0b0111 << 16);
static const uint32_t ADC_CTLR1_DUALMOD_SLOW_INTERLEAVED = (0b1000 << 16);
static const uint32_t ADC_CTLR1_DUALMOD_ALT_TRIGGER = (0b1001 << 16);
// DISCNUM[15:13]
static const uint32_t ADC_CTLR1_DISCNUM_MASK = (0b111 << 13);
static const uint32_t ADC_CTLR1_DISCNUM_1_CHAN = (0b000 << 13);
static const uint32_t ADC_CTLR1_DISCNUM_2_CHAN = (0b001 << 13);
static const uint32_t ADC_CTLR1_DISCNUM_3_CHAN = (0b010 << 13);
static const uint32_t ADC_CTLR1_DISCNUM_4_CHAN = (0b011 << 13);
static const uint32_t ADC_CTLR1_DISCNUM_5_CHAN = (0b100 << 13);
static const uint32_t ADC_CTLR1_DISCNUM_6_CHAN = (0b101 << 13);
static const uint32_t ADC_CTLR1_DISCNUM_7_CHAN = (0b110 << 13);
static const uint32_t ADC_CTLR1_DISCNUM_8_CHAN = (0b111 << 13);
// JDISCEN[12]
static const uint32_t ADC_CTLR1_JDISCEN = (1 << 12);
// DISCEN[11]
static const uint32_t ADC_CTLR1_DISCEN = (1 << 11);
// JAUTO[10]
static const uint32_t ADC_CTLR1_JAUTO = (1 << 10);
// AWDSGL[9]
static const uint32_t ADC_CTLR1_AWDSGL = (1 << 9);
// SCAN[8]
static const uint32_t ADC_CTLR1_SCAN = (1 << 8);
// JEOCIE[7]
static const uint32_t ADC_CTLR1_JEOCIE = (1 << 7);
// AWDIE[6]
static const uint32_t ADC_CTLR1_AWDIE = (1 << 6);
// EOCIE[5]
static const uint32_t ADC_CTLR1_EOCIE = (1 << 5);
// AWDCH[4:0]
static const uint32_t ADC_CTLR1_AWDCH_MASK = (0b00000 << 0);
static const uint32_t ADC_CTLR1_AWDCH_CHAN_0 = (0b00000 << 0);
static const uint32_t ADC_CTLR1_AWDCH_CHAN_1 = (0b00001 << 0);
static const uint32_t ADC_CTLR1_AWDCH_CHAN_2 = (0b00010 << 0);
static const uint32_t ADC_CTLR1_AWDCH_CHAN_3 = (0b00011 << 0);
static const uint32_t ADC_CTLR1_AWDCH_CHAN_4 = (0b00100 << 0);
static const uint32_t ADC_CTLR1_AWDCH_CHAN_5 = (0b00101 << 0);
static const uint32_t ADC_CTLR1_AWDCH_CHAN_6 = (0b00110 << 0);
static const uint32_t ADC_CTLR1_AWDCH_CHAN_7 = (0b00111 << 0);
static const uint32_t ADC_CTLR1_AWDCH_CHAN_8 = (0b01000 << 0);
static const uint32_t ADC_CTLR1_AWDCH_CHAN_9 = (0b01001 << 0);
static const uint32_t ADC_CTLR1_AWDCH_CHAN_10 = (0b01010 << 0);
static const uint32_t ADC_CTLR1_AWDCH_CHAN_11 = (0b01011 << 0);
static const uint32_t ADC_CTLR1_AWDCH_CHAN_12 = (0b01100 << 0);
static const uint32_t ADC_CTLR1_AWDCH_CHAN_13 = (0b01101 << 0);
static const uint32_t ADC_CTLR1_AWDCH_CHAN_14 = (0b01110 << 0);
static const uint32_t ADC_CTLR1_AWDCH_CHAN_15 = (0b01111 << 0);
static const uint32_t ADC_CTLR1_AWDCH_CHAN_16 = (0b10000 << 0);
static const uint32_t ADC_CTLR1_AWDCH_CHAN_17 = (0b10001 << 0);

// CTLR2
//-------
// TSVREFE[23]
static const uint32_t ADC_CTLR2_TSVREFE = (1 << 23);
// SWSTART[22]
static const uint32_t ADC_CTLR2_SWSTART = (1 << 22);
// JSWSTART[21]
static const uint32_t ADC_CTLR2_JSWSTART = (1 << 21);
// EXTTRIG[20]
static const uint32_t ADC_CTLR2_EXTTRIG = (1 << 20);
// EXTSEL[19:17]
static const uint32_t ADC_CTLR2_EXTSEL_MASK = (0b000 << 17);
static const uint32_t ADC_CTLR2_EXTSEL_CC1_EVT_TIMR_1 = (0b000 << 17);
static const uint32_t ADC_CTLR2_EXTSEL_CC2_EVT_TIMR_1 = (0b001 << 17);
static const uint32_t ADC_CTLR2_EXTSEL_CC3_EVT_TIMR_1 = (0b010 << 17);
static const uint32_t ADC_CTLR2_EXTSEL_CC2_EVT_TIMR_2 = (0b011 << 17);
static const uint32_t ADC_CTLR2_EXTSEL_TRGO_EVT_TIMR_3 = (0b100 << 17);
static const uint32_t ADC_CTLR2_EXTSEL_CC4_EVT_TIMR_4 = (0b101 << 17);
static const uint32_t ADC_CTLR2_EXTSEL_EXTI_LINE_11 = (0b110 << 17);
static const uint32_t ADC_CTLR2_EXTSEL_TRGO_EVT_TIMR_8 = (0b110 << 17);  // Not a mistake
static const uint32_t ADC_CTLR2_EXTSEL_SWSTART_SW_TRIG = (0b111 << 17);
// JEXTTRIG[15]
static const uint32_t ADC_CTLR2_JEXTTRIG = (1 << 15);
// JEXTSEL[14:12]
static const uint32_t ADC_CTLR2_JEXTSEL_MASK = (0b000 << 12);
static const uint32_t ADC_CTLR2_JEXTSEL_TRGO_EVT_TIMR_1 = (0b000 << 12);
static const uint32_t ADC_CTLR2_JEXTSEL_CC4_EVT_TIMR_1 = (0b001 << 12);
static const uint32_t ADC_CTLR2_JEXTSEL_TRG0_EVT_TIMR_2 = (0b010 << 12);
static const uint32_t ADC_CTLR2_JEXTSEL_CC1_EVT_TIMR_2 = (0b011 << 12);
static const uint32_t ADC_CTLR2_JEXTSEL_CC4_EVT_TIMR_3 = (0b100 << 12);
static const uint32_t ADC_CTLR2_JEXTSEL_TRG0_EVT_TIMR_4 = (0b101 << 12);
static const uint32_t ADC_CTLR2_JEXTSEL_EXTI_LINE_15 = (0b110 << 12);
static const uint32_t ADC_CTLR2_JEXTSEL_CC4_EVT_TIME_8 = (0b110 << 12);  // Not a mistake
static const uint32_t ADC_CTLR2_JEXTSEL_JSWSTART_SW_TRIG = (0b111 << 12);
// ALIGN[11]
static const uint32_t ADC_CTLR2_ALIGN = (1 << 11);
// DMA[10]
static const uint32_t ADC_CTLR2_DMA = (1 << 10);
// RSTCAL[3]
static const uint32_t ADC_CTLR2_RSTCAL = (1 << 3);
// CAL[2]
static const uint32_t ADC_CTLR2_CAL = (1 << 2);
// CONT[1]
static const uint32_t ADC_CTLR2_CONT = (1 << 1);
// ADON[0]
static const uint32_t ADC_CTLR2_ADON = (1 << 0);

// RSQR1
//-----
// L[23:20]
static const uint32_t ADC_RSQR1_L_MASK = (0b1111 << 20);
#define ADC_RSQR1_L_SET(l) ((l & 0b1111) << 20)
