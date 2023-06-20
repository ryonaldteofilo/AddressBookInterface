//=======================================================
//		Includes
//=======================================================
#include "CAddressBookManager.h"





//=======================================================
//		CAddressBookManager
//=======================================================
CAddressBookManager::CAddressBookManager()
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
CSimpleAddressBook& CAddressBookManager::GetAddressBook()
{
	return mAddressBook;
}
