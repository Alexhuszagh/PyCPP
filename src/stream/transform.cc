//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  These streambufs are implemented as part of the excellent
 *  reference from Angelika Langer's excellent series.
 *
 *  http://www.angelikalanger.com/Articles/Topics.html#CPP
 *  https://artofcode.wordpress.com/2010/12/12/deriving-from-stdstreambuf/
 */

#include "transform.h"


// OBJECTS
// -------

// IOSTREAM

transform_iostream::transform_iostream():
    std::iostream(&buffer)
{}


transform_iostream::~transform_iostream()
{}


//transform_iostream::transform_iostream(transform_iostream &&other):
//    std::iostream(std::move(other)),
//    stream(std::move(other.stream)),
//    buffer(std::move(other.buffer))
//{
//    std::ios::rdbuf(&buffer);
//}
//
//
//transform_iostream & transform_iostream::operator=(transform_iostream &&other)
//{
//    swap(other);
//    return *this;
//}


transform_iostream::transform_iostream(std::iostream& stream):
    std::iostream(&buffer)
{
    open(stream);
}


void transform_iostream::open(std::iostream& stream)
{
    this->stream = &stream;
}


transform_streambuf* transform_iostream::rdbuf() const
{
    return const_cast<transform_streambuf*>(&buffer);
}


void transform_iostream::rdbuf(transform_streambuf *buffer)
{
    std::ios::rdbuf(buffer);
}


//void transform_iostream::swap(transform_iostream &other)
//{
//    // swap
//    std::iostream::swap(other);
//    std::swap(stream, other.stream);
//    std::swap(buffer, other.buffer);
//
//    // set filebuffers
//    std::ios::rdbuf(&buffer);
//    other.rdbuf(&other.buffer);
//}

// ISTREAM

// OSTREAM

// FSTREAM

bool transform_fstream::is_open() const
{
    return file.is_open();
}


void transform_fstream::close()
{
    file.close();
}

// IFSTREAM

bool transform_ifstream::is_open() const
{
    return file.is_open();
}


void transform_ifstream::close()
{
    file.close();
}

// OFSTREAM

bool transform_ofstream::is_open() const
{
    return file.is_open();
}


void transform_ofstream::close()
{
    file.close();
}
