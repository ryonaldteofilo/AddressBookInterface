#ifndef ADDRESS_BOOK_INTERFACE_H
#define ADDRESS_BOOK_INTERFACE_H
//=======================================================
//		Includes
//=======================================================
#include "AddressBookTypes.h"

//=======================================================
//		AddressBookInterface
//=======================================================
namespace AddressBookInterface
{
	// Add an address entry to the address book
	AddressEntryError AddEntry(const AddressEntry& entry);

	// Remove an address entry from the address book
	// if *match* is true, only entries that match exactly will be removed
	AddressEntryError RemoveEntry(const AddressEntry& entry, 
								  bool match = true);

	// Retrieve entries in specified order
	AddressEntries RetrieveEntries(AddressEntryOrderType orderType);

	// Query for addresses in the address book with specified search type
	AddressEntries Search(const std::string& searchKey, 
						  AddressEntrySearchType searchType = AddressEntrySearchType::FirstAndLastNameSearch);

	// Pass in function iteratively applied to each address entry in the book
	void ForEach(const AddressEntryCallback& callback);

	// Clear address book
	void Clear();
}
#endif // ADDRESS_BOOK_INTERFACE_H