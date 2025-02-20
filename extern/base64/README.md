# base64

Simple, open source, header-only base64 encoder

## Usage
```cpp
#include <string>
#include <iostream>
#include <iterator>

#include "base64.hpp"

std::string encoded = base64::encode("hello, world");

std::cout << "encoded: " << encoded << std::endl
  << "decoded: " << base64::decode(encoded);
  
// inplace decoding
base64::decode_inplace(encoded);

// or with iterators
base64::decode(encoded.begin(), encoded.end(), std::ostream_iterator<char>(std::cout));

```

## Documentation

The documenation of the simple API can be found [here](https://terrakuh.github.io/base64/classbase64.html).

## License
The header file is put into the public domain, so everyone is allowed to do whatever they want.
