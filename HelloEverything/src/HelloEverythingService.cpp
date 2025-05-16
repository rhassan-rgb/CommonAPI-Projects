#include <iostream>
#include <thread>

#include <CommonAPI/CommonAPI.hpp>
#include "HelloEverythingStubImpl.hpp"

using CapiRuntime = CommonAPI::Runtime;

int main() {
    
    // Set logging properties
    CapiRuntime::setProperty("LogContext", "HETS");
    CapiRuntime::setProperty("LogApplication", "SRVC");
    
    // get the runtime object
    std::shared_ptr<CapiRuntime> runtime = CapiRuntime::get();

    // Set address translation manually
    // TODO: Remove and use CAPI address translation configs
    std::string domain = "local";
    std::string instance = "rgb.examples.HelloEverythingInstance0";
    std::string connection = "service-sample";

    // instantiate the service
    std::shared_ptr<HelloEverythingStubImpl> myService = std::make_shared<HelloEverythingStubImpl>();

    // registration sentinel
    bool successfullyRegistered = runtime->registerService(domain, instance, myService, connection);
    
    // keep trying 
    while (!successfullyRegistered) {
        std::cout << "Register Service Failed, trying again in 1s..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        successfullyRegistered = runtime->registerService(domain, instance, myService, connection);
    }
    
    std::cout << "Successfully Registered Service!" << std::endl;
    
    // do the work
    while (true) {
        std::cout << "Incrementing.." << std::endl;
        myService->IncrementAll();
        std::cout << "Sleeping for 2S" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;
}