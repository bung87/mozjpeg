
#include "api.h"


typedef struct _error_mgr {
  struct jpeg_error_mgr pub;
  jmp_buf jb;
} error_mgr;


void optimizeJPG(const  char* filename,int quality,int fast_encoding,const char* outfilePath)
{

  struct jpeg_compress_struct cinfo;
  struct jpeg_decompress_struct dinfo;
  error_mgr jerr;

  FILE *infile;   
  FILE *outfile;   
  JSAMPARRAY buffer;
  int row_stride;
  if ((infile = fopen(filename, "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
    return ;
  }
  if ((outfile = fopen(outfilePath, "wb")) == NULL) {
    fprintf(stderr, "can't open %s\n", outfilePath);
    exit(1);
  }
  dinfo.global_state = cinfo.global_state = 0;
  dinfo.err = cinfo.err = jpeg_std_error(&jerr.pub);

  if(setjmp(jerr.jb)) {
    if (dinfo.global_state != 0)
      jpeg_destroy_decompress(&dinfo);
    if (cinfo.global_state != 0)
      jpeg_destroy_compress(&cinfo);
    fclose(infile);
    return ;
  }

  jpeg_create_decompress(&dinfo);

  jpeg_stdio_src(&dinfo, infile);
  
  // jpeg_save_markers(&dinfo, JPEG_COM, 0xFFFF);
  
  // for (int m = 0; m < 16; m++)
  //   jpeg_save_markers(&dinfo, JPEG_APP0 + m, 0xFFFF);

  jpeg_read_header(&dinfo, TRUE);
  
  dinfo.raw_data_out = FALSE;
  jpeg_start_decompress(&dinfo);

 /* JSAMPLEs per row in output buffer */
  row_stride = dinfo.output_width * dinfo.output_components;
  /* Make a one-row-high sample array that will go away when done with image */
  buffer = (*dinfo.mem->alloc_sarray)
                ((j_common_ptr) &dinfo, JPOOL_IMAGE, row_stride, 1);


  jpeg_create_compress(&cinfo);
  
  cinfo.in_color_space = dinfo.out_color_space;
  cinfo.input_components = dinfo.output_components;
  cinfo.data_precision = dinfo.data_precision;
  cinfo.image_width = dinfo.image_width;
  cinfo.image_height = dinfo.image_height;
  cinfo.raw_data_in = FALSE;

  jpeg_set_defaults(&cinfo);

  if (fast_encoding) {
      jpeg_c_set_int_param(&cinfo, JINT_COMPRESS_PROFILE, JCP_FASTEST);
      jpeg_set_defaults(&cinfo);
  } else {
      jpeg_c_set_int_param(&cinfo, JINT_COMPRESS_PROFILE, JCP_MAX_COMPRESSION);
  }
 
  jpeg_set_quality(&cinfo, quality, TRUE);
  jpeg_stdio_dest(&cinfo, outfile);
  jpeg_start_compress(&cinfo, TRUE);

  while (dinfo.output_scanline < dinfo.output_height) {
    JDIMENSION num_scanlines = jpeg_read_scanlines(&dinfo, buffer, 1);
    jpeg_write_scanlines(&cinfo, buffer, num_scanlines);
  }

  jpeg_finish_decompress(&dinfo);
  jpeg_destroy_decompress(&dinfo);

  jpeg_finish_compress(&cinfo);
  jpeg_destroy_compress(&cinfo);

  fclose(infile);
  fclose(outfile);

}
