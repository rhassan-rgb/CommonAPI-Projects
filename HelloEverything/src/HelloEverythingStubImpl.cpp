#include "HelloEverythingStubImpl.hpp"

HelloEverythingStubImpl::HelloEverythingStubImpl() :
serviceBool{true},
serviceInt{0},
serviceDouble{0},
serviceStruct{0, true, 0}
{

}

HelloEverythingStubImpl::~HelloEverythingStubImpl(){
    
}

void HelloEverythingStubImpl::IncrementAll() {
    ++serviceInt;
    serviceBool = ((serviceInt % 2 ) == 0);
    serviceDouble =  serviceInt/10.0;

    serviceStruct.setMyBoolean(serviceBool);
    serviceStruct.setMyInt32(serviceInt);
    serviceStruct.setMyDouble(serviceDouble);

    std::cout << "Updating: " << std::endl << 
                    "\tint value: " << serviceInt << std::endl << 
                    "\tbool value: " << serviceBool << std::endl << 
                    "\tdouble value: " << serviceDouble << std::endl << 
                    std::endl;
    
    setMyTestAttributeAttribute(serviceInt);

    setMyCustomTestAttributeAttribute(serviceStruct);
}

void HelloEverythingStubImpl::onRemoteMyCustomTestAttributeAttributeChanged() {
    std::cout << "Someone updated MyCustomTestAttribute!" << std::endl;
}

void HelloEverythingStubImpl::onRemoteMyTestAttributeAttributeChanged() {
    std::cout << "Someone updated MyTestAttribute!" << std::endl;
}