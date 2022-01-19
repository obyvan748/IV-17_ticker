/*
 * iv-17 fonts V1.0 Rev.0
 *
 * VFD Font for IV-17
 * (C) 2021 Jörg Postert
 * 
 * with ideas from https://github.com/akafugu/VFDDeluxe
 *
 * This program is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 */

unsigned long calculate_segments_16(unsigned char character);

// setup for Breadboard and MAX6921 Pins 
#define segM  0b00000000000000000001   // MAX6921 OUT1
#define segD1 0b00000000000000000010
#define segE  0b00000000000000000100
#define dotL  0b00000000000000001000
#define segF  0b00000000000000100000
#define segA1 0b00000000000001000000
#define segG1 0b00000000000010000000
#define segJ  0b00000000000100000000
#define segH  0b00000000001000000000
#define segK  0b00000000010000000000
#define segG2 0b00000000100000000000
#define segA2 0b00000001000000000000
#define segB  0b00000010000000000000
#define GRID  0b00000100000000000000   // MAX6921 OUT14
#define dotR  0b00001000000000000000
#define segC  0b00010000000000000000
#define segD2 0b00100000000000000000
#define segL  0b01000000000000000000
#define segI  0b10000000000000000000


// asterisk
#define asterisk segH + segI + segJ + segK + segL + segM + segG1 + segG2   


unsigned long segments_16_test[] = {
    segA1 + segA2 + segB + segC + segD1 + segD2 + segE + segF + segK + segM,
    segB + segC + segK,
};

// 0-20
unsigned long segments_16_numerals[] = {
    segA1 + segA2 + segB + segC + segD1 + segD2 + segE + segF + segK + segM,   // 0
    segA1 + segH + segI+ segD1 + segD2,                                        // 1 
    segA1 + segA2 + segB + segG2 + segM + segD1 + segD2,                       // 2
    segA1 + segA2 + segK + segG2 + segC + segD1 + segD2,                       // 3
    segB + segC + segJ + segG2,                                                // 4
    segA1 + segA2 + segJ + segG2 + segC + segD2,                               // 5
    segA1 + segD1 + segD2 + segE + segF + segG1 + segL,                        // 6
    segA1 + segA2 + segK + segI,                                               // 7
    segA1 + segA2 + segJ + segK + segG1 + segG2 + segE + segC + segD1 + segD2, // 8
    segA1 + segA2 + segB + segJ + segG2 + segC + segD2,                        // 9  
    segE + segF + segA2 + segB + segC + segD2 + segI + segH,                   // 10
    segE + segF + segB + segC,                                                 // 11
    segE + segF + segA2 + segB + segG2 + segI + segD2,                         // 12
    segE + segF + segA2 + segB + segC + segD2 + segG2,                         // 13
    segE + segF + segH + segB + segG2 + segC,                                  // 14
    segE + segF + segA2 + segH + segG2 + segC + segD2,                         // 15
    segE + segF + segA2 + segH + segG2 + segI + segC + segD2,                  // 16
    segE + segF + segA2 + segB + segC,                                         // 17
    segE + segF + segA2 + segB + segH + segG2 + segI + segC + segD2,           // 18
    segE + segF + segA2 + segH + segB + segG2 + segC + segD2,                  // 19
};

// A-Z
unsigned long segments_16_alpha_upper[] = {
    segA1 + segA2 + segB + segC + segE + segF + segG1 + segG2,
    segA1 + segA2 + segB + segC + segD1 + segD2 + segG2 + segI + segH,
    segA1 + segA2 + segD1 + segD2 + segE + segF,
    segA1 + segA2 + segB + segC + segD1 + segD2 + segI + segH,
    segA1 + segA2 + segD1 + segD2 + segE + segF + segG1 + segG2,
    segA1 + segA2 + segE + segF + segG1,
    segA1 + segA2 + segC + segD1 + segD2 + segE + segF + segG2,
    segB + segC + segE + segF + segG1 + segG2,
    segA1 + segA2 + segI + segH + segD1 + segD2,
    segB + segC + segD1 + segD2 + segE,
    segE + segF + segG1 + segK + segL,
    segD1 + segD2 + segE + segF,
    segB + segC + segE + segF + segJ + segK,
    segB + segC + segE + segF + segJ + segL,
    segA1 + segA2 + segB + segC + segD1 + segD2 + segE + segF,
    segA1 + segA2 + segB + segE + segF + segG1 + segG2,
    segA1 + segA2 + segB + segC + segD1 + segD2 + segE + segF + segL,
    segA1 + segA2 + segB + segE + segF + segG1 + segG2 + segL,
    segA1 + segA2 + segC + segD1 + segD2 + segF + segG1 + segG2,
    segA1 + segA2 + segI + segH,
    segB + segC + segD1 + segD2 + segE + segF,
    segE + segF + segK + segM,
    segB + segC + segE + segF + segL + segM,
    segJ + segK + segL + segM,
    segJ + segK + segI,
    segA1 + segA2 + segD1 + segD2 + segK + segM
};

unsigned long calculate_segments_16(unsigned char ch)
{
    unsigned long segs = 0;
    if ((ch >= '0') && (ch <= '9'))
        segs = segments_16_numerals[ch - 48]; // 0-9
    else if ((ch >= 'a') && (ch <= 'z'))
        segs = segments_16_alpha_upper[ch - 'a']; // a-z
    else if ((ch >= 'A') && (ch <= 'Z'))
        segs = segments_16_alpha_upper[ch - 'A']; // A-Z
    else if ((ch >= 0) && (ch <= 20))
        segs = segments_16_numerals[ch]; // 0-20
    else if (ch == ' ')
        segs = 0;
    else if (ch == '-')
        segs = segG1 + segG2;
    else if (ch == '+')
        segs = segG1 + segG2 + segI + segL;
    else if (ch == '<')
        segs = segJ + segK;
    else if (ch == '>')
        segs = segH + segM;
    else if (ch == '/')
        segs = segJ + segM;
    else if (ch == '&') // stand-in for degree symbol
        segs = segA2 + segB + segG2 + segI;
    else
        segs = segH + segI + segJ + segK + segL + segM + segG1 + segG2; // asterisk
    return segs;
}
