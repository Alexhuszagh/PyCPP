//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief JSON DOM parser.
 */

#pragma once

#include <pycpp/json/sax.h>
#include <pycpp/json/writer.h>
#include <pycpp/stl/deque.h>
#include <pycpp/string/string.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


/**
 *  \brief Handler for the JSON DOM.
 */
struct json_dom_handler: json_sax_handler
{
public:
    json_dom_handler() = delete;
    json_dom_handler(const json_dom_handler&) = delete;
    json_dom_handler& operator=(const json_dom_handler&) = delete;
    json_dom_handler(json_dom_handler&&);
    json_dom_handler& operator=(json_dom_handler&&);
    json_dom_handler(json_value_t&);

    // SAX EVENTS
    virtual void start_document() override;
    virtual void end_document() override;
    virtual void start_object() override;
    virtual void end_object(size_t) override;
    virtual void start_array() override;
    virtual void end_array(size_t) override;
    virtual void key(const string_wrapper&) override;
    virtual void null() override;
    virtual void boolean(bool) override;
    virtual void number(double) override;
    virtual void string(const string_wrapper&) override;

    // MODIFIERS
    void swap(json_dom_handler&);

private:
    json_value_t* root_ = nullptr;
    bool has_key_ = false;
    json_string_t key_;
    deque<json_value_t*> levels_;
};


/**
 *  \brief JSON document type.
 */
struct json_document_t: json_value_t
{
    json_document_t();
    json_document_t(const json_document_t&) = delete;
    json_document_t& operator=(const json_document_t&) = delete;
    json_document_t(json_document_t&&);
    json_document_t& operator=(json_document_t&&);

    // READERS
    void loads(const string_wrapper&);
    void load(istream&);
    void load(const string_view&);
#if defined(HAVE_WFOPEN)                        // WINDOWS
    void load(const wstring_view&);
    void load(const u16string_view&);
#endif                                          // WINDOWS

    // WRITERS
    std::string dumps(char = ' ', int = 4);
    void dump(ostream&, char = ' ', int = 4);
    void dump(const string_view&, char = ' ', int = 4);
#if defined(HAVE_WFOPEN)                        // WINDOWS
    void dump(const wstring_view&, char = ' ', int = 4);
    void dump(const u16string_view&, char = ' ', int = 4);
#endif                                          // WINDOWS
};

PYCPP_END_NAMESPACE
