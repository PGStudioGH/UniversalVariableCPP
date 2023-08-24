// HEADER
#include <typeinfo>
#include <typeindex>
#include <algorithm>
#include <type_traits>
#include <initializer_list>

#include <cstring>
#include <sstream>
#include <iomanip>

#include <vector>

#include <iostream>

#pragma region typedefs

typedef class UniversalVariable var;

#pragma endregion

#pragma region global_variables

enum { 
    __REAL      = 0, 
    __TEXT      = 1, 
    __ARRAY     = 2,
    __POINTER   = 3,
};

enum {
    __IS_CALLABLE           = 0x01,
    __IS_RETURN_FUNCTION    = 0x02,
};

std::ostringstream __str;
std::vector<const std::type_info*> __type_names = {  };

bool __isSameTypeArgs;
unsigned long long __index_args;
unsigned long long __count_tab = 0;

#pragma endregion

#pragma region meta_programming

#define __GET_FLAG(FLAGS, FLAG) (bool)(FLAGS & FLAG)
#define __SET_FLAG(FLAGS, FLAG, TOGGLE) TOGGLE ? FLAGS |= FLAG : FLAGS &= ~FLAG

template<class T, class... Args>
struct __is_same_type : std::bool_constant<(std::is_same_v<T, Args> || ...)> {};
template<class T, class... Args>
inline constexpr bool __is_same_type_v = __is_same_type<T, Args...>::value;

template <typename T>
struct __return_type { using type = T; };
template <typename R, typename... Args>
struct __return_type<R(Args...)> { using type = R; };
template <typename R, typename... Args>
struct __return_type<R(*)(Args...)> { using type = R; };
template <typename T>
using __return_type_t = typename __return_type<T>::type;

#pragma endregion

#pragma region functions_for_universal_variable

UniversalVariable operator "" _v(long double data);
UniversalVariable operator "" _v(unsigned long long int data);
UniversalVariable operator "" _v(const char* str, std::size_t len);
std::ostream& operator<<(std::ostream& out, const UniversalVariable& variable);
std::ostream& operator<<(std::ostream& out, const std::vector<UniversalVariable>& data);

#pragma endregion

class UniversalVariable
{

#pragma region constructor_and_deconstructor

public:
    UniversalVariable()
    {
        *this = "NULL";
    }
    UniversalVariable(const UniversalVariable& data)
    {
        _flags = data._flags;
        _type = data._type;
        _unknown_type = data._unknown_type;
        _size_of_allocated_data = data._size_of_allocated_data;
        _data = data._data;
        _text = data._text;
        _array = data._array;
        if (_size_of_allocated_data > 0)
        {
            _allocated_data = malloc(_size_of_allocated_data);
            memcpy(_allocated_data, data._allocated_data, _size_of_allocated_data);
        }
        _count_args = data._count_args;
        _type_args = data._type_args;
    }
    UniversalVariable(const std::initializer_list<UniversalVariable>& data)
    {
        *this = data;
    }
    template<typename T> UniversalVariable(const T& data)
    {
        *this = data;
    }
    ~UniversalVariable()
    {
        if (_size_of_allocated_data > 0) {
            free(_allocated_data);
        }
    }

#pragma endregion

#pragma region isSameType_and_load

public:
    template<typename T> bool isSameType(const T& data)
    {
        if constexpr (__is_same_type_v<T, UniversalVariable>)
            return _type == data._type;
        if constexpr (__is_same_type_v<T, bool>)
            return _type == __TEXT && (_text == "true" || _text == "false");
        else if constexpr (__is_same_type_v<T, char, unsigned char, std::string>)
            return _type == __TEXT;
        else if constexpr (__is_same_type_v<T, short, int, long, long long, float, double, long double,
                            unsigned short, unsigned int, unsigned long, unsigned long long>)
            return _type == __REAL;
        else if constexpr (__is_same_type_v<T, std::vector<UniversalVariable>, std::initializer_list<UniversalVariable>>)
            return _type == __ARRAY;
        else {
            auto search = std::find(__type_names.begin(), __type_names.end(), &typeid(data));
            return search != __type_names.end() && _unknown_type == search - __type_names.begin();
        }
    }

    template<typename T, class... Args> void load(T& data)
    {
        if constexpr (__is_same_type_v<T, UniversalVariable>)
            data = *this;
        else if constexpr (__is_same_type_v<T, bool>) {
            if (_type == __REAL)
                data = (int)_data._real % 2;
            else if (_type == __TEXT)
                data = _text == "true";
            else
                data = false;
        }
        else if constexpr (__is_same_type_v<T, char, unsigned char>) {
            if (_type == __REAL) {
                __str.clear();
                __str << std::setprecision(16) << _data._real;
                data = __str.str()[0];
            }
            else if (_type == __TEXT)
                data = _text[0];
            else
                data = 0;
        }
        else if constexpr (__is_same_type_v<T, std::string>) {
            if (_type == __REAL) {
                __str.clear();
                __str << std::setprecision(16) << _data._real;
                data = __str.str();
            }
            else if (_type == __TEXT)
                data = _text;
            else
                data = "";
        }
        else if constexpr (__is_same_type_v<T, short, unsigned short, int, unsigned int>) {
            if (_type == __REAL)
                data = _data._real;
            else if (_type == __TEXT)
                data = std::stoi(_text.c_str());
            else
                data = 0;
        }
        else if constexpr (__is_same_type_v<T, long, unsigned long>) {
            if (_type == __REAL)
                data = _data._real;
            else if (_type == __TEXT)
                data = std::stol(_text.c_str());
            else
                data = 0;
        }
        else if constexpr (__is_same_type_v<T, long long, unsigned long long>) {
            if (_type == __REAL)
                data = _data._real;
            else if (_type == __TEXT)
                data = std::stoll(_text.c_str());
            else
                data = 0;
        }
        else if constexpr (__is_same_type_v<T, float>) {
            if (_type == __REAL)
                data = _data._real;
            else if (_type == __TEXT)
                data = std::stof(_text.c_str());
            else
                data = 0;
        }
        else if constexpr (__is_same_type_v<T, double>) {
            if (_type == __REAL)
                data = _data._real;
            else if (_type == __TEXT)
                data = std::stod(_text.c_str());
            else
                data = 0;
        }
        else if constexpr (__is_same_type_v<T, long double>) {
            if (_type == __REAL)
                data = _data._real;
            else if (_type == __TEXT)
                data = std::stold(_text.c_str());
            else
                data = 0;
        }
        else if constexpr (__is_same_type_v<T, std::vector<UniversalVariable>>) {
            if (_type == __ARRAY)
                data = _array;
            else
                data = std::vector<UniversalVariable>({ *this });
        }
        else if constexpr (std::is_function_v<typename std::remove_pointer<T>::type>) {
            if constexpr (std::is_pointer_v<T>) {
                if (isSameType(*data))
                    data = T(_data._pointer);
                else {
                    std::cout << "\nFailed load!\n";
                    std::cout << "Reason: Type of input variable does not match!\n";
                    std::cout << "Please check type before operator using `IsSameType(...)`\n\n";
                    std::exit(1);
                }
            }
            else {
                std::cout << "\nFailed load!\n";
                std::cout << "Reason: You should not load to address function!\n\n";
                std::exit(1);
            }
        }
        else {
            if (isSameType(data))
                data = *(T*)_data._pointer;
            else {
                std::cout << "\nFailed load!\n";
                std::cout << "Reason: Type of input variable does not match!\n";
                std::cout << "Please check type before operator using `IsSameType(...)`\n\n";
                std::exit(1);
            }
        }
    }

#pragma endregion

#pragma region operator_assign

public:
    UniversalVariable& operator=(const UniversalVariable& data)
    {
        _flags = data._flags;
        _type = data._type;
        _unknown_type = data._unknown_type;
        _size_of_allocated_data = data._size_of_allocated_data;
        _data = data._data;
        _text = data._text;
        _array = data._array;
        if (_size_of_allocated_data > 0)
        {
            _allocated_data = malloc(_size_of_allocated_data);
            memcpy(_allocated_data, data._allocated_data, _size_of_allocated_data);
        }
        _count_args = data._count_args;
        _type_args = data._type_args;
        return *this;
    }
    template<typename T> UniversalVariable& operator=(const T& data)
    {
        if (_size_of_allocated_data > 0) {
            free(_allocated_data);
            _size_of_allocated_data = 0;
        }

        if constexpr (__is_same_type_v<T, bool>) {
            __SET_FLAG(_flags, __IS_CALLABLE, false);
            _type = __TEXT;
            _text = data ? "true" : "false";
        }
        else if constexpr (__is_same_type_v<T, char, unsigned char, char[sizeof(__return_type_t<T>)], const char[sizeof(__return_type_t<T>)], char*, const char*, std::string>) {
            __SET_FLAG(_flags, __IS_CALLABLE, false);
            _type = __TEXT;
            _text = data;
        }
        else if constexpr (__is_same_type_v<T, short, int, long, long long, float, double, long double,
                            unsigned short, unsigned int, unsigned long, unsigned long long>) {
            __SET_FLAG(_flags, __IS_CALLABLE, false);
            _type = __REAL;
            _data._real = data;
        }
        else if constexpr (__is_same_type_v<T, std::vector<UniversalVariable>, std::initializer_list<UniversalVariable>>) {
            __SET_FLAG(_flags, __IS_CALLABLE, false);
            _type = __ARRAY;
            _array = data;
        }
        else if constexpr (__is_same_type_v<T, std::remove_extent_t<__return_type_t<T>>[sizeof(__return_type_t<T>) / sizeof(std::remove_extent_t<__return_type_t<T>>)]>) {
            __SET_FLAG(_flags, __IS_CALLABLE, false);
            _type = __ARRAY;
            _array.clear();
            for (unsigned int i = 0; i < sizeof(T) / sizeof(std::remove_extent_t<T>); i++)
                _array.push_back(data[i]);
        }
        else if constexpr (std::is_function_v<typename std::remove_pointer<T>::type>) {
            if constexpr (__is_same_type_v<__return_type_t<T>, UniversalVariable>)
                __SET_FLAG(_flags, __IS_RETURN_FUNCTION, true);
            if constexpr (__is_same_type_v<__return_type_t<T>, void, UniversalVariable>) {
                __SET_FLAG(_flags, __IS_CALLABLE, true);
                _count_args = 0;
                _type_args.clear();
                writeArgs(data);
            }
            else __SET_FLAG(_flags, __IS_CALLABLE, false);

            auto search = std::find(__type_names.begin(), __type_names.end(), &typeid(typename std::remove_pointer<T>::type));
            if (search == __type_names.end()) {
                __type_names.push_back(&typeid(typename std::remove_pointer<T>::type));
                search = __type_names.end() - 1;
            }
            _unknown_type = search - __type_names.begin();
            _data._pointer = (void*)data;
        }
        else {
            __SET_FLAG(_flags, __IS_CALLABLE, false);
            auto search = std::find(__type_names.begin(), __type_names.end(), &typeid(data));
            if (search == __type_names.end()) {
                __type_names.push_back(&typeid(data));
                search = __type_names.end() - 1;
            }
            _unknown_type = search - __type_names.begin();
            if constexpr (std::is_pointer_v<T>) {
                _type = __POINTER;
                _data._pointer = (void*)data;
            }
            else {
                _size_of_allocated_data = sizeof(T);
                _allocated_data = malloc(sizeof(T));
                memcpy(_allocated_data, &data, sizeof(T));
            }
        }
        return *this;
    }

#pragma endregion

#pragma region operator_index

public:
    UniversalVariable& operator[](unsigned long long int index)
    {
        if (_type == __ARRAY) {
            if (index < _array.size())
                return _array[index];
            else {
                std::cout << "\nFailed indexing!\n";
                std::cout << "Reason: Index out of array range!\n";
                std::cout << "Please check size of array using `size()`\n\n";
                std::exit(1);
            }
        } 
        else if (index == 0)
            return *this;
        else {
            std::cout << "\nFailed indexing!\n";
            std::cout << "Reason: This variable is not an array!\n";
            std::cout << "Please use index zero, it will work\n\n";
            std::exit(1);
        }
    }

#pragma endregion

#pragma region operator_call_function

private:
    template<class R> void _writeArgs(R f())
    {
        return;
    }
    template<class R, class First, class... Rest> void _writeArgs(R f(First, Rest...))
    {
        _type_args.push_back(&typeid(First));
        _count_args++;

        R (*temp)(Rest...);
        _writeArgs(temp);
    }
    template<class First> void _readArgs(First first)
    {
        __isSameTypeArgs = (&typeid(first) == _type_args[__index_args]);
    }
    template<class First, class... Rest> void _readArgs(First first, Rest... rest)
    {
        __isSameTypeArgs = (&typeid(first) == _type_args[__index_args]);
        __index_args++;
        if (__isSameTypeArgs)
            _readArgs(rest...);
    }

public:
    template<class... Args> UniversalVariable operator()(Args... args)
    {
        if (__GET_FLAG(_flags, __IS_CALLABLE) == true) {
            if (sizeof...(args) == _count_args) {
                __isSameTypeArgs = 1;
                if (_count_args != 0) {
                    __index_args = 0;
                    _readArgs(args...);
                }

                if (__isSameTypeArgs) {
                    if (__GET_FLAG(_flags, __IS_RETURN_FUNCTION) == true)
                        return ((UniversalVariable(*)(Args...))_data._pointer)(args...);
                    else {
                        ((void(*)(Args...))_data._pointer)(args...);
                    }
                }
                else {
                    std::cout << "\nFailed calling function!\n";
                    std::cout << "Reason: Different type of arguments passing!\n";
                    std::cout << "Arguments function: ";
                    if (_type_args.size() == 0)
                        std::cout << "empty\n";
                    else {
                        auto it = _type_args.begin();
                        while (it != _type_args.end() - 1) {
                            std::cout << (*it)->name() << ", ";
                            it++;
                        }
                        std::cout << (*it)->name() << "\n";
                    }
                    std::exit(1);
                }
            }
            else {
                std::cout << "\nFailed calling function!\n";
                std::cout << "Reason: Count of arguments does not match!\n";
                std::cout << "Arguments function: ";
                if (_type_args.size() == 0)
                    std::cout << "empty\n";
                else {
                    auto it = _type_args.begin();
                    while (it != _type_args.end() - 1) {
                        std::cout << (*it)->name() << ", ";
                        it++;
                    }
                    std::cout << (*it)->name() << "\n";
                }
                std::exit(1);
            }
        }
        else {
            std::cout << "\nFailed calling function!\n";
            std::cout << "Reason: This variable is not a void function or a return function!\n";
            std::cout << "Support return function: var (UniversalVariable). Please check using `isCallFunction()`\n";
            std::exit(1);
        }
        return 0;
    }
    bool isCallFunction()
    {
        return __GET_FLAG(_flags, __IS_CALLABLE);
    }

#pragma endregion

#pragma region all_data

private:
    //Info of UniversalVariable
    unsigned char _flags = 0;
    unsigned int _type;
    unsigned int _unknown_type;
    unsigned int _size_of_allocated_data = 0;

    //Info of data
    union { long double _real; void* _pointer; } _data;
    std::string _text;
    std::vector<UniversalVariable> _array;
    void* _allocated_data;

    //Info of data pointer callable function
    unsigned int _count_args;
    std::vector<const std::type_info*> _type_args = {  };
    
    //Access friend
    friend std::ostream& operator<<(std::ostream& out, const UniversalVariable& variable);
    friend std::ostream& operator<<(std::ostream& out, const std::vector<UniversalVariable>& data);

#pragma endregion

};

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
std::ostream& showHexData(std::ostream& out, const void* pointer, unsigned int size)
{
    __count_tab++;
    out << "Data: {\n";

    unsigned char hex[3]{};
    unsigned int i = 0;
    while (i < size) {
        for (decltype(__count_tab) i = 0; i < __count_tab; i++)
            out << "    ";
        for (unsigned int j = 0; i < size, j < 4; i++, j++) {
            hex[0] = (0xF & ((const char*)pointer)[i] >> 4) + 48;
            hex[1] = (0xF & ((const char*)pointer)[i]) + 48;
            if (hex[0] > 57) hex[0] += 7;
            if (hex[1] > 57) hex[1] += 7;
            out << hex;
        }
        out << ((i < size - 1) ? (",\n") : ("\n"));
    }

    __count_tab--;
    for (decltype(__count_tab) i = 0; i < __count_tab; i++)
        out << "    ";
    out << "}";

    return out;
}
std::ostream& operator<<(std::ostream& out, const UniversalVariable& variable)
{
    return (
        (variable._type == __REAL) ? (out << variable._data._real) : 
        (variable._type == __TEXT) ? (out << variable._text) : 
        (variable._type == __ARRAY) ? (out << variable._array) :
        (variable._type == __POINTER) ? (out << variable._data._pointer) :
        (showHexData(out, variable._allocated_data, variable._size_of_allocated_data) << " (" << __type_names[variable._unknown_type]->name() << ")")
    );
}
std::ostream& operator<<(std::ostream& out, const std::vector<UniversalVariable>& data)
{
    __count_tab++;
    out << "List: {\n";

    for (auto it = data.begin(); it != data.end(); it++) {
        for (decltype(__count_tab) i = 0; i < __count_tab; i++)
            out << "    ";
        out << ((it->_type == __TEXT) ? ("\"") : (""))
            << *it
            << ((it->_type == __TEXT) ? ("\"") : (""))
            << ((it != data.end() - 1) ? (",\n") : ("\n"));
    }

    __count_tab--;
    for (decltype(__count_tab) i = 0; i < __count_tab; i++)
        out << "    ";
    out << "}";
    
    return out;
}

#pragma endregion













struct ManyInt
{
    int a;
    int b;
    int c;
    int d; 
};

//MAIN
int main()
{
    ManyInt value = { 0x12345678, 0x10101010, 0x00FFAA88, 96 };

    var a = 5;
    var b = { value };
    a = b;

    std::cout << a;

    return 0;
}
