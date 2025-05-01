#include "HelloWorldStubImpl.hpp"

HelloWorldStubImpl::HelloWorldStubImpl() {

}

HelloWorldStubImpl::~HelloWorldStubImpl() {
    
}

void HelloWorldStubImpl::sayHello(  const std::shared_ptr<CommonAPI::ClientId> _client, 
                                    std::string _name, 
                                    sayHelloReply_t _return) {
    std::stringstream messageStream;

    messageStream << "Hello " << _name << "!";
    std::cout << "sayHello('" << _name << "'): " << messageStream.str() << std::endl;
    _return(messageStream.str());
}