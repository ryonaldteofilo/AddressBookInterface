//=======================================================
//		Includes
//=======================================================
#include "AddressBookInterface.h"
#include "CAddressBookManager.h"
#include "CAddressBook.h"

namespace AddressBookInterface
{
	//=======================================================
	//		AddEntry : Add an address entry to the address book
	//=======================================================
	AddressEntryError AddEntry(const AddressEntry& entry)
	{
		return CAddressBookManager::Get()->GetAddressBook()->AddEntry(entry);
	}

	//=======================================================
	//		RemoveEntry : Remove an address entry from the address book
	//					  if *match* is true, only entries that match exactly will be removed
	//=======================================================
	AddressEntryError RemoveEntry(const AddressEntry& entry, bool removeMatchingOnly /* = true */)
	{
		return CAddressBookManager::Get()->GetAddressBook()->RemoveEntry(entry, removeMatchingOnly);
	}

	//=======================================================
	//		RetrieveEntries : Retrieve entries in specified order
	//=======================================================
	AddressEntries RetrieveEntries(AddressEntryOrderType orderType)
	{
		return CAddressBookManager::Get()->GetAddressBook()->RetrieveEntries(orderType);
	}

	//=======================================================
	//		Search : Query for addresses in the address book with specified search type
	//=======================================================
	AddressEntries Search(const std::string& searchKey, AddressEntrySearchType searchType)
	{
		return CAddressBookManager::Get()->GetAddressBook()->Search(searchKey, searchType);
	}

	//=======================================================
	//		ForEach : Pass in function iteratively applied to each address entry in the book
	//=======================================================
	void ForEach(const AddressEntryCallback& callback)
	{
		CAddressBookManager::Get()->GetAddressBook()->ForEach(callback);
	}

	//=======================================================
	//		Clear : Clear address book
	//=======================================================
	void Clear()
	{
		CAddressBookManager::Get()->GetAddressBook()->Reset();
	}
}
