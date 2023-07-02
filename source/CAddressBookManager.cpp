//=======================================================
//		Includes
//=======================================================
#include "CAddressBookManager.h"
#include "CAddressBook.h"

//=======================================================
//		CAddressBookManager
//=======================================================
CAddressBookManager::CAddressBookManager() :
	mpAddressBook(new CAddressBook)
{

}

//=======================================================
//		Get : Get singleton
//=======================================================
CAddressBookManager* CAddressBookManager::Get()
{
	static CAddressBookManager instance;
	return &instance;
}

//=======================================================
//		GetAddressBook : Get address book
//=======================================================
CAddressBook* CAddressBookManager::GetAddressBook()
{
	return mpAddressBook.get();
}
