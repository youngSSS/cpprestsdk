# How to setup, build, and run tests on OSX
1. Install the required development tools
<br/>`brew install camke git boost openssl ninja`
2. Clone the project using Git
<br/>`git clone https://github.com/Microsoft/cpprestsdk.git casablanca`
<br/>`cd casablanca`
<br/>`git submodule update --init`
3. Build the SDK in Debug mode
<br/>`mkdir build.debug`
<br/>`cd build.debug`
<br/>`cmake -G Ninja ../Release -DCMAKE_BUILD_TYPE=Debug`
<br/>`ninja`
4. After building you can run the tests by launching the test_runner executable
<br/>`cd Binaries`
<br/>`./test_runner *_test.dylib`

# Reference, 2021.09.25
### MS CPPRESTSDK(Casablanca)
1. https://github.com/microsoft/cpprestsdk
2. https://github.com/microsoft/cpprestsdk/wiki/How-to-build-for-Mac-OS-X