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

#ifndef pohessian_Hessian1StreamWriter_INCLUDED
#define pohessian_Hessian1StreamWriter_INCLUDED

#include <ostream>

#include "pohessian/PoHessian.h"
#include "pohessian/HessianTypes.h"
#include "pohessian/HessianStreamWriter.h"

namespace PoHessian {

    class PoHessian_API Hessian1StreamWriter : public HessianStreamWriter {
    public:
        
        Hessian1StreamWriter(std::ostream& out);
        
        void writeValue(const ValuePtr& value);
        void writeCall(const CallPtr& call);
        void writeReply(const ReplyPtr& reply);

    };

}

#endif
