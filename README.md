# Address Book Interface
A simple thread-safe address book interface to add, remove, search and retrieve address entries alphabetically in both first or last name order.

## Features/Details
* Thread-safe
* Customisable address entries (*firstname* + *lastname* + *phonenumber* + ...).
* Add/remove entries which are sorted internally in tries/prefix trees.
* Retrieve entries in alphabetical order.
* Search for entries using first or last name.

## Build Instructions
1. Clone repo
2. `mkdir` and `cd` into a build folder
3. Run cmake using `cmake path/to/repo` to configure project
4. Build `cmake --build .` and execute `DemoApp.exe` to test out demo application.