#ifndef C_SIMPLE_ADDRESS_BOOK_H
#define C_SIMPLE_ADDRESS_BOOK_H
//=======================================================
//		Includes
//=======================================================
#include "SimpleAddressBookTypes.h"
#include <mutex>
#include <array>
#include <unordered_set>





//=======================================================
//		Constants
//=======================================================
constexpr uint32_t kAddressTrieCharactersMax = 26;





//====================================================================
//		AddressTrieNode : Address trie node
//====================================================================
struct AddressTrieNode
{
	std::list<std::shared_ptr<AddressEntry>> mEntries;
	std::array<std::unique_ptr<AddressTrieNode>, kAddressTrieCharactersMax> mCharacters;

	AddressTrieNode();
	AddressTrieNode(const AddressTrieNode&) = delete;
	AddressTrieNode& operator=(const AddressTrieNode&) = delete;
};





//=======================================================
//		AddressTrie : An address book trie
//=======================================================
class AddressTrie
{
private:
	// To identify duplicates in entry
	using AddressDuplicateLookup = std::unordered_set<const AddressEntry*>;

public:
	// Criteria for entries
	using EntryPredicate = std::function<bool(const AddressEntry&)>;

	// Return from search/traversals
	using AddressSearchResult = std::pair<AddressDuplicateLookup, AddressEntries>;

public:
	// C-tor
	AddressTrie();

	// Insert node into trie
	AddressEntryError Insert(const AddressEntry& addressEntry);

	// Remove node from trie
	AddressEntryError Remove(const AddressEntry& addressEntry, 
							 bool removeMatchingOnly = true);

	// Search for entry in trie
	AddressSearchResult Search(const std::string& searchKey, 
							   const EntryPredicate& predicate = [](const AddressEntry&) {return true; }) const;

	// Retrieve entries in alphabetical order
	AddressSearchResult AlphabeticOrder(const EntryPredicate& predicate = [](const AddressEntry&) {return true; }) const;

	// Process each address entry in trie
	void ForEach(const AddressEntryCallback& callback) const;

	// Clear address trie
	void Clear();

protected:
	virtual std::string GetTrieKey(const AddressEntry& addressEntry) const = 0;

private:
	// Traverse trie in pre-order fashion
	void PreOrderTraverse(AddressTrieNode* currentNode, 
						  AddressEntries& outAddresses, 
						  AddressDuplicateLookup& duplicatesHash,
						  const EntryPredicate& predicate) const;

	void PreOrderTraverseForEach(AddressTrieNode* currentNode,
								 const AddressEntryCallback& callback) const;
	
private:
	std::unique_ptr<AddressTrieNode> mRootNode;
};





//====================================================================
//		FirstNameAddressTrie : Address entry sorted in first name order
//====================================================================
class FirstNameAddressTrie : public AddressTrie
{
public:
	// C-tor
	FirstNameAddressTrie();

private:
	virtual std::string GetTrieKey(const AddressEntry& addressEntry) const;
};





//====================================================================
//		AddressTrieNode : Address entry sorted in last name order
//====================================================================
class LastNameAddressTrie : public AddressTrie
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
class CSimpleAddressBook
{
public:
	// C-tor
	CSimpleAddressBook();

	// Add address entry
	AddressEntryError AddEntry(const AddressEntry& entry);

	// Find entry and remove if found - additional option to remove only matching entries
	AddressEntryError RemoveEntry(const AddressEntry& entry, bool removeMatchingOnly = true);

	// Retrieve address in desired order
	AddressEntries RetrieveEntries(AddressEntryOrderType orderType) const;

	// Search addresses by first and last name
	AddressEntries Search(const std::string& searchKey, AddressEntrySearchType searchType) const;

	// Process each entry in specified order
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
#endif // C_SIMPLE_ADDRESS_BOOK_H
