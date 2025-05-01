# SomeIP through CommmonAPI
Using Common API unifies the interface used by the application regaless the middleware protocol.

For more details Please refer to [CommonAPI Official Repo](https://github.com/COVESA/capicxx-core-runtime) and [CommonAPI C++ Documentation](https://covesa.github.io/capicxx-core-tools/)

## Dependencies
- Packages: 
    - system tools
        ```
        $ sudo apt-get update
        $ sudo apt-get install -y git wget unzip
        ```
    - Boost Library
        ```
        $ sudo apt-get update
        $ sudo apt-get install -y libboost-all-dev
        ```
- Sources:
    - CommonAPI-core-runtime
    - CommonAPI-someip-runtime
    - vsomeip
- Code Generation Tools:
    - CommonAPI-core-tools
    - CommonAPI-someip-tools

## Environemnt Variables
- DEP_INSTALL_FOLDER
- DEP_LIBRARY_PATH

## Common API Architecture 

~~~
+---------------------------------------+
|                  App                  |     
+---------------------------------------+

+-----------+   +-----------------------+
|           |   |                       |
|           |   |     CommonAPI Core    |
|           |   |     Generated Code    |
|           |   |                       |
|           |   +-----------------------+
|           +-----------+   +-----------+
|    CommonAPI Core     |   |           |
|        runtime        |   |           |
+-----------------------+   | CommonAPI |
                            |  Binding  |
+-----------------------+   |           |
|                       |   | generated |
|   CommonAPI Binding   |   |    code   |
|         Runtime       |   |           |
|                       |   |           |
+-----------------------+   +-----------+

+---------------------------------------+
|               IPC Stack               |
+---------------------------------------+
~~~

The common API library consists of two levels of abstractions:
- Core
- Binding

**Core**:
It implements part the APIs that's used directly by the application, I has no middleware-related information

**Binding**:
It contains all the middleware-related information in its implementation, but from the user perspective it's the same API for whatever middleware.
<!-- 
## Build Steps
1. Clone the dependencies
2. Get the code generation tools
3. Build dependencies
4. Write your app -->

# Let's get our hands dirty

First, let's have a simple interface with only one method `SayHello` that takes a string argument `name` and returns a string value `message`. 

For that we will use Franca IDL to have a stable, versioned API

## Franca Interface Definition Language (fidl)

This file has the interface specs that will be parsed by the core and binding generators in order to generate the core and glue code.
~~~
package commonapi.examples

interface HelloWorld {
    version { major 0 minor 1 }

    method sayHello {
        in {
            String name
        }
        out {
            String message
        }
    }
}
~~~
**package** is part of the interface identifier

**interface** an interface inside that package 

**version** the version of this interface instance, different versions could co-exist 

**method** a method inside this interface

**in** input parameters

**out** output parameters

Types can be found in 5.1.1 of franca specs document.

Now, our interface is ready to be used by the core runtime generator, but if you're familiar with someip you'll notice that there are some missing information that's needed by both the service and its clients (i.e. , service id, instance id, method id, ...etc) that's where the deployment file comes in useful. So, let's do it!

## Franca dependency file (fdepl)
In this file we need to first define the specs by which we will parse this dependency file, we will use CommonAPI-4-SOMEIP_deployment_spec.fdepl which can be found [here](https://github.com/COVESA/capicxx-someip-tools/blob/master/org.genivi.commonapi.someip/deployment/CommonAPI-4-SOMEIP_deployment_spec.fdepl)

Don't worry, the generator will understand that path.

Then, we need to import the fidl file so we can define the deployment details of the interface

After that, we will specify our deployment detail based on the specs.

~~~
import "platform:/plugin/org.genivi.commonapi.someip/deployment/CommonAPI-4-SOMEIP_deployment_spec.fdepl"
import "HelloWorld.fidl"

define org.genivi.commonapi.someip.deployment for interface commonapi.examples.HelloWorld {
    SomeIpServiceID = 4660

    method sayHello {
        SomeIpMethodID = 30000
        SomeIpReliable = true

        in {
            name {
                SomeIpStringEncoding = utf16le
            }
        }
    }
}

define org.genivi.commonapi.someip.deployment for provider as Service {
    instance commonapi.examples.HelloWorld {
        InstanceId = "commonapi.examples.HelloWorld"

        SomeIpInstanceID = 4660

        SomeIpUnicastAddress = "192.168.7.2"
        SomeIpReliableUnicastPort = 30499
        SomeIpUnreliableUnicastPort = 30499
    }
}
~~~

as you can see we have two main parts of the deployment, one for the interface identification details and another one for the service discovery details.

