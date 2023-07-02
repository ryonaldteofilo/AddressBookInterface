//=======================================================
//		Includes
//=======================================================
#include "CAddressBookTrie.h"

//=======================================================
//		LowerCaseString : Lowercase a string
//=======================================================
void LowerCaseString(std::string& str)
{
    for (char& c : str) { c = tolower(c); }
}

//=======================================================
//		CAddressTrie
//=======================================================
CAddressTrie::CAddressTrie() :
    mRootNode(new CAddressTrieNode)
{

}

//=======================================================
//		Insert : Insert entry to trie
//=======================================================
AddressEntryError CAddressTrie::Insert(const AddressEntry& addressEntry)
{
    // Get key
    std::string key(GetTrieKey(addressEntry));

    // Ensure key is not empty
    if (key.empty())
    {
        return AddressEntryError::kAddressEntryInvalid;
    }

    // Traverse trie 
    CAddressTrieNode* currentNode = mRootNode.get();
    for (const char& c : key)
    {
        // Determine index (ascii 'a' is 97)
        int index = c - 97;

        // Allocate nodes until we reach our desired point in the trie
        if (currentNode->mCharacters[index].get() == nullptr)
        {
            currentNode->mCharacters[index].reset(new CAddressTrieNode);
        }

        currentNode = currentNode->mCharacters[index].get();
    }

    // Check for duplicate
    for (const auto& entry : currentNode->mEntries)
    {
        if (*entry.get() == addressEntry)
        {
            return AddressEntryError::kAddressEntryDuplicate;
        }
    }

    // Add entry to node
    currentNode->mEntries.emplace_back(new AddressEntry(addressEntry));
    return AddressEntryError::kAddressEntrySuccess;
}

//====================================================================
//		Remove : Remove entry from trie
//====================================================================
AddressEntryError CAddressTrie::Remove(const AddressEntry& addressEntry,
                                       bool matching /* = true */)
{
    // Get key
    std::string key(GetTrieKey(addressEntry));

    // Ensure key is not empty
    if (key.empty())
    {
        return AddressEntryError::kAddressEntryInvalid;
    }

    // Traverse trie
    CAddressTrieNode* currentNode = mRootNode.get();
    for (const char& c : key)
    {
        // Determine index (ascii 'a' is 97)
        int index = c - 97;

        // If no node is present means we don't have the entry
        CAddressTrieNode* nextNode = currentNode->mCharacters[index].get();
        if (nextNode == nullptr)
        {
            return AddressEntryError::kAddressEntryNotFound;
        }

        currentNode = nextNode;
    }

    if (!currentNode->mEntries.empty())
    {
        if (matching)
        {
            // Look for matching entry(ies)
            bool removed = false;
            for (auto it = currentNode->mEntries.begin(); it != currentNode->mEntries.end();)
            {
                if (*it->get() == addressEntry)
                {
                    currentNode->mEntries.erase(it);

                    // Indicate we've removed an entry
                    if (!removed)
                    {
                        removed = true;
                    }

                    continue;
                }
                it++;
            }
            return removed ? AddressEntryError::kAddressEntrySuccess : AddressEntryError::kAddressEntryNotFound;
        }
        else
        {
            // Clear all entries
            currentNode->mEntries.clear();
            return AddressEntryError::kAddressEntrySuccess;
        }
    }

    return AddressEntryError::kAddressEntryNotFound;
}

//====================================================================
//		Search : Search for entry in trie
//====================================================================
CAddressTrie::AddressSearchResult CAddressTrie::Search(const std::string& searchKey,
    const EntryPredicate& predicate /* = [](const AddressEntry&) {return true; } */) const
{
    // Lower case key
    std::string key(searchKey);
    LowerCaseString(key);

    // Initialise result
    AddressSearchResult result;

    // Traverse to key
    CAddressTrieNode* currentNode = mRootNode.get();
    for (const char& c : key)
    {
        // Determine index (ascii 'a' is 97)
        int index = c - 97;

        // If no node is present means we don't have the entry
        CAddressTrieNode* nextNode = currentNode->mCharacters[index].get();
        if (nextNode == nullptr)
        {
            return result;
        }

        currentNode = nextNode;
    }

    // Add current entry(ies) that passes predicate
    for (const auto& entry : currentNode->mEntries)
    {
        if (predicate(*entry.get()))
        {
            result.second.emplace_back(*entry.get());
            result.first.emplace(entry.get());
        }
    }

    // Populate with entries prefixed by specified key
    PreOrderTraverse(currentNode, result.second, result.first, predicate);

    return result;
}

//====================================================================
//		AlphabeticOrder : Retrieve entries in alphabetical order 
//                                      with optional predicate to filter returning entries
//====================================================================
CAddressTrie::AddressSearchResult CAddressTrie::AlphabeticOrder(const EntryPredicate& predicate /* = [](const AddressEntry&) {return true; } */) const
{
    // Traverse and return
    AddressSearchResult result;
    PreOrderTraverse(mRootNode.get(), result.second, result.first, predicate);

    return result;
}

//====================================================================
//		ForEach : Pass in a function to iterate through each entry in trie
//====================================================================
void CAddressTrie::ForEach(const AddressEntryCallback& callback) const
{
    PreOrderTraverseForEach(mRootNode.get(), callback);
}

//====================================================================
//		Clear : Clear trie
//====================================================================
void CAddressTrie::Clear()
{
    mRootNode.reset(new CAddressTrieNode);
}

//====================================================================
//		PreOrderTraverse : Traverse trie in preorder DFS
//====================================================================
void CAddressTrie::PreOrderTraverse(CAddressTrieNode* currentNode,
                                    AddressEntries& outAddresses,
                                    AddressDuplicateLookup& duplicatesHash,
                                    const EntryPredicate& predicate) const
{
    // Add entries that passes predicate and populate duplicate lookup
    for (const auto& entry : currentNode->mEntries)
    {
        if (predicate(*entry.get()))
        {
            outAddresses.emplace_back(*entry.get());
            duplicatesHash.emplace(entry.get());
        }
    }

    // Go through all nodes
    for (int i = 0; i < kAddressTrieCharactersMax; i++)
    {
        CAddressTrieNode* nextNode = currentNode->mCharacters[i].get();
        if (nextNode != nullptr)
        {
            PreOrderTraverse(nextNode, outAddresses, duplicatesHash, predicate);
        }
    }
}

//====================================================================
//		PreOrderTraverse : Process each entry in preorder DFS
//====================================================================
void CAddressTrie::PreOrderTraverseForEach(CAddressTrieNode* currentNode,
                                           const AddressEntryCallback& callback) const
{
    // Process entries in current node
    for (const auto& entry : currentNode->mEntries)
    {
        callback(*entry.get());
    }

    // Go through all nodes
    for (int i = 0; i < kAddressTrieCharactersMax; i++)
    {
        CAddressTrieNode* nextNode = currentNode->mCharacters[i].get();
        if (nextNode != nullptr)
        {
            PreOrderTraverseForEach(nextNode, callback);
        }
    }
}