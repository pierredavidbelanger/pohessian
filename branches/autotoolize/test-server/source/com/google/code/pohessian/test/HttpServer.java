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

import com.caucho.hessian.server.HessianServlet;
import java.util.Iterator;
import java.util.Map;
import org.eclipse.jetty.server.Server;
import org.eclipse.jetty.servlet.ServletContextHandler;
import org.eclipse.jetty.servlet.ServletHolder;

public class HttpServer implements Runnable {

    private String context;
    private Map servlets;

    public HttpServer(String context, Map servlets) {
        this.context = context;
        this.servlets = servlets;
    }

    public void run() {
        Server server = new Server(8181);
        ServletContextHandler servletContextHandler = new ServletContextHandler();
        servletContextHandler.setContextPath(context);
        for (Iterator it = servlets.entrySet().iterator(); it.hasNext();) {
            Map.Entry entry = (Map.Entry) it.next();
            String path = (String) entry.getKey();
            Object bean = entry.getValue();
            HessianServlet hessianServlet;
            if (bean instanceof HessianServlet) {
                hessianServlet = (HessianServlet) bean;
            } else {
                hessianServlet = new HessianServlet();
                hessianServlet.setHome(bean);
            }
            ServletHolder servletHolder = new ServletHolder(hessianServlet);
            servletContextHandler.addServlet(servletHolder, path);
        }
        server.setHandler(servletContextHandler);
        try {
            server.start();
            server.join();
        } catch (Exception e) {
            e.printStackTrace(System.err);
            throw new RuntimeException(e);
        }
    }
}
