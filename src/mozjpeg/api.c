

#include "api.h"
#include <stdio.h>

typedef struct _error_mgr {
  struct jpeg_error_mgr pub;
  jmp_buf jb;
} error_mgr;


void optimizeJPG(const unsigned char* input_data,unsigned long input_data_size,int quality,int fast_encoding,unsigned char* outbuffer)
{
//   const unsigned char *input_data;
//   unsigned long input_data_size;

  struct jpeg_compress_struct cinfo;
  struct jpeg_decompress_struct dinfo;
  error_mgr jerr;
  // unsigned char *outbuffer = NULL;
  unsigned long outsize = 0;
  JSAMPARRAY buffer;
  int row_stride;
  dinfo.global_state = cinfo.global_state = 0;
  dinfo.err = cinfo.err = jpeg_std_error(&jerr.pub);

  if(setjmp(jerr.jb)) {
    if (dinfo.global_state != 0)
      jpeg_destroy_decompress(&dinfo);
    if (cinfo.global_state != 0)
      jpeg_destroy_compress(&cinfo);
    return ;
  }

  jpeg_create_decompress(&dinfo);
  jpeg_mem_src(&dinfo, input_data, input_data_size);
  fprintf( stdout,"%d\n", input_data_size);
  fprintf( stdout,"%s\n", input_data);

  jpeg_save_markers(&dinfo, JPEG_COM, 0xFFFF);
  
  for (int m = 0; m < 16; m++)
    jpeg_save_markers(&dinfo, JPEG_APP0 + m, 0xFFFF);

  jpeg_read_header(&dinfo, TRUE);

  dinfo.raw_data_out = FALSE;
  jpeg_start_decompress(&dinfo);

  jpeg_create_compress(&cinfo);

  cinfo.in_color_space = dinfo.out_color_space;
  cinfo.input_components = dinfo.output_components;
  cinfo.data_precision = dinfo.data_precision;
  cinfo.image_width = dinfo.image_width;
  cinfo.image_height = dinfo.image_height;
  fprintf( stdout,"%d\n", dinfo.image_width);
  cinfo.raw_data_in = FALSE;

  // jpeg_set_defaults(&cinfo);

  if (fast_encoding) {
      jpeg_c_set_int_param(&cinfo, JINT_COMPRESS_PROFILE, JCP_FASTEST);
      jpeg_set_defaults(&cinfo);
  } else {
      jpeg_c_set_int_param(&cinfo, JINT_COMPRESS_PROFILE, JCP_MAX_COMPRESSION);
  }
  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, quality, TRUE);
  
  jpeg_mem_dest(&cinfo, &outbuffer, &outsize);
  jpeg_start_compress(&cinfo, TRUE);
  fprintf( stdout,"%d\n", outsize);
  row_stride = dinfo.image_width * dinfo.output_components;
  buffer = (*dinfo.mem->alloc_sarray)((j_common_ptr) &dinfo, JPOOL_IMAGE, row_stride, 1);
  while(dinfo.output_scanline < dinfo.output_height) {
    JDIMENSION num_scanlines = jpeg_read_scanlines(&dinfo, buffer, 1);
    jpeg_write_scanlines(&cinfo, buffer, num_scanlines);
  }
  fprintf( stdout,"%s\n", outbuffer);
  fprintf( stdout,"%d\n", dinfo.output_height);
  fprintf( stdout,"%d\n", outsize);
  jpeg_finish_decompress(&dinfo);
  jpeg_destroy_decompress(&dinfo);

  jpeg_finish_compress(&cinfo);
  jpeg_destroy_compress(&cinfo);

}
