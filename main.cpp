// HEADER
#define UNIVERSAL_VARIABLE_EXPERIMENTAL_MODE
#define UNIVERSAL_VARIABLE_ONLY_HEADER

#pragma region check_compiler
// O------------------------------------------------------------------------------O
// | If the header support your compiler, you can use UniversalVariable           |
// O------------------------------------------------------------------------------O
#ifndef __cplusplus
  #error Support only C++!
#endif

#undef CXX_MSC
#undef CXX_CLANG
#undef CXX_GNU
#undef CXX_MAKE_VER

#define CXX_MSC      0
#define CXX_CLANG    0
#define CXX_GNU      0
#define CXX_MAKE_VER(MAJOR, MINOR, PATCH) ((MAJOR) * 10000000 + (MINOR) * 100000 + (PATCH))

#if defined(_MSC_VER) && defined(_MSC_FULL_VER)
  #undef CXX_MSC
  #if _MSC_VER == _MSC_FULL_VER / 10000
    #define CXX_MSC CXX_MAKE_VER(_MSC_VER / 100, _MSC_VER % 100, _MSC_FULL_VER % 10000)
  #else
    #define CXX_MSC CXX_MAKE_VER(_MSC_VER / 100, (_MSC_FULL_VER / 100000) % 100, _MSC_FULL_VER % 100000)
  #endif
#elif defined(__clang__) && defined(__clang_minor__)
  #undef CXX_CLANG
  #define CXX_CLANG CXX_MAKE_VER(__clang_major__, __clang_minor__, __clang_patchlevel__)
#elif defined(__GNUC__) && defined(__GNUC_MINOR__) && defined(__GNUC_PATCHLEVEL__)
  #undef CXX_GNU
  #define CXX_GNU CXX_MAKE_VER(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#else
  #error Alas, your compiler unsupported. Supported compilers: MSVC, Clang and GNU.
#endif

#if CXX_MSC
  #if CXX_MSC < CXX_MAKE_VER(19, 15, 0)
    #error Alas, your compiler is so old. Please update compiler. Supported version: v19.15 and newer.
  #elif _MSVC_LANG < 201703L
    #error Alas, your standart C++ unsupported. Supported standard C++: C++17 and newer.
  #endif
  #pragma warning(push)
  #pragma warning(disable: 4455)
#elif CXX_CLANG
  #if CXX_CLANG < CXX_MAKE_VER(7, 0, 0)
    #error Alas, your compiler is so old. Please update compiler. Supported version: 7.0.0 and newer.
  #elif __cplusplus < 201703L
    #error Alas, your standart C++ unsupported. Supported standard C++: C++17 and newer.
  #endif
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wreturn-type"
  #pragma clang diagnostic ignored "-Wunused-comparison"
  #pragma clang diagnostic ignored "-Wuser-defined-literals"
#elif CXX_GNU
  #if CXX_GNU < CXX_MAKE_VER(7, 1, 0)
    #error Alas, your compiler is so old. Please update compiler. Supported version: 7.1 and newer.
  #elif __cplusplus < 201703L
    #error Alas, your standart C++ unsupported. Supported standard C++: C++17 and newer.
  #endif
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wliteral-suffix"
#endif
#pragma endregion

#pragma region std_includes
// O------------------------------------------------------------------------------O
// | STANDARD INCLUDES                                                            |
// O------------------------------------------------------------------------------O
#include <vector>
#include <variant>
#include <cstring>
#include <sstream>
#include <iostream>
#include <stdint.h>
#include <typeinfo>
#include <algorithm>
#include <type_traits>
#include <initializer_list>
#pragma endregion

// O------------------------------------------------------------------------------O
// | uv::UniversalVariable INTERFACE DECLARATION                                  |
// O------------------------------------------------------------------------------O
#pragma region uv_declaration
namespace uv
{
  typedef std::size_t size_uv;

  class UniversalVariable;

  UniversalVariable operator "" v(long double data);
  UniversalVariable operator "" v(unsigned long long int data);
  UniversalVariable operator "" v(const char* str, std::size_t len);

  void printType(const UniversalVariable& variable);
  std::ostream& operator<<(std::ostream& out, const UniversalVariable& variable);
  std::ostream& operator<<(std::ostream& out, const std::vector<UniversalVariable>& array);

#ifdef UNIVERSAL_VARIABLE_EXPERIMENTAL_MODE
  template<typename T> T get(const UniversalVariable& variable);
#endif

  namespace metaprogramming_declaration
  {
    using supported_types = std::variant<long double, std::string, std::vector<UniversalVariable>, void*>;
  }

  // O------------------------------------------------------------------------------O
  // | uv::UniversalVariable - A dynamic and implicit typing variable               |
  // O------------------------------------------------------------------------------O
  class UniversalVariable
  {
  public:
    UniversalVariable();
    UniversalVariable(const UniversalVariable& variable);
    UniversalVariable(const std::initializer_list<UniversalVariable>& array);
    template<typename T> UniversalVariable(const T& data);
    ~UniversalVariable();

  public:
    bool isSameType(const UniversalVariable& variable);
    template<typename T> bool isSameType(const T& data);
  public:
    UniversalVariable& operator=(const UniversalVariable& variable);
    template<typename T> UniversalVariable& operator=(const T& data);

  public:
    UniversalVariable& operator[](size_uv index);
    size_uv size();

  public:
    bool isCallable();
    template<typename... Args> UniversalVariable operator()(Args... args);
  private:
    template<typename R> void _writeArgs(R f());
    template<typename R, typename First, typename... Rest> void _writeArgs(R f(First, Rest...));
    void _readArgs();
    template<typename First> void _readArgs(First first);
    template<typename First, typename... Rest> void _readArgs(First first, Rest... rest);

  public:
    unsigned int _flags = 0;
    size_uv _type;
    size_uv _type_from_list_types;
    size_uv _size_of_allocated_data = 0;
    metaprogramming_declaration::supported_types _data;

    size_uv _count_args;
    std::vector<const std::type_info*> _list_args;

    friend void printType(const UniversalVariable& variable);
    friend std::ostream& operator<<(std::ostream& out, const UniversalVariable& variable);
    friend std::ostream& operator<<(std::ostream& out, const std::vector<UniversalVariable>& array);

  #ifdef UNIVERSAL_VARIABLE_EXPERIMENTAL_MODE
    template<typename T> friend T get(const UniversalVariable& variable);
  #endif
  };
}

typedef class uv::UniversalVariable var;
#pragma endregion

#ifdef UNIVERSAL_VARIABLE_ONLY_HEADER
// O------------------------------------------------------------------------------O
// | uv::UniversalVariable INTERNAL DECLARATION FOR SINGLE HEADER                 |
// O------------------------------------------------------------------------------O
#pragma region uv_internal_declaration
namespace uv
{
  namespace internal_declaration
  {
    enum {
      REAL         = 0,
      TEXT         = 1,
      ARRAY        = 2,
      POINTER      = 3,
      UNKNOWN_TYPE = 4,
    };
    enum {
      IS_CALLABLE_FUNCTION = 0x01,
      IS_RETURN_FUNCTION   = 0x02,
    };
    static size_uv index_arg = 0;
    static bool is_same_type_arg = 0;
    static std::vector<const std::type_info*> list_types = { };

    static size_uv count_indent = 0;

    std::ostream& showHexData(std::ostream& out, const void* pointer, unsigned int size);
    std::ostream& showListArgs(std::ostream& out, const std::vector<const std::type_info*>& list_args);

    void error_failed_indexing_out_range();
    void error_failed_indexing_not_array();
    void error_failed_call_different_args(const std::vector<const std::type_info*>& list_args);
    void error_failed_call_not_match_count_args(const std::vector<const std::type_info*>& list_args);
    void error_failed_call_unsupported_return_type();

  #ifdef UNIVERSAL_VARIABLE_EXPERIMENTAL_MODE
    std::stringstream str;
    template<typename T> std::string to_string(const T& data);
    void error_failed_getting_not_match_type();
  #endif
  }
  namespace metaprogramming_declaration
  {
    inline bool getFlag(unsigned int flags, unsigned int flag) { return flags & flag; }
    inline void setFlag(unsigned int& flags, unsigned int flag, bool toggle) { toggle ? flags |= flag : flags &= ~flag; }

    template<typename T, typename... Args> struct is_same_type : std::bool_constant<(std::is_same_v<T, Args> || ...)> {};
    template<typename T, typename... Args> inline constexpr bool is_same_type_v = is_same_type<T, Args...>::value;

    template<typename T> struct get_type_return { using type = T; };
    template<typename R, typename... Args> struct get_type_return<R(Args...)> { using type = R; };
    template<typename R, typename... Args> struct get_type_return<R(*)(Args...)> { using type = R; };
    template<typename T> using get_type_return_t = typename get_type_return<T>::type;

    template<typename T> inline constexpr bool is_arithmetic_v =
      std::is_arithmetic_v<T>;
    template<typename T> inline constexpr bool is_bool_v =
      is_same_type_v<T, bool>;
    template<typename T> inline constexpr bool is_symbol_v =
      is_same_type_v<T, char, unsigned char>;
    template<typename T> inline constexpr bool is_real_v =
      is_arithmetic_v<T> && !is_bool_v<T> && !is_symbol_v<T>;
    template<typename T> inline constexpr bool is_text_v =
      is_same_type_v<T, char[sizeof(get_type_return_t<T>)], const char*>;
    template<typename T> inline constexpr bool is_std_text_v =
      is_same_type_v<T, std::string>;
    template<typename T> inline constexpr bool is_array_v =
      std::is_array_v<T>;
    template<typename T> inline constexpr bool is_std_array_v =
      is_same_type_v<T, std::vector<UniversalVariable>, std::initializer_list<UniversalVariable>>;
    template<typename T> inline constexpr bool is_function_v =
      std::is_function_v<typename std::remove_pointer_t<T>>;
    template<typename T> inline constexpr bool is_return_callback_v =
      is_same_type_v<get_type_return_t<T>, UniversalVariable>;
    template<typename T> inline constexpr bool is_supported_callback_v =
      is_same_type_v<get_type_return_t<T>, void, UniversalVariable>;
    template<typename T> inline constexpr bool is_pointer_v =
      std::is_pointer_v<T>;
  }
}
#pragma endregion

// O------------------------------------------------------------------------------O
// | uv::UniversalVariable INTERFACE IMPLEMENTATION FOR SINGLE HEADER             |
// O------------------------------------------------------------------------------O
#pragma region uv_implementation
namespace uv
{
  namespace id = internal_declaration;
  namespace md = metaprogramming_declaration;

  UniversalVariable::UniversalVariable()
  {
    *this = "NULL";
  }
  UniversalVariable::UniversalVariable(const UniversalVariable& variable)
  {
    *this = variable;
  }
  UniversalVariable::UniversalVariable(const std::initializer_list<UniversalVariable>& array)
  {
    *this = array;
  }
  template<typename T> UniversalVariable::UniversalVariable(const T& data)
  {
    *this = data;
  }
  UniversalVariable::~UniversalVariable()
  {
    if (this->_size_of_allocated_data > 0) {
      free(std::get<id::POINTER>(this->_data));
    }
  }

  bool UniversalVariable::isSameType(const UniversalVariable& variable)
  {
    const bool is_same_type = this->_type == variable._type;
    const bool is_same_unknown_type = this->_type_from_list_types == variable._type_from_list_types;
    return is_same_type && (this->_type == id::UNKNOWN_TYPE ? is_same_unknown_type : 1);
  }
  template<typename T> bool UniversalVariable::isSameType(const T& data)
  {
    if constexpr (md::is_real_v<T>) {
      return this->_type == id::REAL;
    } else if constexpr (md::is_bool_v<T>) {
      return this->_type == id::TEXT && (std::get<id::TEXT>(this->_data) == "true" || std::get<id::TEXT>(this->_data) == "false");
    } else if constexpr (md::is_symbol_v<T> || md::is_text_v<T> || md::is_std_text_v<T>) {
      return this->_type == id::TEXT;
    } else if constexpr (md::is_array_v<T> || md::is_std_array_v<T>) {
      return this->_type == id::ARRAY;
    } else {
      auto search = std::find(id::list_types.begin(), id::list_types.end(), &typeid(data));
      return search != id::list_types.end() && this->_type_from_list_types == search - id::list_types.begin();
    }
  }

  UniversalVariable& UniversalVariable::operator=(const UniversalVariable& variable)
  {
    this->_flags = variable._flags;
    this->_type = variable._type;
    this->_type_from_list_types = variable._type_from_list_types;
    if (this->_size_of_allocated_data > 0) {
      free(std::get<id::POINTER>(this->_data));
    }
    this->_size_of_allocated_data = variable._size_of_allocated_data;
    if (this->_size_of_allocated_data > 0) {
      std::get<id::POINTER>(this->_data) = malloc(this->_size_of_allocated_data);
      memcpy(std::get<id::POINTER>(this->_data), std::get<id::POINTER>(variable._data), this->_size_of_allocated_data );
    } else {
      this->_data = variable._data;
    }
    this->_count_args = variable._count_args;
    this->_list_args = variable._list_args;
    return *this;
  }
  template<typename T> UniversalVariable& UniversalVariable::operator=(const T& data)
  {
    md::setFlag(this->_flags, id::IS_CALLABLE_FUNCTION, false);
    if (this->_size_of_allocated_data > 0) {
      free(std::get<id::POINTER>(this->_data));
      this->_size_of_allocated_data = 0;
    }

    if constexpr (md::is_real_v<T>) {
      this->_type = id::REAL;
      std::variant_alternative_t<id::REAL, md::supported_types> temp = data;
      this->_data = temp;
    } else if constexpr (md::is_bool_v<T>) {
      this->_type = id::TEXT;
      std::variant_alternative_t<id::TEXT, md::supported_types> temp = data ? "true" : "false";
      this->_data = temp;
    } else if constexpr (md::is_symbol_v<T> || md::is_text_v<T> || md::is_std_text_v<T>) {
      this->_type = id::TEXT;
      std::variant_alternative_t<id::TEXT, md::supported_types> temp = data;
      this->_data = temp;
    } else if constexpr (md::is_array_v<T>) {
      this->_type = id::ARRAY;
      std::variant_alternative_t<id::ARRAY, md::supported_types> temp = { };
      this->_data = temp;
      for (size_uv i = 0; i < sizeof(T) / sizeof(std::remove_extent_t<T>); i++) {
        std::get<id::ARRAY>(this->_data).push_back(data[i]);
      }
    } else if constexpr (md::is_std_array_v<T>) {
      this->_type = id::ARRAY;
      std::variant_alternative_t<id::ARRAY, md::supported_types> temp = data;
      this->_data = temp;
    } else if constexpr (md::is_function_v<T>) {
      md::setFlag(this->_flags, id::IS_RETURN_FUNCTION, md::is_return_callback_v<T>);
      md::setFlag(this->_flags, id::IS_CALLABLE_FUNCTION, md::is_supported_callback_v<T>);
      if constexpr (md::is_supported_callback_v<T>) {
        this->_count_args = 0;
        this->_list_args.clear();
        _writeArgs(data);
      }
      auto search = std::find(id::list_types.begin(), id::list_types.end(), &typeid(typename std::remove_pointer_t<T>));
      if (search == id::list_types.end()) {
        id::list_types.push_back(&typeid(typename std::remove_pointer_t<T>));
        search = id::list_types.end() - 1;
      }
      this->_type = id::POINTER;
      this->_type_from_list_types = search - id::list_types.begin();
      this->_data = reinterpret_cast<void*>(data);
    } else {
      auto search = std::find(id::list_types.begin(), id::list_types.end(), &typeid(data));
      if (search == id::list_types.end()) {
        id::list_types.push_back(&typeid(data));
        search = id::list_types.end() - 1;
      }
      this->_type_from_list_types = search - id::list_types.begin();
      if constexpr (md::is_pointer_v<T>) {
        this->_type = id::POINTER;
        this->_data = reinterpret_cast<void*>(data);
      } else {
        this->_type = id::UNKNOWN_TYPE;
        this->_size_of_allocated_data = sizeof(T);
        this->_data = malloc(sizeof(T));
        memcpy(std::get<id::POINTER>(this->_data), &data, sizeof(T));
      }
    }
    return *this;
  }

  UniversalVariable& UniversalVariable::operator[](size_uv index)
  {
    if (this->_type == id::ARRAY || this->_type == id::TEXT) {
      if (index < std::get<id::ARRAY>(this->_data).size()) {
        return std::get<id::ARRAY>(this->_data)[index];
      } else {
        id::error_failed_indexing_out_range();
      }
    } else if (index == 0) {
      return *this;
    } else {
      id::error_failed_indexing_not_array();
    }
  }
  size_uv UniversalVariable::size()
  {
    if (this->_type == id::ARRAY) {
      return std::get<id::ARRAY>(this->_data).size();
    } else {
      return 1;
    }
  }

  template<typename... Args> UniversalVariable UniversalVariable::operator()(Args... args)
  {
    if (md::getFlag(this->_flags, id::IS_CALLABLE_FUNCTION)) {
      if (sizeof...(args) == this->_count_args) {
        id::is_same_type_arg = 1;
        if (this->_count_args != 0) {
          id::index_arg = 0;
          _readArgs(args...);
        }

        if (id::is_same_type_arg == true) {
          if (md::getFlag(this->_flags, id::IS_RETURN_FUNCTION)) {
            return ((UniversalVariable(*)(Args...))std::get<id::POINTER>(this->_data))(args...);
          } else {
            ((void(*)(Args...))std::get<id::POINTER>(this->_data))(args...);
          }
        } else {
          id::error_failed_call_different_args(this->_list_args);
        }
      } else {
        id::error_failed_call_not_match_count_args(this->_list_args);
      }
    } else {
      id::error_failed_call_unsupported_return_type();
    }
  }
  bool UniversalVariable::isCallable()
  {
    return md::getFlag(this->_flags, id::IS_CALLABLE_FUNCTION);
  }
  template<typename R> void UniversalVariable::_writeArgs(R f()) { }
  template<typename R, typename First, typename... Rest> void UniversalVariable::_writeArgs(R f(First, Rest...))
  {
    this->_list_args.push_back(&typeid(First));
    this->_count_args++;
    R (*temp)(Rest...);
    _writeArgs(temp);
  }
  void UniversalVariable::_readArgs() { }
  template<typename First> void UniversalVariable::_readArgs(First first)
  {
    id::is_same_type_arg = (&typeid(first) == this->_list_args[id::index_arg]);
  }
  template<typename First, typename... Rest> void UniversalVariable::_readArgs(First first, Rest... rest)
  {
    id::is_same_type_arg = (&typeid(first) == this->_list_args[id::index_arg]);
    id::index_arg++;
    if (id::is_same_type_arg) {
      _readArgs(rest...);
    }
  }

  UniversalVariable operator "" v(long double data)
  {
    return UniversalVariable(data);
  }
  UniversalVariable operator "" v(unsigned long long int data)
  {
    return UniversalVariable(data);
  }
  UniversalVariable operator "" v(const char* str, std::size_t len)
  {
    return UniversalVariable(str);
  }

  void printType(const UniversalVariable& variable)
  {
    std::cout << "\nThis variable is ";
    switch (variable._type) {
    case id::REAL:
      std::cout << "number\n";
      break;
    case id::TEXT:
      std::cout << "text\n";
      break;
    case id::ARRAY:
      std::cout << "array (list)\n";
      break;
    case id::POINTER:
      std::cout << "pointer (" << id::list_types[variable._type_from_list_types]->name() << ")\n";
      break;
    default:
      std::cout << "unknown type (" << id::list_types[variable._type_from_list_types]->name() << ")\n";
    }
  }
  std::ostream& operator<<(std::ostream& out, const UniversalVariable& variable)
  {
    switch (variable._type) {
    case id::REAL:
      return out << std::get<id::REAL>(variable._data);
    case id::TEXT:
      return out << std::get<id::TEXT>(variable._data);
    case id::ARRAY:
      return out << std::get<id::ARRAY>(variable._data);
    case id::POINTER:
      return out << std::get<id::POINTER>(variable._data);
    default:
      out << id::list_types[variable._type_from_list_types]->name() << " (" << variable._size_of_allocated_data << "bytes) = ";
      return id::showHexData(out, std::get<id::POINTER>(variable._data), variable._size_of_allocated_data);
    }
  }
  std::ostream& operator<<(std::ostream& out, const std::vector<UniversalVariable>& data)
  {
    id::count_indent++;
    out << "List (" << data.size() << " elements): [\n";
    for (auto it = data.begin(); it != data.end(); it++) {
      for (size_uv i = 0; i < id::count_indent; i++) {
        out << "  ";
      }
      out << ((it->_type == id::TEXT) ? ("\"") : ("")) << *it << ((it->_type == id::TEXT) ? ("\"") : ("")) << ((it != data.end() - 1) ? (",\n") : ("\n"));
    }
    id::count_indent--;
    for (size_uv i = 0; i < id::count_indent; i++) {
      out << "  ";
    }
    return out << "]";
  }
  std::ostream& id::showHexData(std::ostream& out, const void* pointer, unsigned int size)
  {
    id::count_indent++;
    out << "{\n";
    unsigned char hex[3]{};
    unsigned int i = 0;
    while (i < size) {
      for (size_uv i = 0; i < id::count_indent; i++) {
        out << "  ";
      }
      out << "0x";
      for (unsigned int j = 0; i < size && j < 4; i++, j++) {
        hex[0] = (0xF & ((const char*)pointer)[i] >> 4) + 48;
        hex[1] = (0xF & ((const char*)pointer)[i]) + 48;
        if (hex[0] > 57) hex[0] += 7;
        if (hex[1] > 57) hex[1] += 7;
        out << hex;
      }
      out << ((i < size - 1) ? (",\n") : ("\n"));
    }
    id::count_indent--;
    for (size_uv i = 0; i < id::count_indent; i++) {
      out << "  ";
    }
    return out << "}";
  }
  std::ostream& id::showListArgs(std::ostream& out, const std::vector<const std::type_info*>& list_args)
  {
    if (list_args.size() == 0) {
      return out << "empty\n";
    } else {
      auto it = list_args.begin();
      while (it != list_args.end() - 1) {
        out << (*it)->name() << ", ";
        it++;
      }
      return out << (*it)->name() << "\n";
    }
  }

  void id::error_failed_indexing_out_range()
  {
    std::cout << "\nFailed indexing!\n";
    std::cout << "Reason: Index out of array range!\n";
    std::cout << "Please check size of array using `size()`\n";
    std::exit(1);
  }
  void id::error_failed_indexing_not_array()
  {
    std::cout << "\nFailed indexing!\n";
    std::cout << "Reason: This variable is not an array!\n";
    std::cout << "Please use index zero, it will work\n";
    std::exit(1);
  }
  void id::error_failed_call_different_args(const std::vector<const std::type_info*>& list_args)
  {
    std::cout << "\nFailed calling function!\n";
    std::cout << "Reason: Different type of arguments passing!\n";
    std::cout << "Arguments function: ";
    id::showListArgs(std::cout, list_args);
    std::exit(1);
  }
  void id::error_failed_call_not_match_count_args(const std::vector<const std::type_info*>& list_args)
  {
    std::cout << "\nFailed calling function!\n";
    std::cout << "Reason: Count of arguments does not match!\n";
    std::cout << "Arguments function: ";
    id::showListArgs(std::cout, list_args);
    std::exit(1);
  }
  void id::error_failed_call_unsupported_return_type()
  {
    std::cout << "\nFailed calling function!\n";
    std::cout << "Reason: This variable is not a void function or a return function!\n";
    std::cout << "Support return function: var (UniversalVariable). Please check using `isCallFunction()`\n";
    std::exit(1);
  }

#ifdef UNIVERSAL_VARIABLE_EXPERIMENTAL_MODE
  template<typename T> std::string id::to_string(const T& data)
  {
    id::str.str("");
    if constexpr (md::is_bool_v<T> || md::is_symbol_v<T> || md::is_real_v<T> || md::is_text_v<T>)
      id::str << data;
    else if constexpr (md::is_pointer_v<T>)
      id::str << std::hex << data;
    else id::str << &data << "(" << typeid(data).name() << ")";
    return id::str.str();
  }
  template<typename T> T get(const UniversalVariable& data)
  {
    if constexpr (md::is_real_v<T>) {
      if (data._type == id::REAL) {
        return static_cast<T>(std::get<id::REAL>(data._data));
      } else if (data._type == id::TEXT) {
        try {
          return static_cast<T>(std::stold(std::get<id::TEXT>(data._data)));
        } catch (std::invalid_argument const& ex) {
          return static_cast<T>(0);
        }
      } else if (data._type == id::ARRAY) {
        return get<T>(std::get<id::ARRAY>(data._data)[0]);
      } else if (data._type == id::POINTER) {
        return static_cast<T>(reinterpret_cast<uintptr_t>(std::get<id::POINTER>(data._data)));
      } else id::error_failed_getting_not_match_type();
    } else if constexpr (md::is_bool_v<T>) {
      if (data._type == id::REAL) {
        return static_cast<T>(std::get<id::REAL>(data._data));
      } else if (data._type == id::TEXT) {
        return std::get<id::TEXT>(data._data) == "true" || static_cast<T>(std::stold(std::get<id::TEXT>(data._data)));
      } else if (data._type == id::ARRAY) {
        return get<T>(std::get<id::ARRAY>(data._data)[0]);
      } else if (data._type == id::POINTER) {
        return static_cast<T>(reinterpret_cast<uintptr_t>(std::get<id::POINTER>(data._data)));
      } else id::error_failed_getting_not_match_type();
    } else if constexpr (md::is_symbol_v<T>) {
      if (data._type == id::REAL) {
        return id::to_string(std::get<id::REAL>(data._data))[0];
      } else if (data._type == id::TEXT) {
        return std::get<id::TEXT>(data._data)[0];
      } else if (data._type == id::ARRAY) {
        return get<T>(std::get<id::ARRAY>(data._data)[0]);
      } else if (data._type == id::POINTER) {
        return id::to_string(std::get<id::POINTER>(data._data))[0];
      } else id::error_failed_getting_not_match_type();
    } else if constexpr (md::is_std_text_v<T>) {
      if (data._type == id::REAL) {
        return id::to_string(std::get<id::REAL>(data._data));
      } else if (data._type == id::TEXT) {
        return std::get<id::TEXT>(data._data);
      } else if (data._type == id::ARRAY) {
        return get<T>(std::get<id::ARRAY>(data._data)[0]);
      } else if (data._type == id::POINTER) {
        return id::to_string(std::get<id::POINTER>(data._data));
      } else id::error_failed_getting_not_match_type();
    } else if constexpr (md::is_std_array_v<T>) {
      return std::get<id::ARRAY>(data._data);
    } else if constexpr (md::is_function_v<T> || md::is_pointer_v<T>) {
      return static_cast<T>(std::get<id::POINTER>(data._data));
    } else {
      std::cout << "\nFailed getting value from variable!\n";
      std::exit(1);
    }
  }
  void id::error_failed_getting_not_match_type()
  {
    std::cout << "\nFailed getting value from variable!\n";
    std::cout << "Reason: Not match type of variable!\n";
    std::cout << "Please check type using `showType()`\n";
    std::exit(1);
  }
#endif
}
#pragma endregion
#endif

#if CXX_MSC
  #pragma warning(pop)
#elif CXX_CLANG
  #pragma clang diagnostic pop
#elif CXX_GNU
  #pragma GCC diagnostic pop
#endif

#undef CXX_MSC
#undef CXX_CLANG
#undef CXX_GNU
#undef CXX_MAKE_VER






var test()
{
  return "Hello world!";
}

struct List {
  int a;
  int b;
};

//MAIN
int main()
{
  List list = { 0x7FFFFFFF, 123456789 };

  var Null;
  var Array = { 0, 1, Null, 2, { "Text", Null, 4 }, 5 };
  var CopyArray = Array[4];
  var Real = Array[4][2];
  var Copy = Null;
  var Text = "true";
  var Current = Array;
  var Fun = test;

  Array[4][0] = list;

  std::cout << Array << "\n";

  return 0;
}
