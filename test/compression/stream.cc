//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Filtering compression stream unittests.
 */

#include <pycpp/compression/stream.h>
#include <gtest/gtest.h>
#include <sstream>

PYCPP_USING_NAMESPACE

// DATA
// ----

static std::string BZ2_COMPRESSED("\x42\x5a\x68\x39\x31\x41\x59\x26\x53\x59\xfd\x9e\x0c\x3e\x00\x01\x20\xff\xf5\x00\x10\x20\x00\x40\x65\xf0\x92\x3f\xef\xdf\xe0\x7f\xf7\xdf\xa0\x40\x00\x00\x06\x00\x20\x10\x00\x10\x00\x40\x02\x88\xa7\x15\xd0\x68\x89\xa2\x30\x93\x4d\x03\xd4\x68\x1a\x7a\x87\xa8\xf5\x0c\x80\xd0\x7a\x80\x00\xd4\xc0\x9a\x23\x48\xd4\xf2\x8f\x51\xa4\xc8\xc4\x00\xc0\x68\x21\xa6\x02\x2a\x7f\xa2\x6a\x86\x08\x68\xd3\x26\x9a\x00\x00\x64\x00\x34\x00\x25\x34\x9a\x10\x53\xc2\x9b\xd2\x20\x40\x01\xa6\x81\xa0\x68\xd0\xc8\x04\xf1\x9e\x04\x2d\x96\x98\xc9\xc1\x11\x0a\x75\xef\x2d\x27\x17\xca\x67\xc4\xd0\x98\xb8\x94\x69\x2c\x3e\x84\xa1\x29\x17\x15\x2a\x80\x9b\x88\xaa\x70\xc9\x98\x8b\x55\x1c\xcd\xa6\x1c\x91\x26\x23\xe2\xc8\xcd\x2a\x4b\xe9\xcd\x57\xb6\x97\x40\xa3\xa7\xa7\x3e\xfd\xfa\x10\x61\xe1\xd9\xb3\xb4\xba\xdd\x9c\x9d\x13\x67\x91\x2c\xd5\xa9\xc4\x80\x8d\x1c\x4c\x84\x1b\x4e\x6f\xc2\x4f\x35\x0b\xe3\x4b\xc3\x9d\xc8\xe0\xc5\x60\x38\xa5\x97\x24\xe6\x19\x7a\x2f\x13\x6f\x74\xa2\x9b\xbb\x8f\x0b\x6b\xcc\x2e\x04\x3e\x65\x1a\x91\x44\xf7\xa8\x5b\x89\xa1\x7b\xa2\x1c\x86\x5b\x64\x6c\xb0\x81\x4e\x05\x08\x79\x10\xa2\x14\xfc\x30\x69\x20\x81\xc5\xc2\x43\x05\x6f\xc1\x90\x11\x30\x5a\xed\xfc\x59\x2b\xd8\xa6\xb2\x06\x7a\xab\x89\xd3\x9f\xbb\x2a\x48\x98\xd9\xa7\x0b\x6d\x5f\xb0\x0e\x4c\x89\x23\x96\x30\x77\xdb\x69\x89\x0d\xd8\x31\x7e\x20\xf5\x99\xe1\x88\xbd\x70\x12\xee\xb5\xb1\x87\xa0\x9d\x94\xf7\x24\x87\xc6\x84\x2d\x76\xb6\x93\x39\x16\xbb\x0a\xb2\x95\x23\x84\x1f\x49\x90\x9a\x69\x27\x5b\xb5\x76\x20\xa7\x05\x71\x52\x13\x56\xb3\x28\xac\xdc\x96\xc0\xaf\x13\x53\x8c\x43\x52\xfc\xe9\x84\xda\x77\xbd\x9f\xb2\x46\x74\x41\xa2\x49\x23\x45\x8d\x48\x86\xc7\xa7\x29\x29\x5a\x44\xee\x1d\xca\xf1\xf3\x44\xa5\x1b\xa9\x57\xc9\x26\x83\xc5\x9c\x9b\xa2\x7b\xa0\xe0\x57\x23\x4c\xd0\x18\x8a\x10\x13\xb8\x84\xda\x27\x01\x26\x13\x5c\x89\x05\x06\x8f\x8c\xf4\xb8\x63\x63\xcd\xa6\x5f\x93\x93\xe2\x26\xd7\xf6\x4c\x3a\xbd\x1d\xbd\x50\xc7\x5c\xdf\xbc\x68\x79\xc0\xfd\xcd\xf5\x5e\xe9\xde\x82\x72\xf8\x0f\x92\x10\x0a\xaa\xa8\x40\x00\x19\x45\x80\xd1\x86\x4e\x37\x3e\x93\xa0\x49\xe3\xda\x25\x23\x62\x1a\x98\xe3\x54\x27\x5a\xee\x89\x51\xfa\x27\x5b\x8f\xbe\x35\x7e\x1d\xe1\xdb\xb1\x79\xb0\x6f\xb0\xe3\x28\xd9\xa6\x3b\x4b\x23\x2d\x39\x93\x24\xd2\x87\x39\x51\xb3\x2a\x5c\xe7\x93\x29\x66\x6c\xed\xdb\x18\x1a\xac\x74\xaf\x3e\x39\x88\x3a\x6a\x6f\x30\xbb\x79\x7a\x30\x1c\xe7\x9b\x62\x6d\x2b\xd7\x3c\x1c\x29\xb0\xfd\x15\xb3\x5e\xf0\x3a\x12\x11\x50\x77\x08\x82\x4d\x73\x4d\xb5\x0c\x53\x71\xee\x52\xd2\xe0\x96\xe6\x2d\x6c\xa1\x06\x89\xd3\xb1\xe6\xef\xbb\x87\x02\xe3\x7e\xf9\x85\x84\x20\xc1\x1c\xbb\x6c\xc2\xd2\xd2\x92\xd8\xfc\x17\xc3\x72\x22\xf5\x80\xb3\x4e\x56\x4d\x35\x48\x4b\x20\x64\x5a\x04\x15\xd4\x74\x69\x40\xd6\xb9\x9b\xd2\xac\xf4\x48\x12\xe9\x20\xc1\x2c\x18\x56\xf6\x79\x4a\x82\xed\x46\x72\xca\xc5\xd1\x4c\x0c\xd5\xfa\x8d\x12\x44\x5f\x95\x5d\xbf\xc1\xfd\x0f\xf5\x5a\xdc\xdf\xf1\x77\x24\x53\x85\x09\x0f\xd9\xe0\xc3\xe0", 710);
static std::string ZLIB_COMPRESSED("\x78\x9c\x6d\x52\x4b\x6e\xdb\x30\x10\xdd\xf3\x14\xb3\x4c\x01\x21\xfd\x6c\x0a\x14\xe8\x82\x96\xe8\x98\xa8\x2c\x0a\x14\x1d\xd7\xbb\xc8\x12\x6d\xb1\x90\x45\x43\xa4\x63\x64\x97\x83\xb4\x07\xe8\x35\x7a\x94\x9c\xa4\x43\xca\x49\xda\xa2\x82\x01\x99\xc3\x99\xf7\x1b\xa9\x4e\xc3\x92\x2b\xc8\x4d\xa3\x07\xa7\xe1\x0a\x0f\x6f\xc8\xe7\xff\x3d\x84\xa4\xf6\xf8\x30\x9a\x7d\xe7\xe1\xd7\x4f\xb8\xfb\xf0\xee\xfd\xc7\x3b\xb8\xab\xcc\x08\x33\xdd\x9a\x7b\x3d\x6a\x52\xea\xf1\x60\x9c\x33\x76\x00\xe3\xa0\xc3\xd2\xf6\x01\xf6\x63\x3d\x78\xdd\x26\xb0\x1b\xb5\x06\xbb\x83\xa6\xab\xc7\xbd\x4e\xc0\x5b\xa8\x87\x07\x38\xea\xd1\xd9\x81\xd8\xad\xaf\xcd\x60\x86\x3d\xd4\xd0\x20\x53\xe8\xf4\x1d\xc2\x38\xbb\xf3\xe7\x7a\xd4\xd8\xdc\x42\xed\x9c\x6d\x4c\x8d\x78\xd0\xda\xe6\x74\xd0\x83\xaf\x3d\xf2\x91\x9d\xe9\xb5\x83\x2b\x8f\x86\x9e\x1e\xbf\x57\x97\x99\xa7\xc7\x1f\x6f\x22\x51\xab\xeb\x1e\xcc\x00\xe1\xfe\xf9\x12\xce\xc6\x77\xf6\xe4\xc9\xa8\x9d\x1f\x4d\x13\x70\x12\x6c\x6a\xfa\x53\x1b\x74\x5c\xae\xa1\x37\x07\x33\xb1\xc4\xf1\x18\x81\x0b\xa0\x27\xa7\x13\x12\xb4\x26\x70\xb0\xad\xd9\x85\xb7\x8e\xd6\x8e\xa7\x6d\x6f\x5c\x97\x40\x6b\x02\xf4\xf6\xe4\xb1\xe8\x42\x31\xe6\x9c\x04\x2f\x6f\xed\x08\x4e\xf7\x7d\x40\x30\xa8\x3d\xfa\x7d\x55\x17\x7b\x02\xcb\x31\x84\xea\x2f\x31\x45\xde\x73\x67\x0f\xa1\x97\xbc\x38\xc1\x98\x76\xa7\x71\x40\x4a\x1d\x67\x5a\x8b\xb1\x45\xc6\x6f\xba\xf1\xa1\x12\xa0\x77\xb6\xef\xed\x19\xad\x21\xe5\xd0\x9a\xe0\xc8\x7d\x22\x44\xe1\x55\xbd\xb5\xf7\x3a\xe6\x3e\x6d\x78\xb0\x1e\xa5\x4e\x12\xc2\x12\x8e\xaf\x9b\xbd\x5c\xb9\xae\xee\x7b\xd8\x6a\x32\x05\x86\xbc\x18\x6f\x28\x3d\xdb\x19\x03\xbd\xf3\xb8\x7c\x83\xd9\x1f\xed\x18\xf9\xfe\xb5\x79\x8d\xfc\x0b\x06\x95\x98\xab\x35\x95\x0c\x78\x05\xa5\x14\xb7\x3c\x63\x59\x58\x24\xad\xb0\x82\x5b\x4c\x60\xcd\xd5\x42\xac\x14\x60\x97\xa4\x85\xda\x80\x98\x03\x2d\x36\xf0\x85\x17\x59\x42\xd8\xd7\x52\xb2\xaa\x02\x21\x81\x2f\xcb\x9c\xb3\x2c\x01\x5e\xa4\xf9\x2a\xe3\xc5\x0d\xcc\x70\xae\x10\xf8\x99\x73\xfc\xbe\x11\x58\x09\x08\xa4\x17\x28\xce\x2a\x82\x60\x4b\x26\xd3\x05\x1e\xe9\x8c\xe7\x5c\x6d\x12\x98\x73\x55\x04\xcc\x39\x82\x52\x28\xa9\x54\x3c\x5d\xe5\x54\x42\xb9\x92\xa5\xa8\x18\xd2\x67\xa4\x10\x05\x2f\xe6\x12\x59\xd8\x92\x15\xea\x1a\x59\x91\x0a\xd8\x2d\x1e\xa0\x5a\xd0\x3c\x8f\x54\x74\x85\xea\x65\xd4\x97\x8a\x72\x23\xf9\xcd\x42\x91\x85\xc8\x33\x86\xc5\x19\x43\x65\x74\x96\xb3\x89\x0a\x4d\xa5\x39\xe5\xcb\x04\x32\xba\xa4\x37\x2c\x4e\x09\x44\x91\xb1\x6d\x52\x47\xd6\x0b\x16\x4b\xc8\x47\xf1\x97\x2a\x2e\x8a\x90\x49\x2a\x0a\x25\xf1\x98\xa0\x4b\xa9\x5e\x46\xd7\xbc\x62\x09\x50\xc9\x2b\x94\x4a\xe6\x52\x20\x7c\x88\x13\x27\x44\x04\xc1\xb9\x82\x4d\x28\x21\x6a\xf8\x6b\x2b\xd8\x12\xce\xab\x2a\xfc\x25\x93\x96\x8c\xd1\x1c\xb1\xaa\x30\xfc\x67\xf3\x35\xf9\x0d\x9b\x11\x64\xfe", 669);
static std::string LZMA_COMPRESSED("\xfd\x37\x7a\x58\x5a\x00\x00\x04\xe6\xd6\xb4\x46\x02\x00\x21\x01\x16\x00\x00\x00\x74\x2f\xe5\xa3\xe0\x04\x55\x02\xb3\x5d\x00\x2a\x1a\x08\xa2\x01\xfb\xd6\x96\xf1\x8a\x93\x97\x90\x87\xd3\xb6\x8a\x4a\x8d\x93\xc0\x85\xc1\xd4\xaa\x09\x8a\x40\x26\xae\xa5\xdf\x7f\xe7\x56\xaa\xd3\xcd\x6e\xd1\xfa\x9e\xbf\x49\x87\xa9\x43\x1c\x17\x37\x2b\xc4\x2b\x5a\xa2\x27\x92\x06\x20\x9f\xb1\x2c\x26\x61\x72\x91\x71\xd2\x9a\x7f\x6f\xf3\xbb\xdb\xea\x8b\xd4\xa1\x34\x69\xaf\x06\x42\x9f\xaa\xa3\x6d\xd4\xef\x75\xd4\x2c\x81\xac\xb2\x2f\x3b\xe7\x9e\xed\x93\xe8\xd0\xa1\x0f\x11\x74\xf2\xf6\x54\xb8\xee\xd8\x84\xae\x36\x21\xca\x10\xe4\xea\xa8\xc7\xe1\x98\xaf\x75\x4d\x4a\x31\x99\xad\x24\x81\xfd\x43\xd3\x0e\x10\x84\xcc\x31\xf0\x70\x44\x83\x6b\xc4\x2d\xa9\xff\x43\x62\x5a\x40\xe9\x21\x1a\x8e\xb1\x29\xb1\xf6\xe7\xe8\x34\x61\xc8\x5c\xce\x95\x1c\x2e\x79\xa8\xcb\x22\xb0\x37\x1f\x9a\xe2\xa2\xb2\x71\x3b\x46\x80\xf4\xc7\x45\xe3\xa4\x37\x55\xc8\x7c\xe5\x0a\x5c\x2b\xcc\x8f\xa4\x85\x0b\xae\x2d\x12\xda\xce\xbc\xd2\xfd\xc6\x0d\x14\xe2\x02\x91\x3c\x14\x4b\x0c\x2f\xbf\x87\x5c\x4a\x19\x17\x24\xb4\x4c\x28\x52\xcc\xed\x9d\x12\x9d\x08\x44\xeb\x17\x61\xc8\x00\x0e\xa7\x89\x9a\x7e\x03\xa2\x08\x7a\xc7\xc4\x02\x44\xe6\xd2\xff\x8a\x96\xf6\x24\x71\xb7\x8b\x73\x37\x43\x29\x69\x67\x50\x01\x29\xd7\xeb\xc6\x09\x56\x68\xf3\x37\x77\xad\x23\x57\xe8\x09\x51\xb4\x02\x41\x93\x3c\x1e\xb3\xbb\x47\x06\x3b\xe9\xf7\x0b\xa1\xff\x90\x86\x35\xd0\x4b\xd7\x24\xb0\x77\x5e\xce\xd4\x6a\x8a\x90\x4d\xa0\x46\x46\x5d\xe3\x27\x58\x36\xc7\xe4\x64\x26\xa4\xb6\x9f\x85\x2f\xe2\xad\x6f\x04\x22\x6a\xcc\xb6\xbf\x26\xd7\xae\x26\x01\x67\xe4\x4c\x6b\x10\xac\x9a\xca\x58\x6d\xb7\x81\x8e\x8e\x78\x8c\xd1\xbc\x18\x37\x48\x3b\x18\xde\x10\xf5\xfd\x34\xf1\x2f\x9d\x35\x33\x2b\x81\x08\xc3\xd3\xdc\xc1\x87\xa5\xdd\x0b\xf4\x63\xeb\xcc\x0f\xb1\xf1\xcc\x86\xfc\xa7\x14\x32\x45\x4c\x79\x57\x93\x54\x65\xac\xac\x08\xbb\xbb\xe9\xb6\x3a\xcf\xa3\x6f\x69\x85\x0a\x21\x09\x89\xde\x02\x14\xe0\x21\x4f\x37\x7f\x72\x2d\x57\xb6\x4a\x8b\x3b\x0b\x02\xf3\x4a\x16\x16\x33\x46\x6d\x29\xb8\x31\xfb\x26\x07\xfe\x75\xb3\xb1\xe4\x56\x6e\x7f\xbf\x75\xc1\xb0\x38\x1f\x26\x7a\x65\x6f\x08\xbe\x1d\x53\xab\x04\x18\x07\xd0\x84\x3e\xef\xf9\xf9\x4d\x28\x62\x62\xeb\xe4\x42\x18\xa1\x6c\x16\x75\x4d\x5b\xf8\x51\xba\x77\xb6\x5a\xe6\xf2\x14\x7e\x24\x3b\xe5\x49\x80\x23\x69\x0e\x97\xf5\x16\x89\x7c\xac\x6f\xa5\x33\xd6\x70\x47\xd2\xda\x48\xd9\xa9\x41\xe2\xe4\x9b\xf1\x7e\xb4\xca\xf1\x0d\x5f\x5e\x78\xb2\xfb\x62\x47\xe8\x53\x96\xa1\xa9\xb1\x68\x70\x82\x39\x6c\x55\x53\x0c\xc3\xc9\x09\x91\x90\x6c\x85\xc0\x5a\xd7\x45\xf0\x6d\x0e\x68\x6c\x53\x74\xf0\xf9\x0e\x31\x52\x9d\x0f\x2b\x7f\x3b\xc6\x29\x53\xb0\x22\x48\xde\x30\x50\x18\x5f\x23\x33\x94\x30\xc7\x06\x66\xf7\x95\x29\x2f\x81\x84\x6f\x1d\x35\xbb\x98\xb5\x0a\xcf\x1a\x3a\x55\xe4\xb8\xdf\xa2\xa3\x8f\xa9\x83\xe7\xb3\x70\x54\x99\x74\x39\x0a\x93\x69\x75\xfe\x83\x55\xe9\xc2\x7c\x80\x69\xf7\xb6\x12\xcb\xcf\x75\xef\x6b\xa8\x1f\x69\x9a\x67\xde\x09\x17\xed\x1f\xec\x58\x00\x00\xff\x9a\x61\x8b\x57\xe4\x80\xdb\x00\x01\xcf\x05\xd6\x08\x00\x00\x31\x40\x27\xe6\xb1\xc4\x67\xfb\x02\x00\x00\x00\x00\x04\x59\x5a", 756);
static std::string GZIP_COMPRESSED("\x1f\x8b\x08\x00\x00\x00\x00\x00\x02\xff\x6d\x52\x4b\x6e\xdb\x30\x10\xdd\xf3\x14\xb3\x4c\x01\x21\xfd\x6c\x0a\x14\xe8\x82\x96\xe8\x98\xa8\x2c\x0a\x14\x1d\xd7\xbb\xc8\x12\x6d\xb1\x90\x45\x43\xa4\x63\x64\x97\x83\xb4\x07\xe8\x35\x7a\x94\x9c\xa4\x43\xca\x49\xda\xa2\x82\x01\x99\xc3\x99\xf7\x1b\xa9\x4e\xc3\x92\x2b\xc8\x4d\xa3\x07\xa7\xe1\x0a\x0f\x6f\xc8\xe7\xff\x3d\x84\xa4\xf6\xf8\x30\x9a\x7d\xe7\xe1\xd7\x4f\xb8\xfb\xf0\xee\xfd\xc7\x3b\xb8\xab\xcc\x08\x33\xdd\x9a\x7b\x3d\x6a\x52\xea\xf1\x60\x9c\x33\x76\x00\xe3\xa0\xc3\xd2\xf6\x01\xf6\x63\x3d\x78\xdd\x26\xb0\x1b\xb5\x06\xbb\x83\xa6\xab\xc7\xbd\x4e\xc0\x5b\xa8\x87\x07\x38\xea\xd1\xd9\x81\xd8\xad\xaf\xcd\x60\x86\x3d\xd4\xd0\x20\x53\xe8\xf4\x1d\xc2\x38\xbb\xf3\xe7\x7a\xd4\xd8\xdc\x42\xed\x9c\x6d\x4c\x8d\x78\xd0\xda\xe6\x74\xd0\x83\xaf\x3d\xf2\x91\x9d\xe9\xb5\x83\x2b\x8f\x86\x9e\x1e\xbf\x57\x97\x99\xa7\xc7\x1f\x6f\x22\x51\xab\xeb\x1e\xcc\x00\xe1\xfe\xf9\x12\xce\xc6\x77\xf6\xe4\xc9\xa8\x9d\x1f\x4d\x13\x70\x12\x6c\x6a\xfa\x53\x1b\x74\x5c\xae\xa1\x37\x07\x33\xb1\xc4\xf1\x18\x81\x0b\xa0\x27\xa7\x13\x12\xb4\x26\x70\xb0\xad\xd9\x85\xb7\x8e\xd6\x8e\xa7\x6d\x6f\x5c\x97\x40\x6b\x02\xf4\xf6\xe4\xb1\xe8\x42\x31\xe6\x9c\x04\x2f\x6f\xed\x08\x4e\xf7\x7d\x40\x30\xa8\x3d\xfa\x7d\x55\x17\x7b\x02\xcb\x31\x84\xea\x2f\x31\x45\xde\x73\x67\x0f\xa1\x97\xbc\x38\xc1\x98\x76\xa7\x71\x40\x4a\x1d\x67\x5a\x8b\xb1\x45\xc6\x6f\xba\xf1\xa1\x12\xa0\x77\xb6\xef\xed\x19\xad\x21\xe5\xd0\x9a\xe0\xc8\x7d\x22\x44\xe1\x55\xbd\xb5\xf7\x3a\xe6\x3e\x6d\x78\xb0\x1e\xa5\x4e\x12\xc2\x12\x8e\xaf\x9b\xbd\x5c\xb9\xae\xee\x7b\xd8\x6a\x32\x05\x86\xbc\x18\x6f\x28\x3d\xdb\x19\x03\xbd\xf3\xb8\x7c\x83\xd9\x1f\xed\x18\xf9\xfe\xb5\x79\x8d\xfc\x0b\x06\x95\x98\xab\x35\x95\x0c\x78\x05\xa5\x14\xb7\x3c\x63\x59\x58\x24\xad\xb0\x82\x5b\x4c\x60\xcd\xd5\x42\xac\x14\x60\x97\xa4\x85\xda\x80\x98\x03\x2d\x36\xf0\x85\x17\x59\x42\xd8\xd7\x52\xb2\xaa\x02\x21\x81\x2f\xcb\x9c\xb3\x2c\x01\x5e\xa4\xf9\x2a\xe3\xc5\x0d\xcc\x70\xae\x10\xf8\x99\x73\xfc\xbe\x11\x58\x09\x08\xa4\x17\x28\xce\x2a\x82\x60\x4b\x26\xd3\x05\x1e\xe9\x8c\xe7\x5c\x6d\x12\x98\x73\x55\x04\xcc\x39\x82\x52\x28\xa9\x54\x3c\x5d\xe5\x54\x42\xb9\x92\xa5\xa8\x18\xd2\x67\xa4\x10\x05\x2f\xe6\x12\x59\xd8\x92\x15\xea\x1a\x59\x91\x0a\xd8\x2d\x1e\xa0\x5a\xd0\x3c\x8f\x54\x74\x85\xea\x65\xd4\x97\x8a\x72\x23\xf9\xcd\x42\x91\x85\xc8\x33\x86\xc5\x19\x43\x65\x74\x96\xb3\x89\x0a\x4d\xa5\x39\xe5\xcb\x04\x32\xba\xa4\x37\x2c\x4e\x09\x44\x91\xb1\x6d\x52\x47\xd6\x0b\x16\x4b\xc8\x47\xf1\x97\x2a\x2e\x8a\x90\x49\x2a\x0a\x25\xf1\x98\xa0\x4b\xa9\x5e\x46\xd7\xbc\x62\x09\x50\xc9\x2b\x94\x4a\xe6\x52\x20\x7c\x88\x13\x27\x44\x04\xc1\xb9\x82\x4d\x28\x21\x6a\xf8\x6b\x2b\xd8\x12\xce\xab\x2a\xfc\x25\x93\x96\x8c\xd1\x1c\xb1\xaa\x30\xfc\x67\xf3\x35\xf9\x0d\x8a\x8f\x34\x24\x56\x04\x00\x00", 689);
static std::string DECOMPRESSED("\x54\x68\x65\x20\x4d\x49\x54\x20\x4c\x69\x63\x65\x6e\x73\x65\x20\x28\x4d\x49\x54\x29\x0a\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x0a\x0a\x43\x6f\x70\x79\x72\x69\x67\x68\x74\x20\xc2\xa9\x20\x60\x32\x30\x31\x37\x60\x20\x60\x53\x69\x72\x20\x42\x65\x64\x69\x76\x65\x72\x65\x0a\x50\x65\x72\x6d\x69\x73\x73\x69\x6f\x6e\x20\x69\x73\x20\x68\x65\x72\x65\x62\x79\x20\x67\x72\x61\x6e\x74\x65\x64\x2c\x20\x66\x72\x65\x65\x20\x6f\x66\x20\x63\x68\x61\x72\x67\x65\x2c\x20\x74\x6f\x20\x61\x6e\x79\x20\x70\x65\x72\x73\x6f\x6e\x0a\x6f\x62\x74\x61\x69\x6e\x69\x6e\x67\x20\x61\x20\x63\x6f\x70\x79\x20\x6f\x66\x20\x74\x68\x69\x73\x20\x73\x6f\x66\x74\x77\x61\x72\x65\x20\x61\x6e\x64\x20\x61\x73\x73\x6f\x63\x69\x61\x74\x65\x64\x20\x64\x6f\x63\x75\x6d\x65\x6e\x74\x61\x74\x69\x6f\x6e\x0a\x66\x69\x6c\x65\x73\x20\x28\x74\x68\x65\x20\xe2\x80\x9c\x53\x6f\x66\x74\x77\x61\x72\x65\xe2\x80\x9d\x29\x2c\x20\x74\x6f\x20\x64\x65\x61\x6c\x20\x69\x6e\x20\x74\x68\x65\x20\x53\x6f\x66\x74\x77\x61\x72\x65\x20\x77\x69\x74\x68\x6f\x75\x74\x0a\x72\x65\x73\x74\x72\x69\x63\x74\x69\x6f\x6e\x2c\x20\x69\x6e\x63\x6c\x75\x64\x69\x6e\x67\x20\x77\x69\x74\x68\x6f\x75\x74\x20\x6c\x69\x6d\x69\x74\x61\x74\x69\x6f\x6e\x20\x74\x68\x65\x20\x72\x69\x67\x68\x74\x73\x20\x74\x6f\x20\x75\x73\x65\x2c\x0a\x63\x6f\x70\x79\x2c\x20\x6d\x6f\x64\x69\x66\x79\x2c\x20\x6d\x65\x72\x67\x65\x2c\x20\x70\x75\x62\x6c\x69\x73\x68\x2c\x20\x64\x69\x73\x74\x72\x69\x62\x75\x74\x65\x2c\x20\x73\x75\x62\x6c\x69\x63\x65\x6e\x73\x65\x2c\x20\x61\x6e\x64\x2f\x6f\x72\x20\x73\x65\x6c\x6c\x0a\x63\x6f\x70\x69\x65\x73\x20\x6f\x66\x20\x74\x68\x65\x20\x53\x6f\x66\x74\x77\x61\x72\x65\x2c\x20\x61\x6e\x64\x20\x74\x6f\x20\x70\x65\x72\x6d\x69\x74\x20\x70\x65\x72\x73\x6f\x6e\x73\x20\x74\x6f\x20\x77\x68\x6f\x6d\x20\x74\x68\x65\x0a\x53\x6f\x66\x74\x77\x61\x72\x65\x20\x69\x73\x20\x66\x75\x72\x6e\x69\x73\x68\x65\x64\x20\x74\x6f\x20\x64\x6f\x20\x73\x6f\x2c\x20\x73\x75\x62\x6a\x65\x63\x74\x20\x74\x6f\x20\x74\x68\x65\x20\x66\x6f\x6c\x6c\x6f\x77\x69\x6e\x67\x0a\x63\x6f\x6e\x64\x69\x74\x69\x6f\x6e\x73\x3a\x0a\x0a\x54\x68\x65\x20\x61\x62\x6f\x76\x65\x20\x63\x6f\x70\x79\x72\x69\x67\x68\x74\x20\x6e\x6f\x74\x69\x63\x65\x20\x61\x6e\x64\x20\x74\x68\x69\x73\x20\x70\x65\x72\x6d\x69\x73\x73\x69\x6f\x6e\x20\x6e\x6f\x74\x69\x63\x65\x20\x73\x68\x61\x6c\x6c\x20\x62\x65\x0a\x69\x6e\x63\x6c\x75\x64\x65\x64\x20\x69\x6e\x20\x61\x6c\x6c\x20\x63\x6f\x70\x69\x65\x73\x20\x6f\x72\x20\x73\x75\x62\x73\x74\x61\x6e\x74\x69\x61\x6c\x20\x70\x6f\x72\x74\x69\x6f\x6e\x73\x20\x6f\x66\x20\x74\x68\x65\x20\x53\x6f\x66\x74\x77\x61\x72\x65\x2e\x0a\x0a\x54\x48\x45\x20\x53\x4f\x46\x54\x57\x41\x52\x45\x20\x49\x53\x20\x50\x52\x4f\x56\x49\x44\x45\x44\x20\xe2\x80\x9c\x41\x53\x20\x49\x53\xe2\x80\x9d\x2c\x20\x57\x49\x54\x48\x4f\x55\x54\x20\x57\x41\x52\x52\x41\x4e\x54\x59\x20\x4f\x46\x20\x41\x4e\x59\x20\x4b\x49\x4e\x44\x2c\x0a\x45\x58\x50\x52\x45\x53\x53\x20\x4f\x52\x20\x49\x4d\x50\x4c\x49\x45\x44\x2c\x20\x49\x4e\x43\x4c\x55\x44\x49\x4e\x47\x20\x42\x55\x54\x20\x4e\x4f\x54\x20\x4c\x49\x4d\x49\x54\x45\x44\x20\x54\x4f\x20\x54\x48\x45\x20\x57\x41\x52\x52\x41\x4e\x54\x49\x45\x53\x0a\x4f\x46\x20\x4d\x45\x52\x43\x48\x41\x4e\x54\x41\x42\x49\x4c\x49\x54\x59\x2c\x20\x46\x49\x54\x4e\x45\x53\x53\x20\x46\x4f\x52\x20\x41\x20\x50\x41\x52\x54\x49\x43\x55\x4c\x41\x52\x20\x50\x55\x52\x50\x4f\x53\x45\x20\x41\x4e\x44\x0a\x4e\x4f\x4e\x49\x4e\x46\x52\x49\x4e\x47\x45\x4d\x45\x4e\x54\x2e\x20\x49\x4e\x20\x4e\x4f\x20\x45\x56\x45\x4e\x54\x20\x53\x48\x41\x4c\x4c\x20\x54\x48\x45\x20\x41\x55\x54\x48\x4f\x52\x53\x20\x4f\x52\x20\x43\x4f\x50\x59\x52\x49\x47\x48\x54\x0a\x48\x4f\x4c\x44\x45\x52\x53\x20\x42\x45\x20\x4c\x49\x41\x42\x4c\x45\x20\x46\x4f\x52\x20\x41\x4e\x59\x20\x43\x4c\x41\x49\x4d\x2c\x20\x44\x41\x4d\x41\x47\x45\x53\x20\x4f\x52\x20\x4f\x54\x48\x45\x52\x20\x4c\x49\x41\x42\x49\x4c\x49\x54\x59\x2c\x0a\x57\x48\x45\x54\x48\x45\x52\x20\x49\x4e\x20\x41\x4e\x20\x41\x43\x54\x49\x4f\x4e\x20\x4f\x46\x20\x43\x4f\x4e\x54\x52\x41\x43\x54\x2c\x20\x54\x4f\x52\x54\x20\x4f\x52\x20\x4f\x54\x48\x45\x52\x57\x49\x53\x45\x2c\x20\x41\x52\x49\x53\x49\x4e\x47\x0a\x46\x52\x4f\x4d\x2c\x20\x4f\x55\x54\x20\x4f\x46\x20\x4f\x52\x20\x49\x4e\x20\x43\x4f\x4e\x4e\x45\x43\x54\x49\x4f\x4e\x20\x57\x49\x54\x48\x20\x54\x48\x45\x20\x53\x4f\x46\x54\x57\x41\x52\x45\x20\x4f\x52\x20\x54\x48\x45\x20\x55\x53\x45\x20\x4f\x52\x0a\x4f\x54\x48\x45\x52\x20\x44\x45\x41\x4c\x49\x4e\x47\x53\x20\x49\x4e\x20\x54\x48\x45\x20\x53\x4f\x46\x54\x57\x41\x52\x45\x2e\x0a", 1110);

// TESTS
// -----


// TODO: restore
#if 0
TEST(compression_stream, bz2_istream)
{
    std::istringstream sstream(BZ2_COMPRESSED);
    bz2_istream bz2(sstream);
    std::ostringstream ostream;
    ostream << bz2.rdbuf();

    EXPECT_EQ(ostream.str(), DECOMPRESSED);
}


TEST(compression_stream, bz2_ostream)
{
    std::ostringstream sstream;
    {
        bz2_ostream bz2(sstream);
        bz2 << DECOMPRESSED;
    }

    EXPECT_EQ(sstream.str(), BZ2_COMPRESSED);
}


TEST(compression_stream, zlib_istream)
{
    std::istringstream sstream(ZLIB_COMPRESSED);
    zlib_istream zlib(sstream);
    std::ostringstream ostream;
    ostream << zlib.rdbuf();

    EXPECT_EQ(ostream.str(), DECOMPRESSED);
}


TEST(compression_stream, zlib_ostream)
{
    std::ostringstream sstream;
    {
        zlib_ostream zlib(sstream);
        zlib << DECOMPRESSED;
    }

    EXPECT_EQ(sstream.str(), ZLIB_COMPRESSED);
}


TEST(compression_stream, lzma_istream)
{
    std::istringstream sstream(LZMA_COMPRESSED);
    lzma_istream lzma(sstream);
    std::ostringstream ostream;
    ostream << lzma.rdbuf();

    EXPECT_EQ(ostream.str(), DECOMPRESSED);
}


TEST(compression_stream, lzma_ostream)
{
    std::ostringstream sstream;
    {
        lzma_ostream lzma(sstream);
        lzma << DECOMPRESSED;
    }

    EXPECT_EQ(sstream.str(), LZMA_COMPRESSED);
}


TEST(compression_stream, gzip_istream)
{
    std::istringstream sstream(GZIP_COMPRESSED);
    gzip_istream gzip(sstream);
    std::ostringstream ostream;
    ostream << gzip.rdbuf();

    EXPECT_EQ(ostream.str(), DECOMPRESSED);
}


TEST(compression_stream, gzip_ostream)
{
    std::ostringstream sstream;
    {
        gzip_ostream gzip(sstream);
        gzip << DECOMPRESSED;
    }

    // don't check exact values, just check it can decompress
    EXPECT_EQ(gzip_decompress(sstream.str()), DECOMPRESSED);
}



TEST(compression_stream, decompressing_istream)
{
    // bzip2
    std::ostringstream ostream;
    std::istringstream sstream(BZ2_COMPRESSED);
    {
        decompressing_istream compressed(sstream);
        ostream << compressed.rdbuf();
    }
    EXPECT_EQ(ostream.str(), DECOMPRESSED);

    // zlib
    ostream = std::ostringstream();
    sstream = std::istringstream(ZLIB_COMPRESSED);
    {
        decompressing_istream compressed(sstream);
        ostream << compressed.rdbuf();
    }
    EXPECT_EQ(ostream.str(), DECOMPRESSED);

    // lzma
    ostream = std::ostringstream();
    sstream = std::istringstream(LZMA_COMPRESSED);
    {
        decompressing_istream compressed(sstream);
        ostream << compressed.rdbuf();
    }
    EXPECT_EQ(ostream.str(), DECOMPRESSED);

    // gzip
    ostream = std::ostringstream();
    sstream = std::istringstream(GZIP_COMPRESSED);
    {
        decompressing_istream compressed(sstream);
        ostream << compressed.rdbuf();
    }
    EXPECT_EQ(ostream.str(), DECOMPRESSED);
}
#endif
