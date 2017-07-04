//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief JSON DOM parser.
 */

#pragma once

#include <json/sax.h>
#include <json/writer.h>
#include <vector>

// OBJECTS
// -------


/**
 *  \brief Handler for the JSON DOM.
 */
struct json_dom_handler: json_sax_handler
{
public:
    json_dom_handler(json_value_t&);

    virtual void start_document() override;
    virtual void end_document() override;
    virtual void start_object() override;
    virtual void end_object(size_t) override;
    virtual void start_array() override;
    virtual void end_array(size_t) override;
    virtual void key(const string_view&) override;
    virtual void null() override;
    virtual void boolean(bool) override;
    virtual void number(double) override;
    virtual void string(const string_view&) override;

private:
    json_value_t* root_ = nullptr;
    bool has_key_ = false;
    json_string_t key_;
    std::vector<json_value_t*> levels_;
};


/**
 *  \brief JSON value type.
 *
 *  Store the data in a 64-bit type, storing the data by value for
 *  small types (null, bool, numbers) and by pointer for large
 *  values (string, array, object).
 */
struct json_value_t
{
public:
    json_value_t();
    json_value_t(json_value_t&&);
    json_value_t & operator=(json_value_t&&);

    json_value_t(json_type type);
    json_value_t(json_null_t);
    json_value_t(json_boolean_t);
    json_value_t(json_number_t);
    json_value_t(json_string_t&&);
    json_value_t(json_array_t&&);
    json_value_t(json_object_t&&);
    ~json_value_t();

    void swap(json_value_t&);

    // CHECKERS
    json_type type() const;
    bool has_null() const;
    bool has_boolean() const;
    bool has_number() const;
    bool has_string() const;
    bool has_array() const;
    bool has_object() const;

    // GETTERS
    json_null_t get_null() const;
    json_boolean_t get_boolean() const;
    json_number_t get_number() const;
    json_string_t& get_string() const;
    json_array_t& get_array() const;
    json_object_t& get_object() const;

    // SETTERS
    void set_null(json_null_t);
    void set_boolean(json_boolean_t);
    void set_number(json_number_t);
    void set_string(json_string_t&&);
    void set_array(json_array_t&&);
    void set_object(json_object_t&&);
    void set(json_null_t);
    void set(json_boolean_t);
    void set(json_number_t);
    void set(json_string_t&&);
    void set(json_array_t&&);
    void set(json_object_t&&);

    // CONVERSIONS
    operator json_boolean_t() const;
    operator json_number_t() const;
    operator json_string_t&() const;
    operator json_array_t&() const;
    operator json_object_t&() const;

private:
    json_type type_;
    json_pointer_t data_;

    void reset();
};


/**
 *  \brief JSON document type.
 */
struct json_document_t: json_value_t
{
    void loads(const std::string&);
    void load(std::istream&);
    void load(const std::string&);
#if defined(HAVE_WFOPEN)
    void load(const std::wstring&);
#endif

    std::string dumps(char = ' ', int = 4);
    void dump(std::ostream&, char = ' ', int = 4);
    void dump(const std::string&, char = ' ', int = 4);
#if defined(HAVE_WFOPEN)
    void dump(const std::wstring&, char = ' ', int = 4);
#endif
};
