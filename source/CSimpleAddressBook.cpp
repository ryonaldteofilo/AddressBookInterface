//=======================================================
//		Includes
//=======================================================
#include "CSimpleAddressBook.h"
#include <cctype>
#include <signal.h>





//=======================================================
//		DebugBreak
//=======================================================
static void DebugBreak()
{
#if defined (_MSC_VER)
    __debugbreak();
#elif defined(SIGTRAP)
    raise(SIGTRAP);
#endif
}





//=======================================================
//		ToLowerCase : Lowercase all characters in a string
//=======================================================
static void ToLowerCase(std::string& str)
{
    for (char& c : str)
    {
        c = tolower(c);
    }
}





//=======================================================
//		IsDigitOnly : Query if characters in string are digits only
//=======================================================
static bool IsDigitOnly(const std::string& str)
{
    return std::find_if(str.cbegin(), str.cend(), [](const char& c) {return !isdigit(c); }) == str.cend();
}





//=======================================================
//		IsDigitOnly : Query if characters in string are alphabets only
//=======================================================
static bool IsAlphaOnly(const std::string& str)
{
    return std::find_if(str.cbegin(), str.cend(), [](const char& c) {return !isalpha(c); }) == str.cend();
}





//=======================================================
//		AddressTrieNode::AddressTrieNode
//=======================================================
AddressTrieNode::AddressTrieNode()
{

}





//====================================================================
//		FirstNameAddressTrie::FirstNameAddressTrie
//====================================================================
FirstNameAddressTrie::FirstNameAddressTrie() :
    AddressTrie()
{

}





//====================================================================
//		FirstNameAddressTrie::GetTrieKey
//====================================================================
std::string FirstNameAddressTrie::GetTrieKey(const AddressEntry& addressEntry) const
{
    std::string key(addressEntry.mFirstName + addressEntry.mLastName);
    ToLowerCase(key);

    return key;
}




//====================================================================
//		LastNameAddressTrie::LastNameAddressTrie
//====================================================================
LastNameAddressTrie::LastNameAddressTrie() :
    AddressTrie()
{

}





//====================================================================
//		LastNameAddressTrie::GetTrieKey
//====================================================================
std::string LastNameAddressTrie::GetTrieKey(const AddressEntry& addressEntry) const
{
    std::string key(addressEntry.mLastName + addressEntry.mFirstName);
    ToLowerCase(key);

    return key;
}





//=======================================================
//		AddressTrie::AddressTrie
//=======================================================
AddressTrie::AddressTrie() :
    mRootNode(new AddressTrieNode)
{

}





//=======================================================
//		AddressTrie::Insert : Insert entry to trie
//=======================================================
AddressEntryError AddressTrie::Insert(const AddressEntry& addressEntry)
{
    // Get key
    std::string key(GetTrieKey(addressEntry));

    // Ensure key is not empty
    if (key.empty())
    {
        return AddressEntryError::kAddressEntryInvalid;
    }

    // Traverse trie 
    AddressTrieNode* currentNode = mRootNode.get();
    for (const char& c : key)
    {
        // Determine index (ascii 'a' is 97)
        int index = c - 97;

        // Allocate nodes until we reach our desired point in the trie
        if (currentNode->mCharacters[index].get() == nullptr)
        {
            currentNode->mCharacters[index].reset(new AddressTrieNode);
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
//		AddressTrie::Remove : Remove entry from trie
//====================================================================
AddressEntryError AddressTrie::Remove(const AddressEntry& addressEntry, 
                                      bool removeMatchingOnly /* = true */)
{
    // Get key
    std::string key(GetTrieKey(addressEntry));

    // Ensure key is not empty
    if (key.empty())
    {
        return AddressEntryError::kAddressEntryInvalid;
    }

    // Traverse trie
    AddressTrieNode* currentNode = mRootNode.get();
    for (const char& c : key)
    {
        // Determine index (ascii 'a' is 97)
        int index = c - 97;

        // If no node is present means we don't have the entry
        AddressTrieNode* nextNode = currentNode->mCharacters[index].get();
        if (nextNode == nullptr)
        {
            return AddressEntryError::kAddressEntryNotFound;
        }

        currentNode = nextNode;
    }

    if (!currentNode->mEntries.empty())
    {
        if (removeMatchingOnly)
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
//		AddressTrie::Search : 
//====================================================================
AddressTrie::AddressSearchResult AddressTrie::Search(const std::string& searchKey, 
                                                     const EntryPredicate& predicate /* = [](const AddressEntry&) {return true; } */) const
{
    // Lower case key
    std::string key(searchKey);
    ToLowerCase(key);

    // Initialise result
    AddressSearchResult result;

    // Traverse to key
    AddressTrieNode* currentNode = mRootNode.get();
    for (const char& c : key)
    {
        // Determine index (ascii 'a' is 97)
        int index = c - 97;

        // If no node is present means we don't have the entry
        AddressTrieNode* nextNode = currentNode->mCharacters[index].get();
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
//		AddressTrie::AlphabeticOrder : Retrieve entries in alphabetical order 
//====================================================================
AddressTrie::AddressSearchResult AddressTrie::AlphabeticOrder(const EntryPredicate& predicate /* = [](const AddressEntry&) {return true; } */) const
{
    // Traverse and return
    AddressSearchResult result;
    PreOrderTraverse(mRootNode.get(), result.second, result.first, predicate);

    return result;
}





//====================================================================
//		AddressTrie::ForEach : Process each entry in trie
//====================================================================
void AddressTrie::ForEach(const AddressEntryCallback& callback) const
{
    PreOrderTraverseForEach(mRootNode.get(), callback);
}





//====================================================================
//		AddressTrie::Clear : Clear address trie
//====================================================================
void AddressTrie::Clear()
{
    mRootNode.reset(new AddressTrieNode);
}





//====================================================================
//		AddressTrie::PreOrderTraverse : Traverse trie in preorder DFS
//====================================================================
void AddressTrie::PreOrderTraverse(AddressTrieNode* currentNode,
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
        AddressTrieNode* nextNode = currentNode->mCharacters[i].get();
        if (nextNode != nullptr)
        {
            PreOrderTraverse(nextNode, outAddresses, duplicatesHash, predicate);
        }
    }
}





//====================================================================
//		AddressTrie::PreOrderTraverse : Process each entry in preorder dfs
//====================================================================
void AddressTrie::PreOrderTraverseForEach(AddressTrieNode* currentNode,
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
        AddressTrieNode* nextNode = currentNode->mCharacters[i].get();
        if (nextNode != nullptr)
        {
            PreOrderTraverseForEach(nextNode, callback);
        }
    }
}





//====================================================================
//		CSimpleAddressBook::CSimpleAddressBook
//====================================================================
CSimpleAddressBook::CSimpleAddressBook()
{

}





//====================================================================
//		CSimpleAddressBook::AddEntry : Add entry to address book
//====================================================================
AddressEntryError CSimpleAddressBook::AddEntry(const AddressEntry& entry)
{
    // Ensure either first or last name is not empty, and phone number is valid
    if (entry.mFirstName.empty() && entry.mLastName.empty() || !IsDigitOnly(entry.mPhoneNumber))
    {
        return AddressEntryError::kAddressEntryInvalid;
    }

    {
        std::lock_guard<std::mutex> lock(mMutex);

        AddressEntryError firstNameResult = AddressEntryError::kAddressEntryNotAttempted;
        AddressEntryError lastNameResult = AddressEntryError::kAddressEntryNotAttempted;

        // Add to first name trie depending on entry
        if (!entry.mFirstName.empty())
        {
            firstNameResult = mFirstNameTrie.Insert(entry);
        }
        
        // Return if we attempted and failed
        if (firstNameResult != AddressEntryError::kAddressEntryNotAttempted && 
            firstNameResult != AddressEntryError::kAddressEntrySuccess)
        {
            return firstNameResult;
        }

        // Add to last name trie depending on entry
        if (!entry.mLastName.empty())
        {
            lastNameResult = mLastNameTrie.Insert(entry);
        }

        // Return if we attempted and failed
        if (lastNameResult != AddressEntryError::kAddressEntryNotAttempted &&
            lastNameResult != AddressEntryError::kAddressEntrySuccess)
        {
            // If we attempted on inserting, check if we've also attempted on the other trie
            if (firstNameResult == AddressEntryError::kAddressEntrySuccess)
            {
                DebugBreak(); // this shouldn't happen

                // Remove if insertion succeeded
                mFirstNameTrie.Remove(entry);
            }

            return lastNameResult;
        }

        return AddressEntryError::kAddressEntrySuccess;
    }
}





//====================================================================
//		CSimpleAddressBook::RemoveEntry : Remove entry from address book
//====================================================================
AddressEntryError CSimpleAddressBook::RemoveEntry(const AddressEntry& entry, bool removeMatchingOnly /* = true */)
{
    // Ensure either first or last name is not empty, and phone number is valid
    if (entry.mFirstName.empty() && entry.mLastName.empty() || !IsDigitOnly(entry.mPhoneNumber))
    {
        return AddressEntryError::kAddressEntryInvalid;
    }

    {
        std::lock_guard<std::mutex> lock(mMutex);

        AddressEntryError firstNameResult = AddressEntryError::kAddressEntryNotAttempted;
        AddressEntryError lastNameResult = AddressEntryError::kAddressEntryNotAttempted;

        // Remove from first name trie depending on entry
        if (!entry.mFirstName.empty())
        {
            firstNameResult = mFirstNameTrie.Remove(entry, removeMatchingOnly);
        }

        // Return if we attempted and failed
        if (firstNameResult != AddressEntryError::kAddressEntryNotAttempted &&
            firstNameResult != AddressEntryError::kAddressEntrySuccess)
        {
            return firstNameResult;
        }

        // Remove from last name trie depending on entry
        if (!entry.mLastName.empty())
        {
            lastNameResult = mLastNameTrie.Remove(entry, removeMatchingOnly);
        }

        // Return if we attempted and failed
        if (lastNameResult != AddressEntryError::kAddressEntryNotAttempted &&
            lastNameResult != AddressEntryError::kAddressEntrySuccess)
        {
            // If we attempted on removing, check if we've also attempted on the other trie
            if (firstNameResult == AddressEntryError::kAddressEntrySuccess)
            {
                DebugBreak(); // this shouldn't happen

                // Re-insert if removal succeeded
                mFirstNameTrie.Insert(entry);
            }

            return lastNameResult;
        }

        return AddressEntryError::kAddressEntrySuccess;
    }
}




//====================================================================
//		CSimpleAddressBook::RetrieveEntries : Retrieve entries in sorted order
//====================================================================
AddressEntries CSimpleAddressBook::RetrieveEntries(AddressEntryOrderType orderType) const
{
    std::lock_guard<std::mutex> lock(mMutex);
    
    // Populate result
    AddressEntries result;
    switch (orderType)
    {
    case AddressEntryOrderType::FirstNameOrder:
    {
        // Add all entries without a first name
        result.splice(result.cend(), 
                      mLastNameTrie.AlphabeticOrder([](const AddressEntry& entry) { return entry.mFirstName.empty(); }).second);

        // Add all entries with first name
        result.splice(result.cend(),
                      mFirstNameTrie.AlphabeticOrder().second);

        break;
    }
    case AddressEntryOrderType::LastNameOrder:
    {
        // Add all entries without a last name
        result.splice(result.cend(),
                      mFirstNameTrie.AlphabeticOrder([](const AddressEntry& entry) { return entry.mLastName.empty(); }).second);

        // Add all entries with last name name
        result.splice(result.cend(),
                      mLastNameTrie.AlphabeticOrder().second);

        break;
    }
    default:
        DebugBreak();
        break;
    }

    return result;
}





//====================================================================
//		CSimpleAddressBook::Search : Search address entries
//====================================================================
AddressEntries CSimpleAddressBook::Search(const std::string& searchKey, AddressEntrySearchType searchType) const
{
    if (IsAlphaOnly(searchKey))
    {
        std::lock_guard<std::mutex> lock(mMutex);

        switch (searchType)
        {
        case AddressEntrySearchType::FirstNameSearch:
        {
            return mFirstNameTrie.Search(searchKey).second;
        }
        case AddressEntrySearchType::LastNameSearch:
        {
            return mLastNameTrie.Search(searchKey).second;
        }

        case AddressEntrySearchType::FirstAndLastNameSearch:
        {
            AddressEntries result;
            auto searchResult = mFirstNameTrie.Search(searchKey);
            result.splice(result.cend(), searchResult.second);

            // Ensure there are no duplicates in output result
            const auto& duplicateLookup = searchResult.first;
            result.splice(result.cend(), mLastNameTrie.Search(searchKey, [duplicateLookup](const AddressEntry& entry)->bool
                {
                    return (duplicateLookup.find(&entry) == duplicateLookup.end());
                }).second);

            return result;
        }

        default:
            DebugBreak();
            break;
        }
    }

    return AddressEntries();
}





//====================================================================
//		CSimpleAddressBook::ForEach : Process each entry in address book
//====================================================================
void CSimpleAddressBook::ForEach(const AddressEntryCallback& callback) const
{
    // Ensure each entry is call on exactly once
    std::unordered_set<const AddressEntry*> duplicateLookup;
    mFirstNameTrie.ForEach([&duplicateLookup, &callback](const AddressEntry& entry)
        {
            duplicateLookup.emplace(&entry);
            callback(entry);
        });

    mLastNameTrie.ForEach([&duplicateLookup, &callback](const AddressEntry& entry)
        {
            if (duplicateLookup.find(&entry) == duplicateLookup.end())
            {
                callback(entry);
            }
        });
}





//====================================================================
//		CSimpleAddressBook::Reset : Clear address book
//====================================================================
void CSimpleAddressBook::Reset()
{
    mFirstNameTrie.Clear();
    mLastNameTrie.Clear();
}