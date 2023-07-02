cmake_minimum_required(VERSION 3.25)
project(AddressBookInterface LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 17)

add_library(AddressBookLib STATIC)

target_sources(AddressBookLib
    PRIVATE
    "interface/AddressBookInterface.h"
    "interface/AddressBookTypes.h"
    "interface/AddressBookCommon.h"
    "header/CAddressBookTrie.h"
    "header/CAddressBook.h"
    "header/CAddressBookManager.h"

    "source/AddressBookInterface.cpp"
    "source/AddressBookTypes.cpp"
    "source/CAddressBookTrie.cpp"
    "source/CAddressBook.cpp"
    "source/CAddressBookManager.cpp"
)

target_include_directories(AddressBookLib PUBLIC "interface" PRIVATE "header")

add_executable(DemoApp "DemoApp.cpp")
target_link_libraries(DemoApp PUBLIC AddressBookLib)