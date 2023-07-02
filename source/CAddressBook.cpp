//=======================================================
//		Includes
//=======================================================
#include "CAddressBook.h"

// System
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
//		IsDigitOnly : Check if string is digits only
//=======================================================
static bool IsDigitOnly(const std::string& str)
{
    return std::find_if(str.cbegin(), str.cend(), [](const char& c) {return !isdigit(c); }) == str.cend();
}

//=======================================================
//		IsAlphaOnly : Check if string is alphabets only
//=======================================================
static bool IsAlphaOnly(const std::string& str)
{
    return std::find_if(str.cbegin(), str.cend(), [](const char& c) {return !isalpha(c); }) == str.cend();
}


//====================================================================
//		FirstNameAddressTrie
//====================================================================
FirstNameAddressTrie::FirstNameAddressTrie() :
    CAddressTrie()
{

}

//====================================================================
//		GetTrieKey : First name, then last name
//====================================================================
std::string FirstNameAddressTrie::GetTrieKey(const AddressEntry& addressEntry) const
{
    std::string key(addressEntry.mFirstName + addressEntry.mLastName);
    LowerCaseString(key);
    return key;
}

//====================================================================
//		LastNameAddressTrie
//====================================================================
LastNameAddressTrie::LastNameAddressTrie() :
    CAddressTrie()
{

}

//====================================================================
//		GetTrieKey : Last name, then first name
//====================================================================
std::string LastNameAddressTrie::GetTrieKey(const AddressEntry& addressEntry) const
{
    std::string key(addressEntry.mLastName + addressEntry.mFirstName);
    LowerCaseString(key);
    return key;
}

//====================================================================
//		CAddressBook
//====================================================================
CAddressBook::CAddressBook()
{

}

//====================================================================
//		AddEntry : Add address entry
//====================================================================
AddressEntryError CAddressBook::AddEntry(const AddressEntry& entry)
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
//		RemoveEntry : Remove address entry with option to remove only matching entries
//====================================================================
AddressEntryError CAddressBook::RemoveEntry(const AddressEntry& entry, bool removeMatchingOnly /* = true */)
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
//		RetrieveEntries : Retrieve address in desired order
//====================================================================
AddressEntries CAddressBook::RetrieveEntries(AddressEntryOrderType orderType) const
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
//		Search : Search address in desired search type
//====================================================================
AddressEntries CAddressBook::Search(const std::string& searchKey, 
                                    AddressEntrySearchType searchType /* = AddressEntrySearchType::FirstAndLastNameSearch */) const
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
//		ForEach : Process each entry in address book
//====================================================================
void CAddressBook::ForEach(const AddressEntryCallback& callback) const
{
    std::lock_guard<std::mutex> lock(mMutex);

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
//	    Reset : Clear address book
//====================================================================
void CAddressBook::Reset()
{
    std::lock_guard<std::mutex> lock(mMutex);

    mFirstNameTrie.Clear();
    mLastNameTrie.Clear();
}