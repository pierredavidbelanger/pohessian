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

package com.google.code.pohessian.test;

import com.caucho.hessian.test.Test;
import com.caucho.hessian.test.TestHessian2;
import com.caucho.hessian.test.TestHessian2Servlet;
import com.caucho.hessian.test.TestHessianServlet;
import java.util.HashMap;
import java.util.Map;

public class Main {

    public static void main(String[] args) throws Exception {

        Test test = new TestHessianServlet();
        TestHessian2 test2 = new TestHessian2Servlet();

        Map servlets = new HashMap();
        servlets.put("/test", test);
        servlets.put("/test2", test2);

        HttpServer httpServer = new HttpServer("/test", servlets);
        Thread httpServerThread = new Thread(httpServer);

        SocketServer socketServer = new SocketServer(test);
        Thread socketServerThread = new Thread(socketServer);

        httpServerThread.start();
        socketServerThread.start();

        httpServerThread.join();
        socketServerThread.join();
    }
}
