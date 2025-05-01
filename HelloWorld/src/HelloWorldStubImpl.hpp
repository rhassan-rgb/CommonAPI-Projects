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

#ifndef HELLOWORLDSTUBIMPL_H_
#define HELLOWORLDSTUBIMPL_H_

#include <CommonAPI/CommonAPI.hpp>
#include <v0/commonapi/examples/HelloWorldStubDefault.hpp>

class HelloWorldStubImpl: public v0_1::commonapi::examples::HelloWorldStubDefault {

public:
    HelloWorldStubImpl();
    ~HelloWorldStubImpl();

    void sayHello(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _name, sayHelloReply_t _return) override final;

};
#endif /* HELLOWORLDSTUBIMPL_H_ */