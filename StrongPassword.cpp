#include <iostream>
#include <cmath>

int getMissingSpecialCharacters(std::string& password);

void getNumberOfDeletesForRepetitions(std::string& password, int& numberOfShortDeletions, int& numberOfMediumDeletions,
                                      int& numberOfLargeDeletions);

int numberOfReplacesForRepetitions(std::string& password);


int minimumEditsStrongPassword(std::string& password) {
    const int LOWER_PASSWORD_BOUND = 6;
    const int UPPER_PASSWORD_BOUND = 20;
    // Case Len < 6
    if (password.length() < LOWER_PASSWORD_BOUND) {
        int numberOfRequiredSpecialCharacters = getMissingSpecialCharacters(password);
        return std::max(LOWER_PASSWORD_BOUND - (int) password.length(), numberOfRequiredSpecialCharacters);
    }
        // Case 6 <= Len <= 20
    else if (password.length() <= UPPER_PASSWORD_BOUND) {
        int numberOfRequiredSpecialCharacters = getMissingSpecialCharacters(password);
        int numberOfRequiredReplaces = numberOfReplacesForRepetitions(password);
        return std::max(numberOfRequiredSpecialCharacters, numberOfRequiredReplaces);
    }
        // Case Len > 20
    else {

        // This is the number of deletions we HAVE TO make
        int numberOfRequiredDeletions = (int) password.length() - UPPER_PASSWORD_BOUND;
        // This is the number of deletions for groups of len % 3 == 0
        int numberOfDeletionsSmall;
        // This is the number of deletions for groups of len % 3 == 1
        int numberOfDeletionsMedium;
        // This is the number of deletions for groups of len % 3 == 2
        int numberOfDeletionsLarge;
        getNumberOfDeletesForRepetitions(password, numberOfDeletionsSmall, numberOfDeletionsMedium,
                                         numberOfDeletionsLarge);
        // We also need the total number of replacements needed to eliminate repetitive groups
        int replacements = numberOfReplacesForRepetitions(password);
        // Now we are trying to "save" as many replacements as we can (maximize the effectiveness of delete)
        // Try to use the most effective deletes
        replacements -= std::min(numberOfRequiredDeletions, numberOfDeletionsSmall);
        // Update the number of deletions left to do to reach <= 20 chars
        // If we still need to make deletions it will take the second most optimized deletions
        // we have to divide by 2 because in this case 2 deletions is equivalent to 1 replacement
        replacements -=
                std::min(std::max(numberOfRequiredDeletions - numberOfDeletionsSmall, 0), numberOfDeletionsMedium) / 2;
        // Update the number of deletions left after the second case
        // We reached the last possible and worst optimization for delete instead of replace
        replacements -= std::max(numberOfRequiredDeletions - numberOfDeletionsSmall - numberOfDeletionsMedium, 0) / 3;
        // Now we know if we have to still delete (although no optimization are possible now) elements to reach
        // the character cap of 20. If we still have unoptimal deletes left, we simply do them, then we reach
        // the character cap of 20, once we do that we are simply in case 2 with the replacements we have left
        // (replacements left from the delete optimization)
        // simply the answer is the number of steps left to reach 20 characters (if the string is still over 20)
        // and the solution from case 2 with the new number of replacements
        int numberOfMissingCharacters = getMissingSpecialCharacters(password);
        return numberOfRequiredDeletions + std::max(numberOfMissingCharacters, replacements);
    }
}


int main() {
    std::string originalPassword;
    std::cout << "\nInput password" << "\n > ";
    std::cin >> originalPassword;
    std::cout << "The minimum number of edits to make it a strong password is: \n"
              << minimumEditsStrongPassword(originalPassword);
    return 0;
}

int getMissingSpecialCharacters(std::string& password) {
    bool hasDigit, hasUppercase, hasLowercase;
    int POSSIBLE_MISSING_CHARACTERS = 3;
    hasDigit = hasUppercase = hasLowercase = false;
    for (char currentCharacter: password) {
        if (std::isdigit(currentCharacter))
            hasDigit = true;
        if (std::isupper(currentCharacter))
            hasUppercase = true;
        if (std::islower(currentCharacter))
            hasLowercase = true;
    }
    return POSSIBLE_MISSING_CHARACTERS - (hasDigit + hasUppercase + hasLowercase);
}

void getNumberOfDeletesForRepetitions(std::string& password, int& numberOfShortDeletions, int& numberOfMediumDeletions,
                                      int& numberOfLargeDeletions) {
    numberOfShortDeletions = numberOfMediumDeletions = numberOfLargeDeletions = 0;
    int i = 0;
    while (i < password.length()) {
        int groupLength = 1;
        while (i + groupLength < password.length() &&
               password[i + groupLength] == password[i + groupLength - 1])
            groupLength++;

        if (groupLength >= 3) {
            if (groupLength % 3 == 0)
                numberOfShortDeletions++;
            if (groupLength % 3 == 1)
                numberOfMediumDeletions += 2;
            if (groupLength % 3 == 2)
                numberOfLargeDeletions += 3;
        }
        i += groupLength;
    }
}

int numberOfReplacesForRepetitions(std::string& password) {
    int totalReplaces = 0;
    int i = 0;
    while (i < password.length()) {
        int groupLength = 1;
        while (i + groupLength < password.length() &&
               password[i + groupLength] == password[i + groupLength - 1])
            groupLength++;

        if (groupLength >= 3)
            totalReplaces += groupLength / 3;
        i += groupLength;
    }
    return totalReplaces;
}