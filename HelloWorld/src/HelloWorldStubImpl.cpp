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