//
// PoHessian
// Portable C++ Hessian Implementation
//
// Copyright (C) 2012  Pierre-David Belanger
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <sstream>

#include "hessian_tests.h"

#include "pohessian/HessianTypes.h"
#include "pohessian/HessianClient.h"
#include "pohessian/Hessian1StreamWriter.h"

#include "Poco/SharedPtr.h"

using namespace PoHessian;

int main(int argc, char* argv[]) {

    std::string server = "remote";
    if (argc > 1)
        server = argv[1];
    int ret = 0;
    if (server == "remote") {
        HessianClient client(HessianClient::HESSIAN_VERSION_1, Poco::URI("http://hessian.caucho.com/test/test2"));
        ret += hessian_test2(client);
    } else {
        HessianClient client(HessianClient::HESSIAN_VERSION_1, Poco::URI("http://localhost:8181/test/test"));
        ret += hessian_test(client);
        HessianClient client2(HessianClient::HESSIAN_VERSION_1, Poco::URI("http://localhost:8181/test/test2"));
        ret += hessian_test2(client2);
        HessianClient client3(HessianClient::HESSIAN_VERSION_1, Poco::URI("tcp://localhost:9191"));
        ret += hessian_test(client3);
    }
    return ret == 0 ? 0 : -1;
}
