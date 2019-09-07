#include <string>
#include <initializer_list>
typedef std::initializer_list<std::string> StrArgs;

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

template <typename S>
auto checkStrExists(S &match, S &str) -> bool
{
    std::string::size_type sType;
    sType = str.find(match);

    return sType != std::string::npos ? true : false;
}

template <typename S>
auto remAmpersand(S str) -> S
{
    const char &last = str.back();
    if (last == '&')
        str.pop_back();

    return str;
}