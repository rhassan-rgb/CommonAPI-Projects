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