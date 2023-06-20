cmake_minimum_required(VERSION 3.25 )
project(SimpleAddressBook LANGUAGES CXX)

add_library(SimpleAddressBookLib STATIC)

target_sources(SimpleAddressBookLib
    PRIVATE
    "source/CSimpleAddressBook.cpp"
    "source/CAddressBookManager.cpp"
    "source/SimpleAddressBook.cpp"
    "source/SimpleAddressBookTypes.cpp"
    "headers/CSimpleAddressBook.h"
    "headers/CAddressBookManager.h"
    "interfaces/SimpleAddressBook.h"
    "interfaces/SimpleAddressBookTypes.h"
)
target_include_directories(SimpleAddressBookLib PUBLIC "interfaces" PRIVATE "headers")

add_executable(AddressBookApp "source/main.cpp")
target_link_libraries(AddressBookApp PUBLIC SimpleAddressBookLib)
