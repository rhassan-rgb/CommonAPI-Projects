#include <iostream>
#include <string>
#include <thread>

#include <unistd.h>

#include <CommonAPI/CommonAPI.hpp>
#include <v1/rgb/examples/MyTypes.hpp>
#include <v1/rgb/examples/HelloEverythingProxy.hpp>

using CapiRuntime = CommonAPI::Runtime;
namespace iNamespace = v1::rgb::examples;

int main () {
    // Set logging properties
    CapiRuntime::setProperty("LogContext", "HETC");
    CapiRuntime::setProperty("LogApplication", "CLNT");

    // get runtime object
    std::shared_ptr<CapiRuntime> runtime = CapiRuntime::get();

    // manually define address translation properties
    // TODO: Remove and use address translation configs
    std::string domain = "local";
    std::string instance = "rgb.examples.HelloEverythingInstance0";
    std::string connection = "client-sample";

    // get a proxy from runtime
    std::shared_ptr<iNamespace::HelloEverythingProxy<>> myProxy = runtime->buildProxy<iNamespace::HelloEverythingProxy>(domain, instance, connection);

    std::cout << "Checking availability..." << std::endl;
    while (!myProxy->isAvailable()) {
        std::cout << "Checking availability failed! Trying again..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "Available!" << std::endl;

    CommonAPI::CallStatus callStatus;

    int32_t intAttrValue;
    iNamespace::MyTypes::MyStruct customAttrValue;

    CommonAPI::CallInfo info(1000);
    info.sender_ = 0x1234;

    //getting the actual attribute value
    std::cout << "Getting MyTestAttribute Value.." << std::endl;
    myProxy->getMyTestAttributeAttribute().getValue(callStatus, intAttrValue, &info);

    if (callStatus != CommonAPI::CallStatus::SUCCESS) {
        std::cerr << "Remote call for MyTestAttribute.get() failed!" << std::endl;
        return -1;
    }

    std::cout << "Got attribute value successfully! " << intAttrValue << std::endl;

        //getting the actual attribute value
    std::cout << "Getting MyCustomTestAttribute Value.." << std::endl;
    myProxy->getMyCustomTestAttributeAttribute().getValue(callStatus, customAttrValue, &info);

    if (callStatus != CommonAPI::CallStatus::SUCCESS) {
        std::cerr << "Remote call for MyCustomTestAttribute.get() failed!" << std::endl;
        return -1;
    }

    std::cout << "Got attribute value successfully! " << std::endl <<  
                 "int:  " << customAttrValue.getMyInt32() << std::endl <<
                 "bool:  " << customAttrValue.getMyBoolean() << std::endl <<
                 "double:  " << customAttrValue.getMyDouble() << std::endl;

    // subscribe to updates
    myProxy->getMyTestAttributeAttribute().getChangedEvent().subscribe([&](const int32_t& val) { 
        std::cout << "Received attribute value: " << val << std::endl;
    });

    myProxy->getMyCustomTestAttributeAttribute().getChangedEvent().subscribe([&](const iNamespace::MyTypes::MyStruct& val) { 
            std::cout << "Received attribute value successfully! " << std::endl <<  
                 "int:  " << val.getMyInt32() << std::endl <<
                 "bool:  " << val.getMyBoolean() << std::endl <<
                 "double:  " << val.getMyDouble() << std::endl;
    });

    intAttrValue = 15;
    customAttrValue.setMyBoolean(true);
    customAttrValue.setMyDouble(1.1);
    customAttrValue.setMyInt32(intAttrValue);
    
    while (true) {
        int32_t resp;
        iNamespace::MyTypes::MyStruct structResp;
        std::cout << "Setting MyTestAttribute to: " << intAttrValue << "..." << std::endl;
        myProxy->getMyTestAttributeAttribute().setValue(intAttrValue, callStatus, resp, &info);
        std::cout << "Set MyTestAttribute to: " << resp << "!" << std::endl;

        std::cout << "Setting MyCustomTestAttribute " << std::endl <<  
                 "int:  " << customAttrValue.getMyInt32() << std::endl <<
                 "bool:  " << customAttrValue.getMyBoolean() << std::endl <<
                 "double:  " << customAttrValue.getMyDouble() << std::endl;
        myProxy->getMyCustomTestAttributeAttribute().setValue(customAttrValue, callStatus, structResp, &info);
        std::cout << "Set MyCustomTestAttribute " << std::endl <<  
                 "int:  " << structResp.getMyInt32() << std::endl <<
                 "bool:  " << structResp.getMyBoolean() << std::endl <<
                 "double:  " << structResp.getMyDouble() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

}