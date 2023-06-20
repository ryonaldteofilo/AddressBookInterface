//=======================================================
//		Includes
//=======================================================
#include "SimpleAddressBook.h"





//=======================================================
//		ClearAndResetInputStream : reset input stream
//=======================================================
void ClearAndResetInputStream()
{
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}





//=======================================================
//		GetAddressEntryOrderType : Get entry search type
//=======================================================
const AddressEntryOrderType GetAddressEntryOrderType()
{
	while (true)
	{
		std::cout << "Please choose from the following order types:\n1. First Name Order\n2. Last Name Order" << std::endl;

		ClearAndResetInputStream();
		int option = 0;
		if (std::cin >> option)
		{
			switch (option)
			{
			case 1:
				return AddressEntryOrderType::FirstNameOrder;
			case 2:
				return AddressEntryOrderType::LastNameOrder;
			default:
				std::cout << "Invalid order type, please try again" << std::endl;
				break;
			}
		}
		else
		{
			ClearAndResetInputStream();
		}
	}
}





//=======================================================
//		GetEntrySearchType : Get entry search type
//=======================================================
const AddressEntrySearchType GetEntrySearchType()
{
	while (true)
	{
		std::cout << "Please choose from the following search types:\n1. First Name Search\n2. Last Name Search\n3. First and Last Name Search" << std::endl;
		
		int option = 0;
		if (std::cin >> option)
		{
			switch (option)
			{
			case 1:
				return AddressEntrySearchType::FirstNameSearch;
			case 2:
				return AddressEntrySearchType::LastNameSearch;
			case 3:
				return AddressEntrySearchType::FirstAndLastNameSearch;
			default:
				std::cout << "Invalid search type, please try again" << std::endl;
				break;
			}
		}
		else
		{
			ClearAndResetInputStream();
		}
	}
}





//=======================================================
//		ToString : Convert AddressEntryError to human readable string
//=======================================================
const std::string ToString(const AddressEntryError& error)
{
	switch (error)
	{
	case AddressEntryError::kAddressEntrySuccess:
		return "Success!";

	case AddressEntryError::kAddressEntryDuplicate:
		return "Duplicate found!";

	case AddressEntryError::kAddressEntryInvalid:
		return "Entry invalid!";

	case AddressEntryError::kAddressEntryNotFound:
		return "Entry not found!";

	case AddressEntryError::kAddressEntryNotAttempted:
		return "Action not attempted";

	default:
		return std::string();
	}
}





//=======================================================
//		main
//=======================================================
int main()
{
	std::cout << "Hello, welcome to the Simple Address Book demo!" << std::endl;

	while (true)
	{
		std::cout << "\nPlease choose from the following options below:\n"
			<< "1. Add entry\n2. Remove entry\n3. Show entries\n4. Search entries\n5. Clear entries\n0. Exit\n\n";

		int option = 0;
		if (std::cin >> option)
		{
			switch (option)
			{
			case 1:
			{
				std::cout << "Provide address entry to add:" << std::endl;
				AddressEntry entryToAdd;
				if (std::cin >> entryToAdd)
				{
					auto result = SimpleAddressBook::AddEntry(entryToAdd);
					std::cout << ToString(result);
				}
				break;
			}
			case 2:
			{
				std::cout << "Provide address entry to remove:" << std::endl;
				AddressEntry entryToRemove;
				if (std::cin >> entryToRemove)
				{
					auto result = SimpleAddressBook::RemoveEntry(entryToRemove);
					std::cout << ToString(result);
				}
				break;
			}
			case 3:
			{
				auto result = SimpleAddressBook::RetrieveEntries(GetAddressEntryOrderType());
				if (result.empty())
				{
					std::cout << "Address book is empty!" << std::endl;
				}
				else
				{
					std::cout << "Entries as follows:" << std::endl;
					for (const auto& entry : result)
					{
						std::cout << entry;
					}
				}
				break;
			}

			case 4:
			{
				std::cout << "Please enter search key: ";

				std::string input;
				if (std::cin >> input)
				{
					auto result = SimpleAddressBook::Search(input, GetEntrySearchType());

					if (result.empty())
					{
						std::cout << "No entries found!" << std::endl;
					}
					else
					{
						std::cout << "Entries found:" << std::endl;
						for (const auto& entry : result)
						{
							std::cout << entry;
						}
					}
				}
				break;
			}

			case 5:
			{
				SimpleAddressBook::Clear();
				std::cout << "Address book cleared!" << std::endl;
				break;
			}

			case 0:
			{
				std::cout << "Thank you for trying this demo, bye!" << std::endl;
				return 0;
			}

			default:
			{
				std::cout << "Invalid option!" << std::endl;
				break;
			}
			}
		}
		else
		{
			std::cout << "Invalid input entered, please try again." << std::endl;
			ClearAndResetInputStream();
		}
	}

	return 0;
}