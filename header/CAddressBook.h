#ifndef C_ADDRESS_BOOK_H
#define C_ADDRESS_BOOK_H
//=======================================================
//		Includes
//=======================================================
#include "CAddressBookTrie.h"

//====================================================================
//		FirstNameAddressTrie : Address trie sorted in first name order
//====================================================================
class FirstNameAddressTrie : public CAddressTrie
{
public:
	// C-tor
	FirstNameAddressTrie();

private:
	virtual std::string GetTrieKey(const AddressEntry& addressEntry) const;
};

//====================================================================
//		AddressTrieNode : Address trie sorted in last name order
//====================================================================
class LastNameAddressTrie : public CAddressTrie
{
public:
	// C-tor
	LastNameAddressTrie();

private:
	virtual std::string GetTrieKey(const AddressEntry& addressEntry) const;
};

//=======================================================
//		SimpleAddressBook
//=======================================================
class CAddressBook
{
public:
	// C-tor
	CAddressBook();

	// Add address entry
	AddressEntryError AddEntry(const AddressEntry& entry);

	// Remove address entry with option to remove only matching entries
	AddressEntryError RemoveEntry(const AddressEntry& entry, bool matching = true);

	// Retrieve address in desired order
	AddressEntries RetrieveEntries(AddressEntryOrderType orderType) const;

	// Search address in desired search type
	AddressEntries Search(const std::string& searchKey, 
						  AddressEntrySearchType searchType = AddressEntrySearchType::FirstAndLastNameSearch) const;

	// Pass in a function to iterate through each entry in trie
	void ForEach(const AddressEntryCallback& callback) const;

	// Clear address book
	void Reset();

private:
	mutable std::mutex mMutex;

	// Trie sorted in first name order
	FirstNameAddressTrie mFirstNameTrie;

	// Trie sorted in last name order
	LastNameAddressTrie mLastNameTrie;
};
#endif // C_ADDRESS_BOOK_H