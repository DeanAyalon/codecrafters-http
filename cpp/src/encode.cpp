#include "encode.hpp"

#include <stdexcept>
#include <algorithm>
#include <string>
#include <vector>
#include <zlib.h>

#include "utils/console.hpp"
#include "utils/utils.hpp"

using std::string;
using std::vector;

#define BUFFER_SIZE 1024

// Internal
string gzip_compress(const string &data) {
    z_stream zs{};
    const int windowBits = 15,  // max: 32KB (2^15) gzip window size
        gzipFlag = 16,          // write gzip header/trailer
        memoryLevel = 9;        // max: 256KB of memory - best compression
    // Memory used per stream = 390 KB
    // Formula: (1 << (windowBits + 2)) + (1 << (memLevel + 9)) + 6 KB 
    //             << means bit-wise shift (take 1 as a binary 0001, and shift it left x positions).  ex: 5 << 10 = 5 * 2^10 = 5*1024 (5KB)

    //  return codes            Max compression     algorithm                                       balanced speed and size
    int ret = deflateInit2(&zs, Z_BEST_COMPRESSION, Z_DEFLATED, windowBits | gzipFlag, memoryLevel, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK) throw std::runtime_error("deflateInit2 failed, returned value: " + std::to_string(ret));

    //           Reinterprets char* as Bytef*
    zs.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(data.data()));
    zs.avail_in = data.size();      // Bytes

    char outbuffer[BUFFER_SIZE];    // Compressed chunks temp storage
    std::string result;             // compressed data

    do {
        zs.next_out = reinterpret_cast<Bytef *>(outbuffer);         // Sets the designation of the next compressed data chunk
        zs.avail_out = sizeof(outbuffer);                           // Sets the size available for the next compressed data chunk

        ret = deflate(&zs, Z_FINISH);                               // Compress the next chunk
        if (ret == Z_STREAM_ERROR) 
            throw std::runtime_error("zlib deflate() failed with Z_STREAM_ERROR");

        //                       memory given - memory unused (still available)
        result.append(outbuffer, sizeof(outbuffer) - zs.avail_out); // Add the compressed data to the result
    } while (ret != Z_STREAM_END);

    deflateEnd(&zs);
    return result;
}

// Exported
string selectEncoding(vector<string> encodings) {
    // Supported encodings
    if (std::find(encodings.begin(), encodings.end(), "gzip") != encodings.end()) return "gzip";
    return "";
}

string encode(const string &data, const string &encoding) {
    if (data.empty()) return data;
    if (encoding.compare("gzip") == 0) return gzip_compress(data);
    return data;
}
