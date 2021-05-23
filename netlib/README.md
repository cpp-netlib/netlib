# netlib Directory

This is the main source directory for the project. The intent is to keep this
directory flat with subdirectories for logical groupings. This means all the
headers, implementation, and test code should be co-hosted in this directory.

## Structure

All implementation files must end with the `.cc` filename extension, and all
headers must end in `.h`. If we have a file named `connection.cc` the header
must be `connection.h` and the test(s) should be in `connection_test.cc`.

We shall control the installed headers through our CMake configuration
instead of assuming that all headers are publicly accessible. When including
files, we should always include headers in the netlib repository by relative
inclusion with the `netlib/` directory (based off the root of the
repository). As an example:

```c++
// In connection.cc and connection_test.cc.
#include "netlib/connection.h"
```

## Subdirectories

We can introduce subdirectories for logical grouping, each one following the
same structure rules as described here. For instance, if we have a
subdirectory of encoding/decoding, we can introduce a `coding/` subdirectory
with all the encoders and decoders implemented.