#include <stdio.h>
#include <sys/ioctl.h>
#include <linux/vt.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <dirent.h>

#include "pcx-loader.h"

int open_tty() {
	int fp;
	fp = open("/dev/tty0" , O_RDWR );
	if ( fp >= 0 ) return fp;
	fp = open("/devfs/vc/0" , O_RDWR );
	if ( fp >= 0 ) return fp;
	return -1;
}

int open_fb() {
	int fp;
	fp = open("/dev/fb0" , O_RDWR );
	if ( fp >= 0 ) return fp;
	fp = open("/devfs/fb/0" , O_RDWR );
	if ( fp >= 0 ) return fp;
	return -1;
}

void open_vt() {
	/* Open a New VT */
	long res;
	int fp = open_tty();
	if ( fp < 0 ) {
		fprintf( stderr , "Can't open tty\n" );
		exit( 1 );
	}
	ioctl( fp , VT_OPENQRY ,  &res );
	ioctl( fp , VT_ACTIVATE , res );
	ioctl( fp , VT_WAITACTIVE , res );
	close( fp );

	fp = open_fb();
	if ( fp < 0 ) {
		fprintf( stderr , "Can't open fb\n" );
		exit( 1 );
	}
	/* Makes the cursor invisible */
	/* TODO: Fix this a better way */
	ioctl( fp , FBIOBLANK );
	close( fp );
}

struct fb_var_screeninfo *get_vscreeninfo() {
	int fp;
	struct fb_var_screeninfo *fbvinf = (struct fb_var_screeninfo*)malloc( sizeof( struct fb_var_screeninfo ) );
	
	fp = open_fb();
	if ( fp < 0 ) {
		fprintf( stderr , "Can't open fb\n" );
		exit( 1 );
	}
	ioctl( fp , FBIOGET_VSCREENINFO , fbvinf );
	close( fp );
	return fbvinf;
}

char *get_best_filename( char *dir ) {
	DIR *d;
	struct dirent *de;
	struct fb_var_screeninfo *vinf = get_vscreeninfo();
	char *bestname,*fn;
	ImageInfo bestinfo;
	bestinfo.w = 0;
	bestinfo.h = 0;
	bestinfo.bpp = 0;
	bestname=0;
	d = opendir( dir );
	for ( de = readdir( d ); de != NULL ; de = readdir( d ) ) {
		if ( strcmp( de->d_name , "." ) == 0 || strcmp( de->d_name , ".." ) == 0 ) continue;
		if ( memcmp( de->d_name+(strlen(de->d_name) - 4 ) ,".pcx" , 4 ) == 0 ) {
			fn = (char*)malloc( strlen( de->d_name ) + strlen(dir) + 2 );
			memset( fn , '\0' , strlen( de->d_name ) + strlen(dir) + 2  );
			fn = strcat( fn , dir );
			fn = strcat( fn , "/" );
			fn = strcat( fn , de->d_name );
			ImageInfo info = pcx_info( fn );
			if( ( vinf->xres >= info.w ) && ( vinf->yres >= info.h ) && ( info.h >= bestinfo.h ) && ( info.w >= bestinfo.w ) ) {
				if ( ( vinf->bits_per_pixel == 8 && info.bpp == 8 ) || ( vinf->bits_per_pixel > 8 && info.bpp > 8 ) ) {
					if( bestname ) free( bestname );
					bestname = (char*)malloc( strlen( fn ) + 1 );
					memset( bestname , '\0' , strlen( fn ) + 1 );
					bestname = strcat( bestname , fn );
					bestinfo = info;
				}
			}
			free( fn );
		}
	}
	printf("Using image: %s\n" , bestname );
	closedir( d );
	return bestname;
}


void convert( Image *in ) {
	struct fb_var_screeninfo *vinf = get_vscreeninfo();
	char *data;
	char *dst;
	char *src;
	char *r,*g,*b;

	switch( vinf->bits_per_pixel ) {
		case 16:
			data = (char*) malloc( in->w * in->h * 2 );
			dst = data;
			for( src=in->data ; src < in->data+(in->w*in->h*3 ) ; src+=3 ) {
				b = src;
				g = src+1;
				r = src+2;
				*dst = ( 0xE0 & ( *g << 3 ) ) | ( 0x1F & ( *b >> 3 ) );
				dst++;
				*dst = ( 0xF8 & *r ) | ( 0x07 & ( *g >> 5 ) );
				dst++;
			}
			free( in->data );
			in->data = data;
			in->bpp = 16;
			break;
		case 24:
			break;
		case 32:
			data = (char*) malloc( in->w * in->h * 4 );
			dst = data;
			for( src=in->data ; src < in->data+(in->w*in->h*3 ) ; src+=3 ) {
				memcpy( dst , src , 3 );
				memset( dst+3 , 0xff , 1 );
				dst+=4;
			}
			free( in->data );
			in->data = data;
			in->bpp = 32;
			break;
	}
}

int main( int argc , char *argv[] ) {
	if ( argc != 2 ) {
		printf("Usage: usplat <directory>\n");
		exit(1);
	}
	int fd;
	Image *img;
	char *fn;
	fn = get_best_filename( argv[1] );
	open_vt();
	img = pcx_loader( fn );
	convert( img );

	fd = open_fb();
	write( fd , img->data , img->w*img->h*(img->bpp/8) );
	close( fd );

	return 0;
}
