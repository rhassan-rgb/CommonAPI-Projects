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
#include <string>
#include <thread>

#include <unistd.h>

#include <CommonAPI/CommonAPI.hpp>
#include <v0/commonapi/examples/HelloWorldProxy.hpp>

using namespace v0::commonapi::examples;

int main() {
    CommonAPI::Runtime::setProperty("LogContext", "E01C");
    CommonAPI::Runtime::setProperty("LogApplication", "E01C");
    CommonAPI::Runtime::setProperty("LibraryBase", "HelloWorld");

    std::shared_ptr<CommonAPI::Runtime> runtime = CommonAPI::Runtime::get();

    std::string domain = "local";
    std::string instance = "commonapi.examples.HelloWorld";
    std::string conneection = "client-ample";

    std::shared_ptr<HelloWorldProxy<>> myProxy = runtime->buildProxy<HelloWorldProxy>(domain, instance, conneection);

    std::cout << "Checking Availability..." << std::endl;
    while(!myProxy->isAvailable())
    {
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
    std::cout << "Available!" << std::endl;

    const std::string name = "world";

    CommonAPI::CallStatus callStatus;
    std::string returnMessage;

    CommonAPI::CallInfo info(1000);
    info.sender_ = 0x1234; //Client ID?

    while(true) {
        myProxy->sayHello(name, callStatus, returnMessage, &info);
        if (callStatus != CommonAPI::CallStatus::SUCCESS) {
            std::cerr << "Remote call failed: " << (int)callStatus << std::endl;
            return -1;
        }
        info.timeout_ += 1000;

        std::cout << "Got message: '" << returnMessage << "'\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;

}