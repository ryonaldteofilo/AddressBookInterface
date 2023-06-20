#ifndef SIMPLE_ADDRESS_BOOK_TYPES_H
#define SIMPLE_ADDRESS_BOOK_TYPES_H
//=======================================================
//		Includes
//=======================================================
#include <string>
#include <list>
#include <iostream>
#include <functional>





//=======================================================
//		Enums
//=======================================================
// Error types
enum class AddressEntryError
{
	kAddressEntrySuccess,
	kAddressEntryDuplicate,
	kAddressEntryInvalid,
	kAddressEntryNotFound,
	kAddressEntryNotAttempted
};

// Retrieval order type
enum class AddressEntryOrderType
{
	FirstNameOrder,
	LastNameOrder
};

// Search type
enum class AddressEntrySearchType
{
	FirstNameSearch,
	LastNameSearch,
	FirstAndLastNameSearch
};





//=======================================================
//		AddressEntry : A single address entry
//=======================================================
struct AddressEntry
{
	std::string mFirstName;
	std::string mLastName;
	std::string mPhoneNumber;

	AddressEntry();
	AddressEntry(const std::string& firstName, const std::string& lastName, const std::string& phoneNumber, const std::string& homeAddress);
	bool operator==(const AddressEntry& rhs) const;
};

// Aliases
using AddressEntries = std::list<AddressEntry>;
using AddressEntryCallback = std::function<void(const AddressEntry& addressEntry)>;





//=======================================================
//		Stream operators
//=======================================================
std::ostream& operator<< (std::ostream& os, const AddressEntry& addressEntry);
std::istream& operator>> (std::istream& is, AddressEntry& addressEntry);
#endif // SIMPLE_ADDRESS_BOOK_TYPES_H
