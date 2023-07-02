//=======================================================
//		Includes
//=======================================================
#include "AddressBookTypes.h"

//=======================================================
//		operator<<
//=======================================================
std::ostream& operator<<(std::ostream& os, const AddressEntry& addressEntry)
{
	os << "First name: " << addressEntry.mFirstName
		<< "\nLast name: " << addressEntry.mLastName
		<< "\nPhone number: " << addressEntry.mPhoneNumber << std::endl;

	return os;
}

//=======================================================
//		operator>>
//=======================================================
std::istream& operator>>(std::istream& is, AddressEntry& addressEntry)
{
	std::cout << "Enter first name: ";
	is >> addressEntry.mFirstName;
	std::cout << "Enter last name: ";
	is >> addressEntry.mLastName;
	std::cout << "Enter phone number (optional): ";
	is >> addressEntry.mPhoneNumber;

	return is;
}