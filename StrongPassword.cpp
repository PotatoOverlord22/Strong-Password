#include <iostream>

// The main function computing the solution
int editNumberStrongPassword(std::string& password);

// Used to calculate the number missing special characters of a string (special character: uppercase, lowercase, digit)
int countMissingSpecialCharacters(std::string& password);

// Used to calculate the number of replace operations needed to fix repetitive character groups from the string
int countReplaceForRepetitiveGroups(std::string& password);

// Used to calculate the most optimal number of delete operations needed to fix repetitive character groups
void countDeleteForRepetitiveGroups(std::string& password, int& shortGroupDeletions, int& mediumGroupDeletions, int& longGroupDeletions);


int main() {
    std::string originalPassword;
    std::cout << "\nInput password" << "\n > ";
    std::cin >> originalPassword;
    std::cout << "The minimum number of edits to make it a strong password is: \n"
              << editNumberStrongPassword(originalPassword);
    return 0;
}

int editNumberStrongPassword(std::string& password) {
    /*      Computes the number of operations (insert, remove, replace) needed for a string to reach the status of
     *   'Strong Password'
     *   Strong password --> string has the length between 6 and 20 characters long, has at least one uppercase, lowercase
     *                  character and a digit, and does not contain groups of more than 3 repeating characters
     *  Return -> (int) operation count
     */
    // Define the minimum and maximum amount of characters a password needs to have
    const int LOWER_PASSWORD_BOUND = 6;
    const int UPPER_PASSWORD_BOUND = 20;
    /// Case 1: Len < 6
    if (password.length() < LOWER_PASSWORD_BOUND) {
        // Count the number of missing special characters needed to make the password strong
        int missingSpecialCharactersCount = countMissingSpecialCharacters(password);
        // Count the number of characters needed to be added to make the password strong
        int charactersNeededForLengthCount = LOWER_PASSWORD_BOUND - (int) password.length();
        return std::max(charactersNeededForLengthCount, missingSpecialCharactersCount);
    }
        /// Case 2: 6 <= Len <= 20
    else if (password.length() <= UPPER_PASSWORD_BOUND) {
        // Count the number of missing special characters needed to make the password strong
        int numberOfRequiredSpecialCharacters = countMissingSpecialCharacters(password);
        // Count the number of replace operations needed to fix groups of repetitive characters
        int numberOfRequiredReplaces = countReplaceForRepetitiveGroups(password);
        return std::max(numberOfRequiredSpecialCharacters, numberOfRequiredReplaces);
    }
        /// Case 3: Len > 20
    else {

        // This is the number of deletions we HAVE TO make
        int requiredDeletions = (int) password.length() - UPPER_PASSWORD_BOUND;
        // The number of optimal deletions we need to make
        int smartDeletions = requiredDeletions;
        // This is the number of optimal deletions used for groups of len % 3 == 0 (type 3n)
        int shortGroupDeletions;
        // This is the number of optimal deletions used for groups of len % 3 == 1 (type 3n + 1)
        int mediumGroupDeletions;
        // This is the number of optimal deletions used for groups of len % 3 == 2 (type 3n + 2)
        int longGroupDeletions;
        // Fill the number of deletions used for each repetitive group
        countDeleteForRepetitiveGroups(password, shortGroupDeletions, mediumGroupDeletions, longGroupDeletions);
        // The number of total replacements used to solve all the repetitive groups in the string
        int totalReplacements = countReplaceForRepetitiveGroups(password);
        // The number of replacements saved by smartly using delete operations on each group
        int savedReplacements = 0;
        // Go through each case of possible smart deletion by
        if (smartDeletions > 0) {
            // Don't save more replacements than we need to
            savedReplacements += std::min(smartDeletions, shortGroupDeletions);
            smartDeletions -= shortGroupDeletions;
        }
        // If we still need to make smart deletions, move to the second most optimal deletes
        if (smartDeletions > 0) {
            // The ratio for the medium type of group is 2 deletes for saving 1 replacement
            savedReplacements += std::min(smartDeletions, mediumGroupDeletions) / 2;
            smartDeletions -= mediumGroupDeletions;
        }
        // Still need to do smart deletions, move to worst case for optimal deletions
        if (smartDeletions > 0) {
            // The ratio for long type of group is 3 deletes for saving 1 replacement, and in smart deletions we
            // have only the deletions from the long type of group left
            savedReplacements += smartDeletions / 3;
        }
        // Remove the replacements saved by the above deletes
        totalReplacements -= savedReplacements;
        // Need missing characters to calculate the solution once we go into Case 2
        int missingSpecialCharactersCount = countMissingSpecialCharacters(password);
        // Result is the deletions we made and the solution with the string now in Case 2 with updated number of replacements
        return requiredDeletions + std::max(missingSpecialCharactersCount, totalReplacements);
    }
}

int countMissingSpecialCharacters(std::string& password) {
    /*      Function checks whether the string has the 3 defined types of special characters:
     *      -- uppercase char
     *      -- lowercase char
     *      -- digit
     *   If the string is missing an apparition of said special characters, it will be counted in variable missingCount
     *   Return -> missingCount
     */
    int missingCount = 0;
    bool hasDigit, hasUppercase, hasLowercase;
    // The number of possible missing characters in a string given by the problem
    int POSSIBLE_MISSING_CHARACTERS = 3;
    hasDigit = hasUppercase = hasLowercase = false;
    // Iterate the string character by character
    for (char currentCharacter: password) {
        // Check the type of the character and update by case
        if (std::isdigit(currentCharacter))
            hasDigit = true;
        if (std::isupper(currentCharacter))
            hasUppercase = true;
        if (std::islower(currentCharacter))
            hasLowercase = true;
    }
    missingCount = POSSIBLE_MISSING_CHARACTERS - (hasDigit + hasUppercase + hasLowercase);
    return missingCount;
}

void countDeleteForRepetitiveGroups(std::string& password, int& shortGroupDeletions, int& mediumGroupDeletions, int& longGroupDeletions) {
    /*      Function counts the number of optimal deletions in group of 3 or more repetitive characters
     *  Optimal deletions:
     *   - saving 1 replace at the cost of 1 delete for groups of type groupLength % 3 == 0 (group type 3n)
     *   - saving 1 replace at the cost of 2 deletes for groups of type groupLength % 3 == 1 (group type 3n + 1)
     *   - saving 1 replace at the cost of 3 deletes for groups of type groupLength % 3 == 2 (group type 3n + 2)
     *
     *   Examples:
     *  3. aaa --> aa
     *  4. ...
     *  5. ...
     *  6. aaa aaa --> aaX aa           , 1 replace 1 deletes (instead of 4 deletes)
     *  7. aaa aaaa --> aaX aa          , 1 replace 2 deletes
     *  8. aaa aaaaa --> aaX aa         , 1 replace 3 deletes
     *  9. aaa aaa aaa --> aaX aaX aa   , 2 replace 1 deletes    , now here it emerges a pattern
     *  10. aaa aaa aaaa --> aaX aaX aa , 2 replace 2 deletes
     *  11. aaa aaa aaaaa --> aaX aaX aa, 2 replace 3 deletes
     *  12. aaa aaa aaa aaa --> aaX aaX aaX aa, 3 replace 1 delete
     *
     *  Return -> returns the number of optimal deletions through output parameters:
     *      shortGroupDeletions --> the number of deletions made on groups that are of length of 3n , n representing any natural number
     *      mediumGroupDeletions --> the number of deletions made on groups that are of length of 3n + 1
     *      longGroupDeletions --> the number of deletions made on groups that are length of 3n + 2
     *
     */
    shortGroupDeletions = mediumGroupDeletions = longGroupDeletions = 0;
    int i = 0;
    // Iterate through each character of the string by index
    while (i < password.length()) {
        int groupLength = 1;
        // Count the length of each group by iterating until 2 consecutive characters are no longer the same
        while (i + groupLength < password.length() &&
               password[i + groupLength] == password[i + groupLength - 1])
            groupLength++;
        // Count the number of optimal deletions used for each type of group
        if (groupLength >= 3) {
            if (groupLength % 3 == 0)
                shortGroupDeletions++;
            if (groupLength % 3 == 1)
                mediumGroupDeletions += 2;
            if (groupLength % 3 == 2)
                longGroupDeletions += 3;
        }
        i += groupLength;
    }
}

int countReplaceForRepetitiveGroups(std::string& password) {
    /*      Function counts the number of replacement operations needed to eliminate groups of 3 or more
     *  repeating characters from the given string.
     *  Examples:
     *  aaa --> aaX (function returns 1)
     *  aaaa --> aaXa (function returns 1)
     *  aaaaa --> aaXaa (function returns 1)
     *  abcdefaaaaaaaa --> function returns 2
     *  aaaaaabbbbbbbb --> function returns 4
     */
    int totalReplaces = 0;
    int i = 0;
    // Iterate through each character of the string by index
    while (i < password.length()) {
        int groupLength = 1;
        // Count the length of each group by iterating until 2 consecutive characters are no longer the same
        while (i + groupLength < password.length() && password[i + groupLength] == password[i + groupLength - 1])
            groupLength++;
        // Update the number of total number of replaces needed for the string with the number of replaces needed for this group
        if (groupLength >= 3)
            totalReplaces += groupLength / 3;
        // Skip over the already counted group (if there is no group, we simply increment)
        i += groupLength;
    }
    return totalReplaces;
}