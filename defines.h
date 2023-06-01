#ifndef _DEFINES_H
#define _DEFINES_H
using namespace sc_dt;

#define PE_data_width 12

// typedef sc_int< PE_data_width > PE_data_t;
typedef sc_fixed<16, 8> data_t;
typedef sc_fixed<16, 9> angle_t;
typedef struct CORDIC_output {
    data_t x;
    data_t y;
    angle_t theta;
} CORDIC_output_t;

struct input_t {
    data_t x;
    data_t y;
    angle_t theta;
    bool mode;

    // Default constructor
    input_t() {}
    // Copy Constructor
    input_t(const input_t &obj) {
        x = obj.x;
        y = obj.y;
        theta = obj.theta;
        mode = obj.mode;
    }
    // operator == function
    inline bool operator==(const input_t &other){
        if (x != other.x) return false;
        if (y != other.y) return false;
        if (theta != other.theta) return false;
        if (mode != other.mode) return false;
        return true;
    }
    // Assignment operator.
    inline input_t & operator=(const input_t &other) {
        x = other.x;
        y = other.y;
        theta = other.theta;
        mode = other.mode;
        return *this;
    }
};

// sc_trace function
inline void sc_trace(sc_trace_file *tf, const input_t &obj, const sc_string &name) {
    char buf[16];
    sprintf(buf, ".x");
    sc_trace(tf, obj.x, name + sc_string(buf));
    sprintf(buf, ".y");
    sc_trace(tf, obj.y, name + sc_string(buf));
    sprintf(buf, ".theta");
    sc_trace(tf, obj.theta, name + sc_string(buf));
    sprintf(buf, ".mode");
    sc_trace(tf, obj.mode, name + sc_string(buf));
}

inline ostream & operator<<(ostream &os, const input_t &obj) {
#ifndef STRATUS
    os << " " << obj.x.to_string().c_str();
    os << " " << obj.y.to_string().c_str();
    os << " " << obj.theta.to_string().c_str();
    os << " " << obj.mode;
#endif
    return os;
}

inline istream & operator>>(istream &is, const input_t &obj) {
    bool eof = !is.good();
    is >> std::ws;
    // eof = is.eof();
    // if (!eof) {is >> obj.x;}
    // eof = is.eof();
    // if (!eof) {is >> obj.y;}
    // eof = is.eof();
    // if (!eof) {is >> obj.theta;}
    // eof = is.eof();
    // if (!eof) {is >> obj.mode;}
    return is;
}

struct output_t {
    data_t x;
    data_t y;
    angle_t theta;

    // Default constructor
    output_t() {}
    // Copy Constructor
    output_t(const output_t &obj) {
        x = obj.x;
        y = obj.y;
        theta = obj.theta;
    }
    // operator == function
    inline bool operator==(const output_t &other){
        if (x != other.x) return false;
        if (y != other.y) return false;
        if (theta != other.theta) return false;
        return true;
    }
    // Assignment operator.
    inline output_t & operator=(const output_t &other) {
        x = other.x;
        y = other.y;
        theta = other.theta;
        return *this;
    }
};

// sc_trace function
inline void sc_trace(sc_trace_file *tf, const output_t &obj, const sc_string &name) {
    char buf[16];
    sprintf(buf, ".x");
    sc_trace(tf, obj.x, name + sc_string(buf));
    sprintf(buf, ".y");
    sc_trace(tf, obj.y, name + sc_string(buf));
    sprintf(buf, ".theta");
    sc_trace(tf, obj.theta, name + sc_string(buf));
}

inline ostream & operator<<(ostream &os, const output_t &obj) {
#ifndef STRATUS
    os << " " << obj.x.to_string().c_str();
    os << " " << obj.y.to_string().c_str();
    os << " " << obj.theta.to_string().c_str();
#endif
    return os;
}

inline istream & operator>>(istream &is, const output_t &obj) {
    bool eof = !is.good();
    is >> std::ws;
    // eof = is.eof();
    // if (!eof) {is >> obj.x;}
    // eof = is.eof();
    // if (!eof) {is >> obj.y;}
    // eof = is.eof();
    // if (!eof) {is >> obj.theta;}
    return is;
}



#endif
