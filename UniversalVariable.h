// HEADER
#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include <initializer_list>
#include <iostream>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <vector>

typedef class UniversalVariable var;

UniversalVariable operator "" _v(long double data);
UniversalVariable operator "" _v(unsigned long long int data);
UniversalVariable operator "" _v(const char* str, std::size_t len);
std::ostream& operator<<(std::ostream& out, const UniversalVariable& variable);

class UniversalVariable
{
public:
    UniversalVariable();
    UniversalVariable(const UniversalVariable& data);
    UniversalVariable(const std::initializer_list<UniversalVariable>& data);
    template<typename T> UniversalVariable(const T& data);
    ~UniversalVariable();

    void load(char& data);
    void load(unsigned char& data);
    void load(std::string& data);
    void load(short& data);
    void load(unsigned short& data);
    void load(int& data);
    void load(unsigned int& data);
    void load(long& data);
    void load(unsigned long& data);
    void load(long long& data);
    void load(unsigned long long& data);
    void load(float& data);
    void load(double& data);
    void load(long double& data);
    void load(std::vector<UniversalVariable>& data);
    template<typename T> void load(T& data);

    UniversalVariable& operator=(const UniversalVariable& data);
    UniversalVariable& operator=(const char& data);
    UniversalVariable& operator=(const unsigned char& data);
    UniversalVariable& operator=(const char* data);
    UniversalVariable& operator=(const std::string& data);
    UniversalVariable& operator=(const short& data);
    UniversalVariable& operator=(const unsigned short& data);
    UniversalVariable& operator=(const int& data);
    UniversalVariable& operator=(const unsigned int& data);
    UniversalVariable& operator=(const long& data);
    UniversalVariable& operator=(const unsigned long& data);
    UniversalVariable& operator=(const long long& data);
    UniversalVariable& operator=(const unsigned long long& data);
    UniversalVariable& operator=(const float& data);
    UniversalVariable& operator=(const double& data);
    UniversalVariable& operator=(const long double& data);
    UniversalVariable& operator=(const std::vector<UniversalVariable>& data);
    UniversalVariable& operator=(const std::initializer_list<UniversalVariable>& data);
    template<typename T> UniversalVariable& operator=(const T& data);

    UniversalVariable& operator[](unsigned long long int index);
//private:
    //Info of UniversalVariable
    unsigned int _type;
    unsigned int _count_type = 3;
    enum { REAL = 0, TEXT = 1, ARRAY = 2 };

    //Info of unknown type
    std::unordered_map<std::type_index, int> _type_names;

    //Info of data
    union { long double _real; void* _pointer; } _data;
    std::string _text;
    std::vector<UniversalVariable> _array;
    
    //Access friend class
    friend std::ostream& operator<<(std::ostream& out, const UniversalVariable& variable);
};

//SOURCE
std::ostringstream _str;

// O------------------------------------------------------------------------------O
// | FUNCTIONS FOR UNIVERSALVARIABLE                                              |
// O------------------------------------------------------------------------------O
#pragma region functions_for_universal_variable

UniversalVariable operator "" _v(long double data)
{
    return UniversalVariable(data);
}
UniversalVariable operator "" _v(unsigned long long int data)
{
    return UniversalVariable(data);
}
UniversalVariable operator "" _v(const char* str, std::size_t len)
{
    return UniversalVariable(str);
}
std::ostream& print_vector_UniversalVariable(std::ostream& out, const std::vector<UniversalVariable>& data)
{
    std::vector<UniversalVariable>::const_iterator it = data.begin();
    out << "{ ";
    while (it != data.end() - 1) out << *it++ << ", ";
    return out << *it << " }";
}
std::ostream& operator<<(std::ostream& out, const UniversalVariable& variable)
{
    return (
        (variable._type == variable.REAL) ?
            (out << variable._data._real) : (
        (variable._type == variable.TEXT) ?
            (out << variable._text) : (
        (variable._type == variable.ARRAY) ?
            (print_vector_UniversalVariable(out, variable._array)) :
            (out << variable._data._pointer)
        ))
    );
}

#pragma endregion

// O------------------------------------------------------------------------------O
// | CONSTRUCTOR AND DECONSTRUCTOR                                                |
// O------------------------------------------------------------------------------O
#pragma region constructor_and_deconstructor

UniversalVariable::UniversalVariable()
{ UniversalVariable("NULL"); }
UniversalVariable::UniversalVariable(const UniversalVariable& data)
{ *this = data; }
UniversalVariable::UniversalVariable(const std::initializer_list<UniversalVariable>& data)
{ *this = data; }
template<typename T> UniversalVariable::UniversalVariable(const T& data)
{ *this = data; }
UniversalVariable::~UniversalVariable()
{ }

#pragma endregion

// O------------------------------------------------------------------------------O
// | LOAD OR GET VALUE                                                            |
// O------------------------------------------------------------------------------O
#pragma region load_value

//TEXT TYPE
void UniversalVariable::load(char& data)
{
    if (_type == TEXT) data = _text[0];
    else if (_type == REAL) data = std::to_string(_data._real)[0];
    else data = 0;
}
void UniversalVariable::load(unsigned char& data)
{
    if (_type == TEXT) data = _text[0];
    else if (_type == REAL) data = std::to_string(_data._real)[0];
    else data = 0;
}
void UniversalVariable::load(std::string& data)
{
    if (_type == TEXT) data = _text;
    else if (_type == REAL) {
        _str.clear();
        _str << std::setprecision(16) << _data._real;
        data = _str.str();
    }
    else data = "";
}

//REAL TYPE
void UniversalVariable::load(short& data)
{
    if (_type == REAL) data = _data._real;
    else if (_type == TEXT) data = std::stoi(_text.c_str());
    else data = 0;
}
void UniversalVariable::load(unsigned short& data)
{
    if (_type == REAL) data = _data._real;
    else if (_type == TEXT) data = std::stoi(_text.c_str());
    else data = 0;
}
void UniversalVariable::load(int& data)
{
    if (_type == REAL) data = _data._real;
    else if (_type == TEXT) data = std::stoi(_text.c_str());
    else data = 0;
}
void UniversalVariable::load(unsigned int& data)
{
    if (_type == REAL) data = _data._real;
    else if (_type == TEXT) data = std::stoi(_text.c_str());
    else data = 0;
}
void UniversalVariable::load(long& data)
{
    if (_type == REAL) data = _data._real;
    else if (_type == TEXT) data = std::stol(_text.c_str());
    else data = 0;
}
void UniversalVariable::load(unsigned long& data)
{
    if (_type == REAL) data = _data._real;
    else if (_type == TEXT) data = std::stol(_text.c_str());
    else data = 0;
}
void UniversalVariable::load(long long& data)
{
    if (_type == REAL) data = _data._real;
    else if (_type == TEXT) data = std::stoll(_text.c_str());
    else data = 0;
}
void UniversalVariable::load(unsigned long long& data)
{
    if (_type == REAL) data = _data._real;
    else if (_type == TEXT) data = std::stoll(_text.c_str());
    else data = 0;
}
void UniversalVariable::load(float& data)
{
    if (_type == REAL) data = _data._real;
    else if (_type == TEXT) data = std::stof(_text.c_str());
    else data = 0;
}
void UniversalVariable::load(double& data)
{
    if (_type == REAL) data = _data._real;
    else if (_type == TEXT) data = std::stod(_text.c_str());
    else data = 0;
}
void UniversalVariable::load(long double& data)
{
    if (_type == REAL) data = _data._real;
    else if (_type == TEXT) data = std::stold(_text.c_str());
    else data = 0;
}

//ARRAY TYPE
void UniversalVariable::load(std::vector<UniversalVariable>& data)
{
    if (_type == ARRAY) data = _array;
    else data = std::vector<UniversalVariable>({ *this });
}

//UNKNOWN TYPE
template<typename T>
void UniversalVariable::load(T& data)
{
    auto search = _type_names.find(std::type_index(typeid(data)));
    if (search != _type_names.end()) data = *(T*)_data._pointer;
    else data = (T){ 0 };
}

#pragma endregion

// O------------------------------------------------------------------------------O
// | OPERATOR =                                                                   |
// O------------------------------------------------------------------------------O
#pragma region operator_assign

//VAR TYPE
UniversalVariable& UniversalVariable::operator=(const UniversalVariable& data)
{ _type = data._type; _count_type = data._count_type; _type_names = data._type_names;
  _data = data._data; _text = data._text; _array = data._array; return *this; }

//TEXT TYPE
UniversalVariable& UniversalVariable::operator=(const char& data)
{ _type = TEXT; _text = data; return *this; }
UniversalVariable& UniversalVariable::operator=(const unsigned char& data)
{ _type = TEXT; _text = data;  return *this; }
UniversalVariable& UniversalVariable::operator=(const char* data)
{ _type = TEXT; _text = data; return *this; }
UniversalVariable& UniversalVariable::operator=(const std::string& data)
{ _type = TEXT; _text = data;  return *this; }

//REAL TYPE
UniversalVariable& UniversalVariable::operator=(const short& data)
{ _type = REAL; _data._real = data; return *this; }
UniversalVariable& UniversalVariable::operator=(const unsigned short& data)
{ _type = REAL; _data._real = data; return *this; }
UniversalVariable& UniversalVariable::operator=(const int& data)
{ _type = REAL; _data._real = data; return *this; }
UniversalVariable& UniversalVariable::operator=(const unsigned int& data)
{ _type = REAL; _data._real = data; return *this; }
UniversalVariable& UniversalVariable::operator=(const long& data)
{ _type = REAL; _data._real = data; return *this; }
UniversalVariable& UniversalVariable::operator=(const unsigned long& data)
{ _type = REAL; _data._real = data; return *this; }
UniversalVariable& UniversalVariable::operator=(const long long& data)
{ _type = REAL; _data._real = data; return *this; }
UniversalVariable& UniversalVariable::operator=(const unsigned long long& data)
{ _type = REAL; _data._real = data; return *this; }
UniversalVariable& UniversalVariable::operator=(const float& data)
{ _type = REAL; _data._real = data; return *this; }
UniversalVariable& UniversalVariable::operator=(const double& data)
{ _type = REAL; _data._real = data; return *this; }
UniversalVariable& UniversalVariable::operator=(const long double& data)
{ _type = REAL; _data._real = data; return *this; }

//ARRAY TYPE
UniversalVariable& UniversalVariable::operator=(const std::vector<UniversalVariable>& data)
{ _type = ARRAY; _array = data; return *this; }
UniversalVariable& UniversalVariable::operator=(const std::initializer_list<UniversalVariable>& data)
{ _type = ARRAY; _array = data; return *this; }

//UNKNOWN TYPE
template<typename T>
UniversalVariable& UniversalVariable::operator=(const T& data)
{
    auto search = _type_names.find(std::type_index(typeid(data)));
    if (search == _type_names.end())
    {
        _type_names[std::type_index(typeid(data))] = _count_type;
        _count_type++;
    }
    _type = _type_names[std::type_index(typeid(data))];
    _data._pointer = (void*)&data; 
    return *this; 
}

#pragma endregion

// O------------------------------------------------------------------------------O
// | OPERATOR []                                                                  |
// O------------------------------------------------------------------------------O
#pragma region operator_index

UniversalVariable& UniversalVariable::operator[](unsigned long long int index)
{
    if (_type == ARRAY)
    {
        if (index < _array.size()) return _array[index];
        else return *this;
    } 
    else return *this;
}

#pragma endregion
