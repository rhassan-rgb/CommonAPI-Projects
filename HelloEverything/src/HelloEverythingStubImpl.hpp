#pragma once
#include <cstdint>

#include "CommonAPI/CommonAPI.hpp"
#include "v1/rgb/examples/HelloEverythingStubDefault.hpp"

namespace iNamespace = v1_0::rgb::examples;
using myStruct = iNamespace::MyTypes::MyStruct;

class HelloEverythingStubImpl : public iNamespace::HelloEverythingStubDefault {
public:
    HelloEverythingStubImpl();
    ~HelloEverythingStubImpl();

    void IncrementAll();
    
private:
    bool serviceBool;
    double serviceDouble;
    int32_t serviceInt;
    myStruct serviceStruct;

    void onRemoteMyTestAttributeAttributeChanged() override;
    void onRemoteMyCustomTestAttributeAttributeChanged() override;
};
