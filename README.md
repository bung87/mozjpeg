# mozjpeg  

wraps mozjpeg c api in Nim  

## prerequires  

`brew install mozjpeg`

## Basic API  

It is recommended to keep the quality setting between 60-80 for optimum results.  

``` Nim
proc optimizeJPG*(infile: string,quality:int,fast_encoding:bool,outfile: string)
```

## References  

[Using mozjpeg to Create Efficient JPEGs](https://hacks.mozilla.org/2014/08/using-mozjpeg-to-create-efficient-jpegs/)

[mozjpeg](https://github.com/mozilla/mozjpeg)