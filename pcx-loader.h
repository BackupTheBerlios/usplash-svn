#ifndef __pcx_loader_h
#define __pcx_loader_h

typedef struct {
	char signature;
	char version;
	char encoding;
	char bits_per_pixel;
	unsigned short int xmin;
	unsigned short int ymin;
	unsigned short int xmax;
	unsigned short int ymax;
	unsigned short int vres;
	unsigned short int hres;
	char palette[48];
	char reserved;
	char planes;
	unsigned short int bytes_per_line;
	unsigned short int palette_type;
	char unused[58];
} PCX_Header;

typedef struct {
	int w,h,bpp;
	char *data;
} Image;

typedef struct {
	int w,h,bpp;
} ImageInfo;

Image *pcx_loader( char *filename );
ImageInfo pcx_info( char *filename );

#endif
