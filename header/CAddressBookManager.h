#ifndef C_ADDRESS_BOOK_MANAGER_H
#define C_ADDRESS_BOOK_MANAGER_H
//=======================================================
//		Includes
//=======================================================
#include "AddressBookCommon.h"

//=======================================================
//		Forward declaration
//=======================================================
class CAddressBook;

//=======================================================
//		CAddressBookManager : Manager class for address book(s)
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
	CAddressBook* GetAddressBook();

private:
	std::unique_ptr<CAddressBook> mpAddressBook;
};
#endif // C_ADDRESS_BOOK_MANAGER_H