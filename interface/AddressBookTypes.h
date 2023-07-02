#ifndef ADDRESS_BOOK_TYPES_H
#define ADDRESS_BOOK_TYPES_H
//=======================================================
//		Includes
//=======================================================
#include "AddressBookCommon.h"

//=======================================================
//		Enums
//=======================================================
// Error types
enum class AddressEntryError : uint32_t
{
	kAddressEntrySuccess,
	kAddressEntryDuplicate,
	kAddressEntryInvalid,
	kAddressEntryNotFound,
	kAddressEntryNotAttempted
};

// Retrieval order type
enum class AddressEntryOrderType : uint32_t
{
	FirstNameOrder,
	LastNameOrder
};

// Search type
enum class AddressEntrySearchType : uint32_t
{
	FirstNameSearch,
	LastNameSearch,
	FirstAndLastNameSearch
};

//=======================================================
//		Aliases
//=======================================================
struct AddressEntry;
using AddressEntries = std::list<AddressEntry>;
using AddressEntryCallback = std::function<void(const AddressEntry& addressEntry)>;

//=======================================================
//		AddressEntry : A single address entry
//=======================================================
struct AddressEntry
{
	std::string mFirstName;
	std::string mLastName;
	std::string mPhoneNumber;

	AddressEntry() {}
	AddressEntry(const std::string& firstName, const std::string& lastName, const std::string& phoneNumber, const std::string& homeAddress) :
		mFirstName(firstName), mLastName(lastName), mPhoneNumber(phoneNumber)
	{

	}

	// Comparison
	bool operator==(const AddressEntry& rhs) const
	{
		return mFirstName == rhs.mFirstName && mLastName == rhs.mLastName && mPhoneNumber == rhs.mPhoneNumber;
	}
};

//=======================================================
//		Stream operators
//=======================================================
std::ostream& operator<<(std::ostream& os, const AddressEntry& addressEntry);
std::istream& operator>>(std::istream& is, AddressEntry& addressEntry);

#endif // ADDRESS_BOOK_TYPES_H