
// #include "jversion.h"           /* for version message */
// #include "jmorecfg.h"
#include "cdjpeg.h"
// #include "jpeglib.h"             /* Common decls for cjpeg/djpeg applications */
// extern JSAMPLE *buffer;
unsigned char* optimizeJPG(unsigned char* input_data,unsigned long input_data_size,int quality,boolean fast_encoding)
{
//   const unsigned char *input_data;
//   unsigned long input_data_size;

  struct jpeg_compress_struct cinfo;
  struct jpeg_decompress_struct dinfo;
//   error_mgr jerr;
  unsigned char *outbuffer = NULL;
  unsigned long outsize = 0;
  JSAMPARRAY buffer;
  int row_stride;

  jpeg_create_decompress(&dinfo);
  jpeg_mem_src(&dinfo, input_data, input_data_size);

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

  cinfo.raw_data_in = FALSE;

  jpeg_set_defaults(&cinfo);

  if (fast_encoding) {
      jpeg_c_set_int_param(&cinfo, JINT_COMPRESS_PROFILE, JCP_FASTEST);
      jpeg_set_defaults(&cinfo);
  } else {
      jpeg_c_set_int_param(&cinfo, JINT_COMPRESS_PROFILE, JCP_MAX_COMPRESSION);
  }

  jpeg_set_quality(&cinfo, quality, TRUE);
  
  jpeg_mem_dest(&cinfo, &outbuffer, &outsize);
  jpeg_start_compress(&cinfo, TRUE);

  row_stride = dinfo.image_width * dinfo.output_components;
  buffer = (*dinfo.mem->alloc_sarray)((j_common_ptr) &dinfo, JPOOL_IMAGE, row_stride, 1);
  while(dinfo.output_scanline < dinfo.output_height) {
    JDIMENSION num_scanlines = jpeg_read_scanlines(&dinfo, buffer, 1);
    jpeg_write_scanlines(&cinfo, buffer, num_scanlines);
  }

  jpeg_finish_decompress(&dinfo);
  jpeg_destroy_decompress(&dinfo);

  jpeg_finish_compress(&cinfo);
  jpeg_destroy_compress(&cinfo);

  return outbuffer;
}
