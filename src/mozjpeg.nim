import os

{.compile: "mozjpeg/api.c",passc:"-I" & currentSourcePath.parentDir() / "mozjpeg" / "mozjpeg-3.3.1",passc:"-DJPEG_LIB_VERSION=62 -DBITS_IN_JSAMPLE=8".}

proc optimizeJPG*(input_data:string,size:culong,quality:int,fast_encoding:bool): cstring {.importc.}