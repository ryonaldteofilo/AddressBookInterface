//=======================================================
//		Includes
//=======================================================
#include "SimpleAddressBookTypes.h"





//=======================================================
//		AddressEntry::AddressEntry
//=======================================================
AddressEntry::AddressEntry()
{

}





//=======================================================
//		AddressEntry::AddressEntry
//=======================================================
AddressEntry::AddressEntry(const std::string& firstName, const std::string& lastName, const std::string& phoneNumber, const std::string& homeAddress) :
	mFirstName(firstName), mLastName(lastName), mPhoneNumber(phoneNumber)
{

};





//=======================================================
//		AddressEntry::operator==
//=======================================================
bool AddressEntry::operator==(const AddressEntry& rhs) const
{
	return mFirstName == rhs.mFirstName && mLastName == rhs.mLastName && mPhoneNumber == rhs.mPhoneNumber;
}





//=======================================================
//		operator<<
//=======================================================
std::ostream& operator<< (std::ostream& os, const AddressEntry& addressEntry)
{
	os << "First name: " << addressEntry.mFirstName
		<< "\nLast name: " << addressEntry.mLastName
		<< "\nPhone number: " << addressEntry.mPhoneNumber << std::endl;

	return os;
}





//=======================================================
//		operator>>
//=======================================================
std::istream& operator>> (std::istream& is, AddressEntry& addressEntry)
{
	std::cout << "Enter first name: ";
	is >> addressEntry.mFirstName;
	std::cout << "Enter last name: ";
	is >> addressEntry.mLastName;
	std::cout << "Enter phone number (optional): ";
	is >> addressEntry.mPhoneNumber;

	return is;
}
