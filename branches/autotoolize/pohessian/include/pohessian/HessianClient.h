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
// Contributor(s): Pierre-David Belanger <pierredavidbelanger@gmail.com>
//

#ifndef PoHessian_HessianClient_INCLUDED
#define PoHessian_HessianClient_INCLUDED

#include <string>
#include <vector>
#include <ostream>

#include "pohessian/PoHessian.h"
#include "pohessian/HessianTypes.h"

#include "Poco/URI.h"

namespace PoHessian {

    class PoHessian_API HessianClientImpl;
    
    class PoHessian_API HessianClient {
    public:
        
        enum HessianVersion {
            HESSIAN_VERSION_1
        };
        
        HessianClient(const HessianVersion version, const Poco::URI& uri);
        
        ValuePtr call(const std::string& method);
        ValuePtr call(const std::string& method, const HeaderList& headers);
        ValuePtr call(const std::string& method, const ParameterList& parameters);
        ValuePtr call(const std::string& method, const HeaderList& headers, const ParameterList& parameters);
        ReplyPtr call(const CallPtr& call);
        
    protected:
        const HessianVersion _version;
        const Poco::URI _uri;
    };

}

#endif
