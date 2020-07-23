import os

when defined(macosx):
  const mozjpeg = "/usr/local/opt/mozjpeg/lib/libjpeg.dylib"
elif defined(linux) or defined(bsd):
  const mozjpeg = "mozjpeg.so"
else:
  const mozjpeg = "mozjpeg.dll"

# 
{.compile: "mozjpeg/api.c",passc:"-I" & currentSourcePath.parentDir() / "mozjpeg" / "mozjpeg-3.3.1",passL:mozjpeg,passc:"-DJPEG_LIB_VERSION=62 -DBITS_IN_JSAMPLE=8".}

proc optimizeJPG*(input_data: string,size:culong,quality:cint,fast_encoding:bool,output: cstring) {.importc:"optimizeJPG",header:"mozjpeg/api.h".}

when isMainModule:
  var c = readFile("th.jpeg")
  var o:cstring
  echo c.len.culong
  optimizeJPG(c,c.len.culong,77.cint,true,o)
  echo o
  writeFile("th_out.jpeg", $o)