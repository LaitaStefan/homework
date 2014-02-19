/*
 *  Bitmap Manipulation Structures

    Copyright (C) 2008 - 2012 Data Structures Team, UPB

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef BITMAP_H_
#define BITMAP_H_

#include <stdint.h>

//////////////////////////////////////////////////
// The Bitmap Format Header Structures
//
// Further details here: 
//  * http://msdn2.microsoft.com/en-us/library/ms532311(VS.85).aspx
//  * http://www.fortunecity.com/skyscraper/windows/364/bmpffrmt.html
//  * http://www.fileformat.info/format/bmp/egff.htm

// The first header in the Bitmap file
typedef struct tagBITMAPFILEHEADER { 
  uint16_t	bfType; 
  uint32_t	bfSize; 
  uint16_t	bfReserved1; 
  uint16_t	bfReserved2; 
  uint32_t	bfOffBits; 
} __attribute__((__packed__)) BITMAPFILEHEADER, *PBITMAPFILEHEADER;

// The second header in the Bitmap file
typedef struct tagBITMAPINFOHEADER {
  uint32_t	biSize; 
  int32_t	biWidth; 
  int32_t	biHeight; 
  uint16_t	biPlanes; 
  uint16_t	biBitCount; 
  uint32_t	biCompression; 
  uint32_t	biSizeImage; 
  int32_t	biXPelsPerMeter; 
  int32_t	biYPelsPerMeter; 
  uint32_t	biClrUsed; 
  uint32_t	biClrImportant; 
} __attribute__((__packed__)) BITMAPINFOHEADER, *PBITMAPINFOHEADER;

// An entry in the color table
typedef struct tagRGBQUAD {
	uint8_t rgbBlue;
	uint8_t rgbGreen;
	uint8_t rgbRed;
	uint8_t rgbReserved;
} __attribute__((__packed__)) RGBQUAD, *PRGBQUAD;

// Constants for the biCompression field
#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L
#define BI_JPEG       4L
#define BI_PNG        5L

// Custom MCB constant
#define BI_MCB		  0xC0FFEEL


#endif /*BITMAP_H_*/
