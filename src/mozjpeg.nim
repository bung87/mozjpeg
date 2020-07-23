import os

when defined(macosx):
  const mozjpeg = "/usr/local/opt/mozjpeg/lib/libjpeg.dylib"
elif defined(linux) or defined(bsd):
  const mozjpeg = "mozjpeg.so"
else:
  const mozjpeg = "mozjpeg.dll"

# 
{.compile: "mozjpeg/api.c",passc:"-DJPEG_LIB_VERSION=80 -DBITS_IN_JSAMPLE=8",passc:"-I" & currentSourcePath.parentDir() / "mozjpeg" / "mozjpeg-3.3.1",passL:mozjpeg.}

proc optimizeJPG*(input_data: cstring,size:culong,quality:cint,fast_encoding:bool,output: cstring) {.importc:"optimizeJPG",header:"mozjpeg/api.h".}

when isMainModule:
  var c = readFile("th.jpeg")
  var o:cstring
  echo c.len.culong
  optimizeJPG(c.cstring,c.len.culong,100.cint,false,o)
  echo $o
  writeFile("th_out.jpeg", $o)