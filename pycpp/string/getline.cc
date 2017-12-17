//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/string/getline.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------


istream& getline(istream& stream, std::string& line)
{
    line.clear();

    istream::sentry sentry(stream, true);
    streambuf* buffer = stream.rdbuf();

    for(;;) {
        int8_t c = buffer->sbumpc();
        switch (c) {
        case '\n':
            return stream;

        case '\r':
            if(buffer->sgetc() == '\n'){
                buffer->sbumpc();
            }
            return stream;

        case EOF:
            if(line.empty()) {
                stream.setstate(ios::failbit);
            }
            return stream;

        default:
            line += (char)c;
        }
    }
}

PYCPP_END_NAMESPACE
