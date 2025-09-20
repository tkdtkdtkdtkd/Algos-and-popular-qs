1. Minimum number of bracket reversals needed to make an expression balanced
/*
odd number of brackets cannot be balanced anyways
use open and close variables instead of a stack, less space complexity
*/
int minReversals(string s) {
    int n = s.length();
    if (n % 2 != 0) return -1; // odd length can't be balanced

    int open = 0, close = 0;
    for (char c : s) {
        if (c == '{') open++;
        else {
            if (open > 0) open--; 
            else close++;
        }
    }
    return (open + 1) / 2 + (close + 1) / 2;
}

2. Count and say
/*
simple enough n iterations, find number of chars repeated, so 999 becomes 39 and so on
use of to_string function
*/
string countAndSay(int n) {
    string s = "1";
    if (n == 1)
        return s;
    for (int i = 2; i <= n; i++) {
        string next = "";
        for (int j = 0; j < s.size(); j++) {
            int cnt = 1;
            while (j + 1 < s.size() && s[j] == s[j + 1]) {
                j++;
                cnt++;
            }
            next += to_string(cnt);
            next += s[j];
        }
        s = next;
    }
    return s;
}

3. Hashing in strings
/*
hash(s)=s[0]*p^0+s[1]*p^1+....+s[n-1]*p^(n-1) mod M
p = a prime base (commonly 31 for lowercase, 53 for full alphabet)
M = large prime modulus (1e9+7, 1e9+9)
*/
const ll MOD = 1e9 + 7,
    p = 31;
vector < ll > power, prefix;
void stringHasher(string & s) {
    power.assign(n + 1, 1);
    prefix.assign(n + 1, 0);
    for (int i = 1; i <= n; i++) power[i] = (p * power[i - 1]) % MOD;
    for (int i = 0; i < n; i++) prefix[i + 1] = ((s[i] - 'a') * power[i] + prefix[i]) % MOD;
}

ll getHash(int l, int r) {
    ll h = (prefix[r + 1] - prefix[l] + MOD) % MOD;
    return (h * power[n - l - 1]) % MOD; // normalized to P^(n-1)
}

4. Rabin Karp
Given a text and a pattern, find all positions in the text where the pattern occurs.

vector < int > rabinKarp(string & text, string & pattern) {
    //reverse hashing is simpler -> H(s) = s[0]⋅ Pk− 1 + s[1]⋅ Pk− 2 + ⋯ +s[k− 1]⋅ P0
    int n = text.size(), m = pattern.size();
    vector < int > res;
    if (m > n) return res;
    const ll MOD = 1e9 + 7,
        p = 31;
    vector < ll > power(n + 1, 1), prefix(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        power[i] = (power[i - 1] * p) % MOD;
    }
    for (int i = 0; i < n; i++) {
        prefix[i + 1] = (prefix[i] * p + (text[i] - 'a' + 1)) % MOD;
        //reverse hashing, don't use power hashing
        //H(s) = s[0]⋅ Pk− 1 + s[1]⋅ Pk− 2 + ⋯ +s[k− 1]⋅ P0
    }
    ll pattern_hashed = 0;
    for (int i = 0; i < m; i++) {
        pattern_hashed = (pattern_hashed * p + (pattern[i] - 'a' + 1)) % MOD;
    }

    auto getHash = [ & ](int l, int r) {
        int len = r - l + 1;
        ll h = (prefix[r + 1] - (prefix[l] * power[len]) % MOD + MOD) % MOD;
        //extremely important to take % MOD at every step, code doesn 't work else
        //* power[len] to cancel unnecessary powers and get absolute hash value
        return h;
    };

    for (int i = 0; i + m - 1 < n; i++) {
        if (getHash(i, i + m - 1) == pattern_hashed) {
            res.push_back(i);
        }
    }
    return res;
}

5. Z-Function // z[i] denotes the number of characters starting from ith index that match with characters starting from 0th index

int strStr(string haystack, string needle) {
    string s = needle + "$" + haystack;
    int n = s.size(), m = needle.size();
    if(m==0) return 0;
    int l = 0, r = 0;
    vector<int> z(n, 0);
    for (int i = 1; i < n; i++) {
        if (i <= r) {
            //if i is within range l and r, then reuse precomputed stuff, we know that substring from l to r is same as 0 to r-l, so i should be same as i-l, also it cannot exceed r so min(z[i-l],r-i+1)
            z[i] = min(z[i - l], r - i + 1);
        }
        while (i + z[i] < n && s[z[i]] == s[i + z[i]])
            z[i]++; //keep going till it matches
        if (i + z[i] - 1 > r) {
            r = i + z[i] - 1;
            l = i; //update l and r accordingly
        }
        if (z[i] == m)
            return i-(m+1); //$ contributes 1 more index so len of needle+"$"
    }
    return -1;
}

6. KMP / LPS(pi) algo

//lps[i] stores length of prefix that is = suffix in range 0 to i
vector < int > makeLPS(string s) {
    int m = s.size();
    vector < int > lps(m, 0);
    int len = 0, i = 1;
    while (i < m) {
        if (s[i] == s[len]) {
            len++;
            lps[i] = len;
            i++;
        }
        else {
            if (len != 0) {
                //try next smallest prefix and see if that matches, don't change i yet
                len = lps[len - 1];
            }
            else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

int strStr(string haystack, string needle) {
    int i = 0, j = 0;
    int n = haystack.size(), m = needle.size();
    vector < int > lps = makeLPS(needle);
    while (i < n) {
        if (haystack[i] == needle[j]) {
            i++;
            j++;
        }
        if (j == m) return i - m;
        else if (i < n && haystack[i] != needle[j]) {
            //move to longest smaller prefix that might still match
            if (j != 0) j = lps[j - 1];
            else i++;
        }
    }
    return -1;
}

7. Add characters to the front of string s and make it a palindrome, return the palindrome.

//master = "aacecaaa$aaacecaa" string+"$"+reversed_string

vector < int > makeLPS(string s) {
    int n = s.size();
    vector < int > lps(n, 0);
    int len = 0, i = 1;
    while (i < n) {
        if (s[i] == s[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0)
                len = lps[len - 1];
            else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

string shortestPalindrome(string s) {
    int n = s.size();
    if (n < 2)
        return s;
    string rev = s;
    reverse(rev.begin(), rev.end());
    string master = s + "$" + rev;
    vector < int > lps = makeLPS(master);
    int maxi = lps.back(); //find prefix=suffix for lps[n-1]! not the just the longest
    string toAdd = s.substr(maxi); //s.substr(start)->default till the end, or you can do s.substr(start,len)
    reverse(toAdd.begin(), toAdd.end());
    return toAdd + s;
}

Can also be done using Z function.

vector < int > makeZ(string s) {
    int n = s.size();
    vector < int > z(n, 0);
    int l = 0, r = 0;
    for (int i = 1; i < n; i++) {
        if (i <= r) {
            z[i] = min(z[i - l], r - i + 1);
        }
        while (i + z[i] < n && s[i + z[i]] == s[z[i]])
            z[i]++;
        if (i + z[i] > r) {
            r = i + z[i];
            l = i;
        }
    }
    return z;
}

string shortestPalindrome(string s) {
    int n = s.size();
    string rev = s;
    reverse(rev.begin(), rev.end());
    string master = s + "$" + rev;
    vector < int > z = makeZ(master);
    int longest = 0;
    for (int i = n + 1; i < 2 * n + 1; i++) {
        if (i + z[i] == 2 * n + 1) { //again see that the suffix is till the end
            longest = z[i];
            break;
        }
    }
    string to_add = s.substr(longest);
    reverse(to_add.begin(), to_add.end());
    return to_add + s;
}

8. Longest happy prefix-> direct implementation of Z-function / KMP algo

