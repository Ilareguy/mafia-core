/********************************************************
 *
 *             ______  ____    ______   ______
 *     /'\_/`\/\  _  \/\  _`\ /\__  _\ /\  _  \
 *    /\      \ \ \L\ \ \ \L\_\/_/\ \/ \ \ \L\ \
 *    \ \ \__\ \ \  __ \ \  _\/  \ \ \  \ \  __ \
 *     \ \ \_/\ \ \ \/\ \ \ \/    \_\ \__\ \ \/\ \
 *      \ \_\\ \_\ \_\ \_\ \_\    /\_____\\ \_\ \_\
 *       \/_/ \/_/\/_/\/_/\/_/    \/_____/ \/_/\/_/
 *
 *                  The Mafia project
 *
 ********************************************************
 *
 * File created by Anthony Ilareguy on 21/03/2021.
 * [File Description]
 *
 ********************************************************
 * 
 * [Credits]
 *
 ********************************************************/

#ifndef DEF_MAFIA_CORE_PAIR_HASH_H
#define DEF_MAFIA_CORE_PAIR_HASH_H

template<typename T, typename U>
std::size_t pair_hash(const T& first, const U& second)
{
    size_t _hash = std::hash<T>()(first);
    _hash ^= std::hash<U>()(second) + 0x9e3779b9 + (_hash << 6) + (_hash >> 2);
    return _hash;
}

#endif // DEF_MAFIA_CORE_PAIR_HASH_H
