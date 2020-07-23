import os
import sequtils, strutils
when defined(macosx):
  const mozjpeg = "/usr/local/opt/mozjpeg/lib/libjpeg.8.dylib"
elif defined(linux):
  const mozjpeg = "libmozjpeg.so"
else:
  const mozjpeg = "libmozjpeg.dll"

{.compile: "mozjpeg/api.c",passc:"-DJPEG_LIB_VERSION=80 -DBITS_IN_JSAMPLE=8",passc:"-I" & currentSourcePath.parentDir() / "mozjpeg" / "mozjpeg-3.3.1",passL:mozjpeg.}

proc optimizeJPG*(infile: cstring,quality:cint,fast_encoding:bool,outfile: cstring) {.importc:"optimizeJPG",header:"mozjpeg/api.h".}

proc optimizeJPG*(infile: string,quality:int,fast_encoding:bool,outfile: string) = 
  ## It is recommended to keep the quality setting between 60-80 for optimum results.
  optimizeJPG( absolutePath(infile), quality.cint,fast_encoding, outfile.cstring)

when isMainModule:
  
  let p = absolutePath "OIP.jpeg"
  let output = absolutePath "OIP_out.jpeg"
  optimizeJPG(p,75,false,output)