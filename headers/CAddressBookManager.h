#ifndef C_ADDRESS_BOOK_MANAGER_H
#define C_ADDRESS_BOOK_MANAGER_H
//=======================================================
//		Includes
//=======================================================
#include "CSimpleAddressBook.h"





//=======================================================
//		CAddressBookManager : Manager class for address book
//=======================================================
class CAddressBookManager
{
private:
	CAddressBookManager();
	CAddressBookManager(const CAddressBookManager&) = delete;
	CAddressBookManager& operator=(const CAddressBookManager&) = delete;

public:
	// Get manager singleton
	static CAddressBookManager* Get();

	// Get address book
	CSimpleAddressBook& GetAddressBook();

private:
	CSimpleAddressBook mAddressBook;
};
#endif // C_ADDRESS_BOOK_MANAGER_H