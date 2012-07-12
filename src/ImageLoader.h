/*
 * ImageLoader.h
 *
 * An image loader designed to read a 32-bit bitmap. Although it may be extended to be
 * do other things too in the future.
 *
 *  Created on: 2010-08-11
 *      Author: Michael Yagudaev
 *      Copyright: yagudaev.com
 *      Version: $0.1.0$
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License Version 3 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifndef IMAGELOADER_H_
#define IMAGELOADER_H_

typedef unsigned char BYTE;
typedef int LONG;
typedef unsigned int DWORD;
typedef unsigned short WORD;

//File information header
//provides general information about the file
typedef struct __attribute__ ((__packed__)) tagBITMAPFILEHEADER
{
  WORD    bfType;
  DWORD   bfSize;
  WORD    bfReserved1;
  WORD    bfReserved2;
  DWORD   bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

//Bitmap information header
//provides information specific to the image data
typedef struct __attribute__ ((__packed__)) tagBITMAPINFOHEADER
{
  DWORD  biSize;
  LONG   biWidth;
  LONG   biHeight;
  WORD   biPlanes;
  WORD   biBitCount;
  DWORD  biCompression;
  DWORD  biSizeImage;
  DWORD  biXPelsPerMeter;
  DWORD  biYPelsPerMeter;
  DWORD  biClrUsed;
  DWORD  biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

//Colour palette
typedef struct __attribute__ ((__packed__)) tagRGBQUAD
{
  BYTE    rgbBlue;
  BYTE    rgbGreen;
  BYTE    rgbRed;
  BYTE    rgbReserved;
} RGBQUAD;


class ImageLoader
{
public:
    //variables

    //methods

    /**
     * Initializes an empty image
     */
    ImageLoader(void);

    /**
     * Initializes an image with the given image loaded from disk
     */
    ImageLoader(const char *fileName);

    /**
     * Destructor...
     */
    virtual ~ImageLoader();

    /**
     * Loads the given image.
     * @return True on success false on failure
     */
    bool loadBMP(const char *fileName);

    /**
     * Get the alpha channel as an array of bytes
     * @param size The size of the returned array, will return -1 on failure
     */
    BYTE *getAlpha() const;

    // Getter and setters...
    LONG getHeight() const
    {
        return height;
    }

    RGBQUAD *getColors() const
    {
        return colors;
    }

    bool getLoaded() const
    {
        return loaded;
    }

    BYTE *getPixelData() const
    {
        return pixelData;
    }

    LONG getWidth() const
    {
        return width;
    }

private:
    //variables
    BITMAPFILEHEADER bmfh;
    BITMAPINFOHEADER bmih;
    RGBQUAD *colors;
    BYTE *pixelData;
    bool loaded;
    LONG width;
    LONG height;
    WORD bpp;

    //methods
    void reset(void);
    bool fixPadding(BYTE const * const tempPixelData, DWORD size);
};
#endif /* IMAGELOADER_H_ */
