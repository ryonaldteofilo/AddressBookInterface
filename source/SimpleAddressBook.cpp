//=======================================================
//		Includes
//=======================================================
#include "SimpleAddressBook.h"
#include "CAddressBookManager.h"





//=======================================================
//		SimpleAddressBook::AddEntry
//=======================================================
AddressEntryError SimpleAddressBook::AddEntry(const AddressEntry& entry)
{
	return CAddressBookManager::Get()->GetAddressBook().AddEntry(entry);
}





//=======================================================
//		SimpleAddressBook::RemoveEntry
//=======================================================
AddressEntryError SimpleAddressBook::RemoveEntry(const AddressEntry& entry, bool removeMatchingOnly /* = true */)
{
	return CAddressBookManager::Get()->GetAddressBook().RemoveEntry(entry, removeMatchingOnly);
}





//=======================================================
//		SimpleAddressBook::RetrieveEntries
//=======================================================
AddressEntries SimpleAddressBook::RetrieveEntries(AddressEntryOrderType orderType)
{
	return CAddressBookManager::Get()->GetAddressBook().RetrieveEntries(orderType);
}





//=======================================================
//		SimpleAddressBook::Search
//=======================================================
AddressEntries SimpleAddressBook::Search(const std::string& searchKey, AddressEntrySearchType searchType)
{
	return CAddressBookManager::Get()->GetAddressBook().Search(searchKey, searchType);
}





//=======================================================
//		SimpleAddressBook::ForEach
//=======================================================
void SimpleAddressBook::ForEach(const AddressEntryCallback& callback)
{
	CAddressBookManager::Get()->GetAddressBook().ForEach(callback);
}





//=======================================================
//		SimpleAddressBook::Clear
//=======================================================
void SimpleAddressBook::Clear()
{
	CAddressBookManager::Get()->GetAddressBook().Reset();
}
