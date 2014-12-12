#ifndef TGA_H
#define TGA_H

//#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>


/* OpenGL texture info */
struct gl_texture_t
{
  GLsizei width;
  GLsizei height;

  GLenum format;
  GLint	internalFormat;
  GLuint id;

  GLubyte *texels;
};

#pragma pack(push, 1)
/* TGA header */
struct tga_header_t
{
  GLubyte id_lenght;          /* size of image id */
  GLubyte colormap_type;      /* 1 is has a colormap */
  GLubyte image_type;         /* compression type */

  short	cm_first_entry;       /* colormap origin */
  short	cm_length;            /* colormap length */
  GLubyte cm_size;            /* colormap size */

  short	x_origin;             /* bottom left x coord origin */
  short	y_origin;             /* bottom left y coord origin */

  short	width;                /* picture width (in pixels) */
  short	height;               /* picture height (in pixels) */

  GLubyte pixel_depth;        /* bits per pixel: 8, 16, 24 or 32 */
  GLubyte image_descriptor;   /* 24 bits = 0x00; 32 bits = 0x80 */
};
#pragma pack(pop)

void GetTextureInfo (const struct tga_header_t *header, struct gl_texture_t *texinfo);

void ReadTGA8bits (FILE *fp, const GLubyte *colormap, struct gl_texture_t *texinfo);

void ReadTGA16bits (FILE *fp, struct gl_texture_t *texinfo);

void ReadTGA24bits (FILE *fp, struct gl_texture_t *texinfo);

void ReadTGA32bits (FILE *fp, struct gl_texture_t *texinfo);

void ReadTGAgray8bits (FILE *fp, struct gl_texture_t *texinfo);

void ReadTGAgray16bits (FILE *fp, struct gl_texture_t *texinfo);

void ReadTGA8bitsRLE (FILE *fp, const GLubyte *colormap, struct gl_texture_t *texinfo);

void ReadTGA16bitsRLE (FILE *fp, struct gl_texture_t *texinfo);

void ReadTGA24bitsRLE (FILE *fp, struct gl_texture_t *texinfo);

void ReadTGA32bitsRLE (FILE *fp, struct gl_texture_t *texinfo);

void ReadTGAgray8bitsRLE (FILE *fp, struct gl_texture_t *texinfo);

void ReadTGAgray16bitsRLE (FILE *fp, struct gl_texture_t *texinfo);

struct gl_texture_t * ReadTGAFile (const char *filename);

GLuint loadTGATexture (const char *filename);

#endif // TGA_H
