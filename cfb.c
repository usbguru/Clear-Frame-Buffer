#include  <sys/types.h>
#include  <sys/stat.h>
#include  <sys/mman.h>
#include  <sys/ioctl.h>
#include  <fcntl.h>
#include  <linux/fb.h>
#include  <unistd.h>
#include  <stdio.h>
 
int  main (int argc, char  *argv[])  { 
   int  row, col, width, height, bitspp, bytespp; 
   unsigned int  *data ;
 
   int  fd  =  open ( "/dev/fb0" ,  O_RDWR );
   if (fd<0) {
      printf("Error opening buffer.\n");
      return 1;
      }
 
   struct  fb_var_screeninfo  screeninfo ; 
   ioctl ( fd, FBIOGET_VSCREENINFO ,  & screeninfo );
   screeninfo.activate |= FB_ACTIVATE_NOW | FB_ACTIVATE_FORCE;
   ioctl (fd, FBIOPUT_VSCREENINFO, &screeninfo);
 
   bitspp  =  screeninfo . bits_per_pixel ; 
   if ( bitspp!=  32 )  {  
      printf( "color resolution =%d bits per pixel, expected 32\n" ,  bitspp ); 
      close ( fd ); 
      return  1 ;   
      }
   width  =screeninfo.xres; 
   height =screeninfo.yres; 
   bytespp = bitspp / 8 ;  // calculate bytes per pixel
  
   if ( sizeof ( unsigned  int ) != bytespp )  { 
      close ( fd ); 
      printf("Error.  Pixels are not 32-bit wide.\n");
      return  1 ; 
      }
      // get pointer to frame buffer memory
   data  =  ( unsigned  int * )  mmap( 0 ,width*height*bytespp ,PROT_READ  |  
		   PROT_WRITE ,  MAP_SHARED ,  fd ,  0 );
   printf("Writing to buffer.\n");
   // the color 0x000000FF (blue) because a pixel has the AARRGGBB format 
   for ( row=0;row<height;row++) 
      for (col=0;col<width;col++) 
         data [row*width+col]=0xFF;
   munmap (data,width*height*bytespp);
   close (fd); 
   return  0 ; 
   }

