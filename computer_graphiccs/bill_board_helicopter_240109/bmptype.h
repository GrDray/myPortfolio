#ifndef BMPTYPE__H
#define BMPTYPE__H

//#include <Windows.h>

typedef unsigned int       mDWORD;
typedef unsigned char      BYTE;
typedef unsigned short int WORD;
typedef unsigned int       mLONG;

#pragma once

typedef struct mtagBITMAPINFOHEADER{
  mDWORD  biSize; 
  mLONG   biWidth; 
  mLONG   biHeight; 
  WORD   biPlanes; 
  WORD   biBitCount; 
  mDWORD  biCompression; 
  mDWORD  biSizeImage; 
  mLONG   biXPelsPerMeter; 
  mLONG   biYPelsPerMeter; 
  mDWORD  biClrUsed; 
  mDWORD  biClrImportant; 
} mBITMAPINFOHEADER, *mPBITMAPINFOHEADER;

typedef struct mtagBITMAPFILEHEADER { 
  unsigned short        bfType; 
  unsigned int          bfSize; 
  unsigned short        bfReserved1; 
  unsigned short        bfReserved2; 
  unsigned int          bfOffBits; 
} mBITMAPFILEHEADER;

typedef struct mtagRGBTRIPLE { 
  BYTE rgbtBlue; 
  BYTE rgbtGreen; 
  BYTE rgbtRed; 
} mRGBTRIPLE; 

#endif


/*------------------------------------------------------
 * Procedure to read a BMP file and keep the results in
 * a gray-level array. The intensities are of float type.
 * Input:
 *   filename[]: name of the input file.
 * Output:
 *   width, height: width and height of the image data.
 *   imgData[]: array to keep the intensity.
 * Return 0, if successful, -1, if fail.
 */
int  readBMPfile(char *filename, int *width, int *height, float **imgData);



/*------------------------------------------------------------------
 *	Procedure: to save the rgb-image pic[] into 24-bit BMP
 *	image file.
 *      pic: the image in unsigned byte data type.
 *	w, h: width & height of the image
 */
void save_rgb_2_BMP24(unsigned char pic[], int w, int h, 
                      unsigned char filename[]);


/*-------------------------------------------------------------
 * Procedure to write a gray-level (0-255) image into a .BMP
 * file. 
 *1. Each pixel is represented by using a floating number.
 *   The gray-level range is [0-255].
 *2. The output .bmp file is not compressed.
 *3. The .bmp file is in RGB format (24-bits).
 *Input:
 *   img[]: the input image,
 *   w, h: the width & height of the image.
 *   filename: name of the .bmp file.
 *Output:
 *   A .bmp file.
 */
void save_float_2_BMP24(float img[], int w, int h, unsigned char filename[]);


/////////////////////////////////////////////////////////////////////






