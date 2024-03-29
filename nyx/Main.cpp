// MIT License
//
// Copyright (c) 2018-2023 y1yang0 <kelthuzadx@qq.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "Debug.hpp"
#include "Interpreter.h"
#include "Utils.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        panic("Feed your *.nyx source file to interpreter!\n");
    }

    auto* rt = new Runtime;

    Parser parser(argv[1]);
#if NYX_DEBUG
    printLex(argv[1]);
#endif
    parser.parse(rt);
    Interpreter nyx;
    nyx.execute(rt);

    return 0;
}
