/**
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 * 
 * Copyright (C) 2025 Ragab R. Hassan (r.elkattawy@gmail.com)
 * The source of this file can be found here:
 * https://github.com/COVESA/capicxx-core-tools/wiki/Loading-Bindings-And-Libraries  
 * 
 */

#include <iostream>
#include <thread>

#include <CommonAPI/CommonAPI.hpp>
#include "HelloWorldStubImpl.hpp"

int main() {
    CommonAPI::Runtime::setProperty("LogContext", "E01S");
    CommonAPI::Runtime::setProperty("LogApplication", "E01S");
    CommonAPI::Runtime::setProperty("LibraryBase", "HelloWorld");

    std::shared_ptr<CommonAPI::Runtime> runtime = CommonAPI::Runtime::get();

    std::string domain = "local";
    std::string instance = "commonapi.examples.HelloWorld";
    std::string connection = "service-sample";

    std::shared_ptr<HelloWorldStubImpl> myService = std::make_shared<HelloWorldStubImpl>();
    bool successfullyRegistered = runtime->registerService(domain,instance, myService, connection);

    while (!successfullyRegistered) {
        std::cout << "Register Service Failed, trying again in 10 ms..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        successfullyRegistered = runtime->registerService(domain,instance, myService, connection);
    }
    
    std::cout << "Successfully Registered Service!" << std::endl;
    
    while (true) {
        std::cout << "Waiting for calls... (Abort with CTRL+C)" <<  std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
    return 0;
}