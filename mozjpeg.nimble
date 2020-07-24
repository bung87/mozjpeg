# Package

version       = "0.1.0"
author        = "bung87"
description   = "A new awesome nimble package"
license       = "MIT"
srcDir        = "src"
installDirs   = @["mozjpeg/mozjpeg-3.3.1"]
installExt = @["nim","c","h"]

# Dependencies

requires "nim >= 1.3.5"
