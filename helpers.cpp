/**
 * @file helpers.cpp
 * @author Lochemem Bruno Michael (lochbm@gmail.com)
 * @brief 
 * @version 0.1.0
 */
#include <string>
#include <initializer_list>
typedef std::initializer_list<std::string> StrArgs;

/**
 * @brief Concatenate strings
 * 
 * @tparam S 
 * @tparam L 
 * @param glue 
 * @param words 
 * @return S 
 */
template <typename S, typename L>
auto concat(S glue, L words) -> S
{
    std::string retval = "";
    for (auto &iter : words)
    {
        retval += iter + glue;
    }
    if (!glue.empty())
        retval.pop_back();

    return retval;
}

/**
 * @brief Check if a string exists in another string
 * 
 * @tparam S 
 * @param match 
 * @param str 
 * @return true 
 * @return false 
 */
template <typename S>
auto checkStrExists(S &match, S &str) -> bool
{
    std::string::size_type sType;
    sType = str.find(match);

    return sType != std::string::npos ? true : false;
}

/**
 * @brief 
 * 
 * @tparam S 
 * @param str 
 * @return S 
 */
template <typename S>
auto remAmpersand(S str) -> S
{
    const char &last = str.back();
    if (last == '&')
        str.pop_back();

    return str;
}
