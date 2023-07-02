#ifndef C_ADDRESS_BOOK_TRIE_H
#define C_ADDRESS_BOOK_TRIE_H
//=======================================================
//		Includes
//=======================================================
#include "AddressBookTypes.h"
#include "AddressBookCommon.h"

//=======================================================
//		Constants
//=======================================================
constexpr uint32_t kAddressTrieCharactersMax = 26;

//=======================================================
//		LowerCaseString : Lowercase a string
//=======================================================
void LowerCaseString(std::string& str);

//====================================================================
//		CAddressTrieNode : Address trie node
//====================================================================
struct CAddressTrieNode
{
	std::list<std::shared_ptr<AddressEntry>> mEntries;
	std::array<std::unique_ptr<CAddressTrieNode>, kAddressTrieCharactersMax> mCharacters;

	CAddressTrieNode() {}
	CAddressTrieNode(const CAddressTrieNode&) = delete;
	CAddressTrieNode& operator=(const CAddressTrieNode&) = delete;
};

//=======================================================
//		CAddressTrie : Trie holding address entries
//=======================================================
class CAddressTrie
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
	CAddressTrie();

	// Insert entry to trie
	AddressEntryError Insert(const AddressEntry& addressEntry);

	// Remove entry from trie
	AddressEntryError Remove(const AddressEntry& addressEntry,
							 bool matching = true);

	// Search for entry in trie
	AddressSearchResult Search(const std::string& searchKey,
							   const EntryPredicate& predicate = [](const AddressEntry&) {return true; }) const;

	// Retrieve entries in alphabetical order with optional predicate to filter returning entries
	AddressSearchResult AlphabeticOrder(const EntryPredicate& predicate = [](const AddressEntry&) {return true; }) const;

	// Pass in a function to iterate through each entry in trie
	void ForEach(const AddressEntryCallback& callback) const;

	// Clear address trie
	void Clear();

protected:
	// Determines how trie is sorted
	virtual std::string GetTrieKey(const AddressEntry& addressEntry) const = 0;

private:
	void PreOrderTraverse(CAddressTrieNode* currentNode,
						  AddressEntries& outAddresses,
						  AddressDuplicateLookup& duplicatesHash,
						  const EntryPredicate& predicate) const;
	
	void PreOrderTraverseForEach(CAddressTrieNode* currentNode,
								 const AddressEntryCallback& callback) const;

private:
	std::unique_ptr<CAddressTrieNode> mRootNode;
};
#endif // C_ADDRESS_BOOK_TRIE_H