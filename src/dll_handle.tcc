/*
 * Created by Anthony Ilareguy on 2020-05-10.
 * Copyright 2020 Anthony Ilareguy <anthony at ilareguy dot com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <dlfcn.h>
#include <fmt/format.h>
#include <typeinfo>

template<typename T>
T mafia::DLLHandle::get(std::string_view symbol, bool mandatory)
{
    auto sym_address = dlsym(_raw_handle, symbol.data());
    if (mandatory && nullptr == sym_address)
    {
        throw SymbolNotFound {
                fmt::format(R"(Mandatory symbol "{}" with type "{}" not found.)", symbol, typeid(T).name())
        };
    }
    return reinterpret_cast<T>(sym_address);
}
