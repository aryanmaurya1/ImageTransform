#include <iostream>
#include <cmath>
#include <cstdlib>

#include "uiuc/PNG.h"
#include "uiuc/HSLAPixel.h"
#include "ImageTransform.h"

/* ******************
(Begin multi-line comment...)

Write your name and email address in the comment space here:

Name:
Email:

(...end multi-line comment.)
******************** */

using uiuc::PNG;
using uiuc::HSLAPixel;

/**
 * Returns an image that has been transformed to grayscale.
 *
 * The saturation of every pixel is set to 0, removing any color.
 *
 * @return The grayscale image.
 */
PNG grayscale(PNG image) {
  /// This function is already written for you so you can see how to
  /// interact with our PNG class.
  for (unsigned x = 0; x < image.width(); x++) {
    for (unsigned y = 0; y < image.height(); y++) {
      HSLAPixel & pixel = image.getPixel(x, y);

      // `pixel` is a reference to the memory stored inside of the PNG `image`,
      // which means you're changing the image directly. No need to `set`
      // the pixel since you're directly changing the memory of the image.
      pixel.s = 0;
    }
  }

  return image;
}



/**
 * Returns an image with a spotlight centered at (`centerX`, `centerY`).
 *
 * A spotlight adjusts the luminance of a pixel based on the distance the pixel
 * is away from the center by decreasing the luminance by 0.5% per 1 pixel euclidean
 * distance away from the center.
 *
 * For example, a pixel 3 pixels above and 4 pixels to the right of the center
 * is a total of `sqrt((3 * 3) + (4 * 4)) = sqrt(25) = 5` pixels away and
 * its luminance is decreased by 2.5% (0.975x its original value).  At a
 * distance over 160 pixels away, the luminance will always decreased by 80%.
 * 
 * The modified PNG is then returned.
 *
 * @param image A PNG object which holds the image data to be modified.
 * @param centerX The center x coordinate of the crosshair which is to be drawn.
 * @param centerY The center y coordinate of the crosshair which is to be drawn.
 *
 * @return The image with a spotlight.
 */
PNG createSpotlight(PNG image, int centerX, int centerY) {
    unsigned int ht = image.height();
    unsigned int wd = image.width();
    for(unsigned int i = 0; i < ht; i++){
        for(unsigned int j = 0; j < wd; j++){
                HSLAPixel& pix = image.getPixel(j,i);
                int x_diff = centerX - j;
                int y_diff = centerY - i;
                int dist = sqrt((x_diff * x_diff) + (y_diff * y_diff));
                int reductionPercent = dist * 0.5;
                
                
                if(dist >= 160){
                    pix.l = pix.l * 0.2;
                }else {
                    double reductionPercent = dist * 0.5;
                    pix.l = pix.l * (1.0 - (reductionPercent/100));
                }
        }
    }
    return image;
  
}
 

/**
 * Returns a image transformed to Illini colors.
 *
 * The hue of every pixel is set to the a hue value of either orange or
 * blue, based on if the pixel's hue value is closer to orange than blue.
 *
 * @param image A PNG object which holds the image data to be modified.
 *
 * @return The illinify'd image.
**/
PNG illinify(PNG image) {
    unsigned int ht = image.height();
    unsigned int wd = image.width();
    for(unsigned int i = 0; i < ht; i++){
        for(unsigned int j = 0; j < wd; j++){
            HSLAPixel& pix = image.getPixel(j, i);
            int org_dist = abs(11 - pix.h);
            int blue_dist = abs(216 - pix.h);
            if(org_dist < blue_dist){
                pix.h = 11;
            }else {
                pix.h = 216;
            }
        }
    }
    return image;
}
 

/**
* Returns an immge that has been watermarked by another image.
*
* The luminance of every pixel of the second image is checked, if that
* pixel's luminance is 1 (100%), then the pixel at the same location on
* the first image has its luminance increased by 0.2.
*
* @param firstImage  The first of the two PNGs, which is the base image.
* @param secondImage The second of the two PNGs, which acts as the stencil.
*
* @return The watermarked image.
*/
PNG watermark(PNG firstImage, PNG secondImage) {
    unsigned int ht = secondImage.height();
    unsigned int wd = secondImage.width();
    for(unsigned int i = 0; i < ht; i++){
        for(unsigned int j = 0; j < wd; j++){
            HSLAPixel& pix1 = firstImage.getPixel(j, i);
            HSLAPixel& pix2 = secondImage.getPixel(j, i);
            if(pix2.l == 1.0){
                pix1.l += 0.2;
                if(pix1.l > 1.0){
                        pix1.l = 1.0;
                }
            }
        }
    }

    return firstImage;
}
