#ifndef SIMPLE_ADDRESS_BOOK_H
#define SIMPLE_ADDRESS_BOOK_H
//=======================================================
//		Includes
//=======================================================
#include "SimpleAddressBookTypes.h"





//=======================================================
//		SimpleAddressBook
//=======================================================
namespace SimpleAddressBook
{
	// Add address entry
	AddressEntryError AddEntry(const AddressEntry& entry);

	// Find entry and remove if found
	// removeMatchingOnly - true, remove only entries that match the passed in entry exactly
	//						false, remove all entries that match the name, regardless of phone number 
	AddressEntryError RemoveEntry(const AddressEntry& entry, bool removeMatchingOnly = true);

	// Retrieve address in desired order
	AddressEntries RetrieveEntries(AddressEntryOrderType orderType);

	// Search addresses by first and last name
	AddressEntries Search(const std::string& searchKey, AddressEntrySearchType searchType);

	// Process each address entry in address book
	void ForEach(const AddressEntryCallback& callback);

	// Clear address book
	void Clear();
}
#endif // SIMPLE_ADDRESS_BOOK_H