/*
 * This file was generated by the CommonAPI Generators.
 * Used org.genivi.commonapi.someip 3.2.15.v202410041637.
 * Used org.franca.core 0.13.1.201807231814.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * http://mozilla.org/MPL/2.0/.
 */
#ifndef V1_RGB_EXAMPLES_HELLO_EVERYTHING_SOMEIP_STUB_ADAPTER_HPP_
#define V1_RGB_EXAMPLES_HELLO_EVERYTHING_SOMEIP_STUB_ADAPTER_HPP_

#include <v1/rgb/examples/HelloEverythingStub.hpp>
#include <v1/rgb/examples/MyTypesSomeIPDeployment.hpp>

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#define HAS_DEFINED_COMMONAPI_INTERNAL_COMPILATION_HERE
#endif

#include <CommonAPI/SomeIP/AddressTranslator.hpp>
#include <CommonAPI/SomeIP/StubAdapterHelper.hpp>
#include <CommonAPI/SomeIP/StubAdapter.hpp>
#include <CommonAPI/SomeIP/Factory.hpp>
#include <CommonAPI/SomeIP/Types.hpp>
#include <CommonAPI/SomeIP/Constants.hpp>

#if defined (HAS_DEFINED_COMMONAPI_INTERNAL_COMPILATION_HERE)
#undef COMMONAPI_INTERNAL_COMPILATION
#undef HAS_DEFINED_COMMONAPI_INTERNAL_COMPILATION_HERE
#endif

namespace v1 {
namespace rgb {
namespace examples {

template <typename _Stub = ::v1::rgb::examples::HelloEverythingStub, typename... _Stubs>
class HelloEverythingSomeIPStubAdapterInternal
    : public virtual HelloEverythingStubAdapter,
      public CommonAPI::SomeIP::StubAdapterHelper< _Stub, _Stubs...>,
      public std::enable_shared_from_this< HelloEverythingSomeIPStubAdapterInternal<_Stub, _Stubs...>>
{
public:
    typedef CommonAPI::SomeIP::StubAdapterHelper< _Stub, _Stubs...> HelloEverythingSomeIPStubAdapterHelper;

    ~HelloEverythingSomeIPStubAdapterInternal() {
        deactivateManagedInstances();
        HelloEverythingSomeIPStubAdapterHelper::deinit();
    }

    void fireMyTestAttributeAttributeChanged(const int32_t &_value);
    
    void fireMyCustomTestAttributeAttributeChanged(const ::v1::rgb::examples::MyTypes::MyStruct &_value);
    
    void deactivateManagedInstances() {}
    
    CommonAPI::SomeIP::GetAttributeStubDispatcher<
        ::v1::rgb::examples::HelloEverythingStub,
        CommonAPI::Version
    > getHelloEverythingInterfaceVersionStubDispatcher;

    CommonAPI::SomeIP::GetAttributeStubDispatcher<
        ::v1::rgb::examples::HelloEverythingStub,
        int32_t,
        CommonAPI::SomeIP::IntegerDeployment<int32_t>
    > getMyTestAttributeAttributeStubDispatcher;
    
    CommonAPI::SomeIP::SetObservableAttributeStubDispatcher<
        ::v1::rgb::examples::HelloEverythingStub,
        int32_t,
        CommonAPI::SomeIP::IntegerDeployment<int32_t>
    > setMyTestAttributeAttributeStubDispatcher;
    
    CommonAPI::SomeIP::GetAttributeStubDispatcher<
        ::v1::rgb::examples::HelloEverythingStub,
        ::v1::rgb::examples::MyTypes::MyStruct,
        ::v1::rgb::examples::MyTypes_::MyStructDeployment_t
    > getMyCustomTestAttributeAttributeStubDispatcher;
    
    CommonAPI::SomeIP::SetObservableAttributeStubDispatcher<
        ::v1::rgb::examples::HelloEverythingStub,
        ::v1::rgb::examples::MyTypes::MyStruct,
        ::v1::rgb::examples::MyTypes_::MyStructDeployment_t
    > setMyCustomTestAttributeAttributeStubDispatcher;
    
    HelloEverythingSomeIPStubAdapterInternal(
        const CommonAPI::SomeIP::Address &_address,
        const std::shared_ptr<CommonAPI::SomeIP::ProxyConnection> &_connection,
        const std::shared_ptr<CommonAPI::StubBase> &_stub):
        CommonAPI::SomeIP::StubAdapter(_address, _connection),
        HelloEverythingSomeIPStubAdapterHelper(
            _address,
            _connection,
            std::dynamic_pointer_cast< HelloEverythingStub>(_stub)),
        getHelloEverythingInterfaceVersionStubDispatcher(&HelloEverythingStub::lockInterfaceVersionAttribute, &HelloEverythingStub::getInterfaceVersion, false, true),
        getMyTestAttributeAttributeStubDispatcher(
            &::v1::rgb::examples::HelloEverythingStub::lockMyTestAttributeAttribute,
            &::v1::rgb::examples::HelloEverythingStub::getMyTestAttributeAttribute,
            false,
            _stub->hasElement(0)),
        setMyTestAttributeAttributeStubDispatcher(
            &::v1::rgb::examples::HelloEverythingStub::lockMyTestAttributeAttribute,
            &::v1::rgb::examples::HelloEverythingStub::getMyTestAttributeAttribute,
            &HelloEverythingStubRemoteEvent::onRemoteSetMyTestAttributeAttribute,
            &HelloEverythingStubRemoteEvent::onRemoteMyTestAttributeAttributeChanged,
            &HelloEverythingStubAdapter::fireMyTestAttributeAttributeChanged,
            false,
            _stub->hasElement(0))
        ,
        getMyCustomTestAttributeAttributeStubDispatcher(
            &::v1::rgb::examples::HelloEverythingStub::lockMyCustomTestAttributeAttribute,
            &::v1::rgb::examples::HelloEverythingStub::getMyCustomTestAttributeAttribute,
            false,
            _stub->hasElement(1)),
        setMyCustomTestAttributeAttributeStubDispatcher(
            &::v1::rgb::examples::HelloEverythingStub::lockMyCustomTestAttributeAttribute,
            &::v1::rgb::examples::HelloEverythingStub::getMyCustomTestAttributeAttribute,
            &HelloEverythingStubRemoteEvent::onRemoteSetMyCustomTestAttributeAttribute,
            &HelloEverythingStubRemoteEvent::onRemoteMyCustomTestAttributeAttributeChanged,
            &HelloEverythingStubAdapter::fireMyCustomTestAttributeAttributeChanged,
            false,
            _stub->hasElement(1))
    {
        HelloEverythingSomeIPStubAdapterHelper::addStubDispatcher( { CommonAPI::SomeIP::method_id_t(0xa01) }, &getMyTestAttributeAttributeStubDispatcher );
        HelloEverythingSomeIPStubAdapterHelper::addStubDispatcher( { CommonAPI::SomeIP::method_id_t(0xa02) }, &setMyTestAttributeAttributeStubDispatcher );
        HelloEverythingSomeIPStubAdapterHelper::addStubDispatcher( { CommonAPI::SomeIP::method_id_t(0xee9) }, &getMyCustomTestAttributeAttributeStubDispatcher );
        HelloEverythingSomeIPStubAdapterHelper::addStubDispatcher( { CommonAPI::SomeIP::method_id_t(0xb02) }, &setMyCustomTestAttributeAttributeStubDispatcher );
        std::shared_ptr<CommonAPI::SomeIP::ClientId> itsClient = std::make_shared<CommonAPI::SomeIP::ClientId>();

        // Provided events/fields
        if (_stub->hasElement(0)) {
            std::set<CommonAPI::SomeIP::eventgroup_id_t> itsEventGroups;
            itsEventGroups.insert(CommonAPI::SomeIP::eventgroup_id_t(CommonAPI::SomeIP::eventgroup_id_t(0xa03)));
            CommonAPI::SomeIP::StubAdapter::registerEvent(CommonAPI::SomeIP::event_id_t(0xa03), itsEventGroups, CommonAPI::SomeIP::event_type_e::ET_FIELD, CommonAPI::SomeIP::reliability_type_e::RT_RELIABLE);
            fireMyTestAttributeAttributeChanged(std::dynamic_pointer_cast< ::v1::rgb::examples::HelloEverythingStub>(_stub)->getMyTestAttributeAttribute(itsClient));
        }

        if (_stub->hasElement(1)) {
            std::set<CommonAPI::SomeIP::eventgroup_id_t> itsEventGroups;
            itsEventGroups.insert(CommonAPI::SomeIP::eventgroup_id_t(CommonAPI::SomeIP::eventgroup_id_t(0xb03)));
            CommonAPI::SomeIP::StubAdapter::registerEvent(CommonAPI::SomeIP::event_id_t(0xb03), itsEventGroups, CommonAPI::SomeIP::event_type_e::ET_FIELD, CommonAPI::SomeIP::reliability_type_e::RT_RELIABLE);
            fireMyCustomTestAttributeAttributeChanged(std::dynamic_pointer_cast< ::v1::rgb::examples::HelloEverythingStub>(_stub)->getMyCustomTestAttributeAttribute(itsClient));
        }

    }

    // Register/Unregister event handlers for selective broadcasts
    void registerSelectiveEventHandlers();
    void unregisterSelectiveEventHandlers();

};

template <typename _Stub, typename... _Stubs>
void HelloEverythingSomeIPStubAdapterInternal<_Stub, _Stubs...>::fireMyTestAttributeAttributeChanged(const int32_t &_value) {
    CommonAPI::Deployable< int32_t, CommonAPI::SomeIP::IntegerDeployment<int32_t>> deployedValue(_value, static_cast< CommonAPI::SomeIP::IntegerDeployment<int32_t>* >(nullptr));
    CommonAPI::SomeIP::StubEventHelper<
        CommonAPI::SomeIP::SerializableArguments<
            CommonAPI::Deployable<
                int32_t,
                CommonAPI::SomeIP::IntegerDeployment<int32_t>
            >
            >
    >::sendEvent(
        *this,
        CommonAPI::SomeIP::event_id_t(0xa03),
        false,
        deployedValue
    );
}

template <typename _Stub, typename... _Stubs>
void HelloEverythingSomeIPStubAdapterInternal<_Stub, _Stubs...>::fireMyCustomTestAttributeAttributeChanged(const ::v1::rgb::examples::MyTypes::MyStruct &_value) {
    CommonAPI::Deployable< ::v1::rgb::examples::MyTypes::MyStruct, ::v1::rgb::examples::MyTypes_::MyStructDeployment_t> deployedValue(_value, static_cast< ::v1::rgb::examples::MyTypes_::MyStructDeployment_t* >(nullptr));
    CommonAPI::SomeIP::StubEventHelper<
        CommonAPI::SomeIP::SerializableArguments<
            CommonAPI::Deployable<
                ::v1::rgb::examples::MyTypes::MyStruct,
                ::v1::rgb::examples::MyTypes_::MyStructDeployment_t
            >
            >
    >::sendEvent(
        *this,
        CommonAPI::SomeIP::event_id_t(0xb03),
        false,
        deployedValue
    );
}


template <typename _Stub, typename... _Stubs>
void HelloEverythingSomeIPStubAdapterInternal<_Stub, _Stubs...>::registerSelectiveEventHandlers() {

}

template <typename _Stub, typename... _Stubs>
void HelloEverythingSomeIPStubAdapterInternal<_Stub, _Stubs...>::unregisterSelectiveEventHandlers() {

}

template <typename _Stub = ::v1::rgb::examples::HelloEverythingStub, typename... _Stubs>
class HelloEverythingSomeIPStubAdapter
    : public HelloEverythingSomeIPStubAdapterInternal<_Stub, _Stubs...> {
public:
    HelloEverythingSomeIPStubAdapter(const CommonAPI::SomeIP::Address &_address,
                                            const std::shared_ptr<CommonAPI::SomeIP::ProxyConnection> &_connection,
                                            const std::shared_ptr<CommonAPI::StubBase> &_stub)
        : CommonAPI::SomeIP::StubAdapter(_address, _connection),
          HelloEverythingSomeIPStubAdapterInternal<_Stub, _Stubs...>(_address, _connection, _stub) {
    }
};

} // namespace examples
} // namespace rgb
} // namespace v1

#endif // V1_RGB_EXAMPLES_Hello_Everything_SOMEIP_STUB_ADAPTER_HPP_
