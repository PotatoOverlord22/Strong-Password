# Explanation
## Info

    A password is considered strong if below conditions are all met:
        1. It has at least 6 characters and at most 20 characters.
        2. It must contain at least one lowercase letter, at least one uppercase letter, and at least one
            digit.
        3. It must NOT contain three repeating characters in a row ("...aaa..." is weak, but "...aa...a..."
            is strong, assuming other conditions are met).

## Task

        Write an algorithm that takes a string s as input, and return the MINIMUM change required to
    make s a strong password. If s is already strong, return 0.
    Insertion, deletion or replace of any one character are all considered as one change.

## Idea

        Intuition says that if you want to calculate the MINIMUM changes to turn the string into a strong password, 
    then we need to OPTIMIZE each change, meaning each change should aim to fix as many problems as possible.

## First, How To Fix Each Requirement?
        Before we can start optimizing, we have to analyze how and with which operations we can fix each
    requirement. By requirement I'm denoting the above characteristics of a strong password (requirement 1
    is  6 <= lenght <= 20, requirement 2 is special characters, and requirement 3 is the one with repetitive
    groups). Let's talk about each requirement and its appropriate operations for fixing it:

    1. The password lenght must be between 6 and 20 characters long:
        This requirement can be broken into 2 cases:
            1.a. When the string is < 6 char long
            2.a. When the string is > 20 char long
        For case 1.a. the single operation that can fix it is adding/inserting characters
        For case 2.a. the single operation that can fix it is removing characters
    So, requirement 1 can be solved (depending on the case) with insertion and removal of characters.
    
    2. The password must contain special characters:
        Unlike the first requirement, to solve requirement 2 we have to analyze the characters that are in the
        string. Once we took a look at the password and identified how many special characters we are missing (a 
        number between 0 and 3) we will now have to either add or replace already existing non-special characters
        with the number of missing special characters.
    So, requirement 2 can be solved with add or replace.
    
    3. The password must not have 3 or more repeating characters in a row:
        For the sake of clarity, I will denote a 'group of repeating characters' a sequence of the same character
        in the given password of length >= 3, and a 'subgroup of repeating characters' a sequence of 3, 4, or 
        maximum 5 characters in a group of repeating characters (we will see why I chose this notations later).
        Now, this requirement can be solved using any of the possible operations, BUT with different effects,
        meaning that some operations are better than others at dealing with groups of repeating characters, but
        we will talk about their effects in more detail when talking about optimizing all of this.
    So, requirement 3 can be solved with add, replace or delete.

## How to optimize each change?
        Now here is where the real challenge begins. Firstly, optimizing each change made upon the password means,
    as mentioned above, that we are aiming to work towards solving as many requirements as possible with one change.
    As you might've already noticed, the use of operations overlaps between requirements and their case, so one
    deletion of a character could fix both the length issue and the grouping issue, one insertion of a character
    could fix all 3 requirements, and so on. Unfortunately the usefulness of each operation is highly dependend on the
    state of the password, take for example:
        1. password: aaaaa --> here the best steps are to add 2 new characters, ultimately splitting the group of
                            repeating characters, inserting the required special characters and fixing the length issue
                  aaaaa --> aa1aXaa
        2. password: aaa aaaa --> here 2 replacements fix the password, whereas using insertions you would need 3
                                insertions to fix it
                    aaa aaaa --> aaX aa1a
    Now, the whole problem revolves around splitting the string into cases and using the best operations for that
    specific case. For each case we will denote an operation more 'powerful' than another operation when the first
    operation solves more requirements that the second operation.

## The Power of each operation
        In this section, we will put each operation and requirement face to face and compare each operation's
    effectiveness at solving that requirement. We are creating a priority of operations when aiming to solve a
    requirement:
    
    Requirement 1: -> when password length < 6 --> only INSERTION can solve it
                    -> when password length > 20 --> only DELETION can solve it
    
    Requirement 2:  -> BEST: Insertion / Replacement (same power)
                    -> WORST: Deletion (doesn't even solve requirement 2)
    
    Requirement 3:  -> BEST: Replacement
                    -> AVERAGE: Insertion
                    -> WORST: Deletion

## Breaking the problem into cases

        Dividing the problem by the length of the password has the benefits of streamlining the optimal
    order of operations, we will see more once we break down each case.

    Case 1: Password length < 6
    Case 2: 6 <= Password length <= 20
    Case 3: Password length > 20

    Let's break down in detail each case.

## Case 1: Password Length < 6
        This case is fairly simple, since the password needs extra padding of characters, we are forced
    into using INSERTION as the priority operation, this is fortunate for us since insertion can solve the
    other two requirements quite optimally.
    Subtasks:
        1. We need to add special characters
        2. We need to separate a group of repeating characters
    Fortunately for us, separating a group of repeating characters is easy, because if we have a group of
    repeating characters in this case it 100% guarantees that we need to add at least 1 special character, and we
    can simply insert those special character(s) to separate the group of repeating characters. Examples:
    1.    aaa aa --> aaXa1aa
    2.    aaa 1X --> aXaa1X, in this case we needed to insert one character for the length condition
                            so we separated the group as well even though we already had special characters
    3.    aaa a1 --> aaXaa1, again we inserted one special character and separated the group of repeating characters

    So, the solution for this case is simply the maximum amount of changes we would need to make between inserting
    special characters and the number of insertions necessary to reach the lower bound, or in other words:

    result = max(6 - lengthOfPassword, numberOfMissingSpecialCharacters)

    Here, by inserting a special character, or any character for that matter, in the right spot, 
    requirement 3 becomes trivial.

## Case 2: 6 <= Password Length <= 20
        In this case, requirement 1 is already solved for us, which means that INSERTION and DELETION don't make much
    sense, because they modify the length of the password, possibly undoing the first requirement. Another reason to
    use only REPLACEMENT here is because, if we look above in the power of operations 'chart', we can see that
    replacement is already the best out of the three operations for solving requirement 2 and requirement 3 
    at the same time. Now that we know for sure which operation we will use, we need to calculate the number of
    operations to reach a strong password. Using replace effectively in this case boils down to counting how many
    special characters to add we have left and counting how many replacements we need to do separate the group of 
    repeating characters. Since we can use 1 replacement to progress both requirements, the minimum number of 
    operations to reach a strong password is the maximum between the count of missing special characters and the 
    count of replacements needed. 
        How to count the minimum replacements needed for splitting a group of repeating characters?
    We can reach the conclusion to this question by taking a couple of examples and noticing a pattern:
    Denote 'X' as any character that is a viable replacement for previous character on X's position
    1.      a -->                       , no replacement
    2.      aa -->                      , no replacement
    3.      aaa ---> aaX                , 1 replacement
    4.      aaaa ---> aaXa              , 1 replacement
    5.      aaaaa ---> aaXaa            , 1 replacement
    6.      aaa aaa ---> aaX aaX        , 2 replacements (example 3 repeated 2 times)
    7.      aaa aaaa ---> aaX aaXa      , 2 replacements (example 3 + example 4)
    8.      aaa aaaaa ---> aaX aaXaa    , 2 replacements (example 3 + example 5)
    9.      aaa aaa aaa ---> aaX aaX aaX, 3 replacements (example 3 repeated 3 times)
    .................................................................................
    n.      aaa aaa aaa ..... aaa      , if n % 3 == 0       , n / 3  replacements (example 3 repeated n times)
    n + 1.  aaa aaa aaa ..... aaaa     , if (n + 1) % 3 == 1 , (n + 1) / 3 replacements (example 3 repeated n times + example 4)
    n + 2.  aaa aaa aaa ..... aaaaa    , if (n + 2) % 3 == 2 , (n + 2) / 3 replacements (exampl 3 repeated n times + example 5)
    
    As we can see the number of replacements needed to fix a group is given by: 

    replacements = groupLenght / 3

    Reaching our final solution for this case:

    result = max(numberOfReplacements, numberOfMissingSpecialCharacters)

## Case 3: Password Length > 20
        In this case, we are forced from the start to use DELETE until we fix the length issue.
    Unfortunately, delete is the WEAKEST operation out of all 3 of them, poorly solving requirement 3 and not even 
    solving requirement 2. Since delete is the weakest, we will use it only in the beggining to fix the length of the
    password, then once our password is of length <= 20, we arrive in Case 2, where we know how to compute the solution.
        However, simply deleting characters with no strategy until we reach length 20 will not grant us the best solution
    for this case. We will have to use our deletes smartly, and since we cannot use delete to solve requirement 2, we
    will use our required deletes to solve requirement 3.
        A trap that you could fall in is to simply try to delete characters from groups of repeating characters and
    and think that is the optimal way of using delete to solve requirement 3. Let's see the process of finding the 
    best strategy for solving requirement 3 using deletes. Take the above groups of repeating characters format:
    1. a -->                    , no delete
    2. aa -->                   , no delete
    3. aaa --> aa               , 1 delete
    4. aaaa --> aa              , 2 deletes
    5. aaaaa --> aa             , 3 deletes
    ............................................
    n. aaa aaa aaa ..... aaa --> aa  , n - 2 deletes
    Whoah, the number of deletes required to solve long groups of repeating characters grows really fast. This is
    the trap that I was talking about above. Then what is the best use of deletes? The key here is to use them in
    tandem with replacements to reach the best way of getting rid of group of repeating characters. We can notice
    from the above examples number 3, 4, 5 that 1, 2, 3 deletes respectively have the same effect as one replace.
    This is true on big groups too when combining deletes with replacements, meaning:
    1. ...
    2. ...
    3. aaa --> aa
    4. ...
    5. ... 
    6. aaa aaa --> aaX aa           , 1 replace 1 deletes (instead of 4 deletes)
    7. aaa aaaa --> aaX aa          , 1 replace 2 deletes
    8. aaa aaaaa --> aaX aa         , 1 replace 3 deletes
    9. aaa aaa aaa --> aaX aaX aa   , 2 replace 1 deletes    , now here it emerges a pattern
    10. aaa aaa aaaa --> aaX aaX aa , 2 replace 2 deletes
    11. aaa aaa aaaaa --> aaX aaX aa, 2 replace 3 deletes
    12. aaa aaa aaa aaa --> aaX aaX aaX aa, 3 replace 1 delete
    
        We can notice a pattern above, but what does this pattern exactly mean semantically? It means that we are
    essentially:
        - saving 1 replace at the cost of 1 delete for groups of type groupLength % 3 == 0 (group type 3n)
        - saving 1 replace at the cost of 2 deletes for groups of type groupLength % 3 == 1 (group type 3n + 1)
        - saving 1 replace at the cost of 3 deletes for groups of type groupLength % 3 == 2 (group type 3n + 2)

        Armed with this knowledge, now we can optimally use deletes for progressing towards requirement 3 by 
    using the deletes we are required to do in the order of their effectivenes:
        - first use deletes on groups of type 3n, saving (deletesUsed / 1) replacements
        - if there are required deletes left, use deletes on groups of type 3n + 1 (saving deletesUsed / 2) replacemenets
        - if there are required deletes left, use deletes on groups of type 3n + 2 (saving deletesUsed / 3) replacemenets
        - if there are required deletes left, use them on any group (there no longer exist optimizations)
        Once we deleted all the extra characters in a smart way, we now reach Case 2 with our problem, but with the exception
    of using the updated number of replacements(subtract the replacements saved by deletes).

    So, the solution for the final case looks like:

    result = requiredDeletes + max(totalReplacements - savedReplacements, missingRequiredSpecialCharacters)
    
