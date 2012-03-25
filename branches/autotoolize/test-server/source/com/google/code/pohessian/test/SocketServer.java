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

import com.caucho.hessian.server.HessianSkeleton;
import com.caucho.services.server.ServiceContext;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class SocketServer implements Runnable {

    protected HessianSkeleton skeleton;

    public SocketServer(Object bean) {
        skeleton = new HessianSkeleton(bean, bean.getClass());
    }

    public void run() {
        try {
            ServerSocket serverSocket = new ServerSocket(9191);
            while (true) {
                final Socket socket = serverSocket.accept();
                new Thread(new Runnable() {

                    public void run() {
                        SocketServer.this.run(socket);
                    }
                }).start();
            }
        } catch (Exception e) {
            e.printStackTrace(System.err);
            throw new RuntimeException(e);
        }
    }

    protected void run(Socket socket) {
        try {
            InputStream is = socket.getInputStream();
            OutputStream os = socket.getOutputStream();
            ServiceContext.begin(null, null, null);
            skeleton.invoke(is, os);
        } catch (Exception e) {
            e.printStackTrace(System.err);
            throw new RuntimeException(e);
        } finally {
            ServiceContext.end();
            try {
                socket.close();
            } catch (Exception e) {
                e.printStackTrace(System.err);
            }
        }
    }
}
