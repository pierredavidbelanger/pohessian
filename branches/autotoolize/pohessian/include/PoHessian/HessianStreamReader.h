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

#ifndef PoHessian_HessianStreamReader_INCLUDED
#define PoHessian_HessianStreamReader_INCLUDED

#include <istream>
#include <vector>

#include "PoHessian/PoHessian.h"
#include "PoHessian/HessianTypes.h"

namespace PoHessian {

    class PoHessian_API HessianStreamReader {
    public:
        
        virtual ValuePtr readValue() = 0;
        virtual CallPtr readCall() = 0;
        virtual ReplyPtr readReply() = 0;

    protected:
        
        HessianStreamReader(std::istream& in);
        
        std::istream& _in;
        RefList _refs;
    };

}

#endif
