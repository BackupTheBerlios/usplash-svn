#include "pcx-loader.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>

ImageInfo pcx_info( char *filename ) {
	ImageInfo info;
	PCX_Header *h;
	int fp = open( filename , O_RDONLY );
	h = (PCX_Header *)mmap( 0 , 128 ,  PROT_READ , MAP_PRIVATE , fp , 0 );
	info.w = h->xmax - h->xmin + 1;
	info.h = h->ymax - h->ymin + 1;
	info.bpp = h->bits_per_pixel * h->planes;
	munmap( (void*)h , 128 );
	close( fp );
	return info;
}

Image *pcx_loader( char *filename ) {
	PCX_Header *header;
	char *file;	//Used to store the file
	char *pos;
	char *row;
	char *red,*green,*blue;
	char *imgpos;
	int filesize,i;
	int y, totalbytes;
	Image *img;

	// Load entire file
	FILE *fp = fopen( filename , "rb" );
	if ( fp ) {
		fseek( fp , 0 , SEEK_END );
		filesize = ftell( fp );
		rewind( fp );
		file =(char*) malloc( filesize );
		fread( file , 1 , filesize , fp );
		fclose( fp );
	} else {
		fprintf( stderr , "Failed to load file\n" );
		fclose( fp );
		return 0;
	}

	header = ( PCX_Header *)file;
	pos = file + sizeof( PCX_Header );

	img = (Image*)malloc( sizeof( Image ) );
	img->w = header->xmax - header->xmin + 1;
	img->h = header->ymax - header->ymin + 1;
	img->bpp = 24;
	img->data = (char*)malloc( img->w * img->h * ( img->bpp / 8 ) );
	imgpos = img->data;

	totalbytes = header->planes * header->bytes_per_line;
	row = (char*)malloc( totalbytes );

	for( y=0; y<img->h; y++ ) {
		char *rowpos = row;
		while( rowpos - row != totalbytes ) {
			if( ( 0xC0 & *pos ) == 0xC0 ) {
				unsigned int length = 0x3F & *pos;
				pos++;
				memset( rowpos , *pos , length );
				rowpos += length;
				pos++;
			} else {
				memset( rowpos , *pos , 1 );
				rowpos++;
				pos++;
			};
		}
		blue = row;
		green = row+img->w;
		red = row+img->w*2;
		for( i=0; i<img->w ; i++ ) {
			memcpy( imgpos , red , 1 );
			imgpos++;
			memcpy( imgpos , green , 1 );
			imgpos++;
			memcpy( imgpos , blue , 1 );
			imgpos++;
			red++;
			green++;
			blue++;
		}
	}

	free( file );
	free( row );
	return img;
}
