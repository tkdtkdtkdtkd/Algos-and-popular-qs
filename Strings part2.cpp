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
