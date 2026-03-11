#include <iostream>
#include <random>
#include <cstdint>
#include <vector>
#include <algorithm>
using namespace std;


// ---------------------------
// Global randomness
// ---------------------------
static const std::uint64_t RNG_SEED = 123456789ULL;
static std::mt19937_64 RNG(RNG_SEED);

// Sample from {-1, 0, 1}
int sampleSmallCoeff() {
    static std::uniform_int_distribution<int> dist(0, 2);
    return dist(RNG) - 1;
}

// Reduce into [0, q-1]
int64_t modq(int64_t x, int64_t q) {
    int64_t r = x % q;
    return (r < 0 ? r + q : r);
}

// SAP instance container
struct SAPInstance {
    std::vector<std::vector<int64_t>> A; // n x m matrix mod q
    std::vector<int64_t> z;              // short solution
    int64_t q;
};

// Generate SAP/SIS-style instance with known short solution
SAPInstance generateSAPInstance(int n, int m, int64_t q) {
    if (m <= n) {
        throw std::invalid_argument("Need m > n for SIS/SAP.");
    }

    std::uniform_int_distribution<int64_t> dist_mod(0, q - 1);

    // Step 1: random B (n x (m-1))
    std::vector<std::vector<int64_t>> B(n, std::vector<int64_t>(m - 1));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m - 1; j++)
            B[i][j] = dist_mod(RNG);

    // Step 2: short vector v in Z^{m-1}
    std::vector<int64_t> v(m - 1);
    for (int j = 0; j < m - 1; j++)
        v[j] = sampleSmallCoeff();

    // Step 3: last column a_m = -B * v mod q
    std::vector<int64_t> a_m(n, 0);
    for (int i = 0; i < n; i++) {
        int64_t sum = 0;
        for (int j = 0; j < m - 1; j++)
            sum += B[i][j] * v[j];
        a_m[i] = modq(-sum, q);
    }

    // Step 4: assemble A = [B | a_m]
    std::vector<std::vector<int64_t>> A(n, std::vector<int64_t>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m - 1; j++)
            A[i][j] = B[i][j];
        A[i][m - 1] = a_m[i];
    }

    // Step 5: short solution z = (v, 1)
    std::vector<int64_t> z(m);
    for (int j = 0; j < m - 1; j++)
        z[j] = v[j];
    z[m - 1] = 1;

    return {A, z, q};
}



//This code will allow us to generate a random prime number from 0 to 2q as most efficiently as possible

// ----- 64-bit modular multiplication -----
uint64_t mulmod(uint64_t a, uint64_t b, uint64_t mod) {
    __uint128_t r = ( __uint128_t )a * b;
    return (uint64_t)(r % mod);
}

// ----- modular exponentiation -----
uint64_t modpow(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    while (exp > 0) {
        if (exp & 1) result = mulmod(result, base, mod);
        base = mulmod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

// ----- Miller–Rabin primality test (deterministic for 64-bit) -----
bool isPrime(uint64_t n) {
    if (n < 2) return false;
    for (uint64_t p : {2,3,5,7,11,13,17,19,23,29,31,37})
        if (n % p == 0) return n == p;

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    // Deterministic bases for 64-bit integers
    for (uint64_t a : {2ULL, 325ULL, 9375ULL, 28178ULL,
                       450775ULL, 9780504ULL, 1795265022ULL}) {
        if (a % n == 0) continue;
        uint64_t x = modpow(a, d, n);
        if (x == 1 || x == n - 1) continue;

        bool composite = true;
        for (uint64_t r = 1; r < s; r++) {
            x = mulmod(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

// ----- Generate random prime ≤ N -----
uint64_t randomPrime(uint64_t N) {
    if (N < 2) throw std::invalid_argument("No primes <= N");

    static random_device rd;
    static mt19937_64 gen(rd());
    uniform_int_distribution<uint64_t> dist(2, N);

    while (true) {
        uint64_t x = dist(gen);
        if (isPrime(x)) return x;
    }
}

// End of functions to help generate large random prime number for a




//Define big q, which is a very large prime number
//Question: How big does it need to be?
//A sample q is provided here
const unsigned long long q = 1795265022;

//Define the error range the vector can hold. Must be very small
//Question: What is the maximum smallest size we can get?
//Assumption: For now, 1/q is used as a placeholder
const float error_range = (1/(double)q);

//Define the dimension we are working in
//Rows
int n = 359;
//Columns
int m = 2934;
const unsigned int N = (m-n);

//Create a, which a random prime number from 1 to 2q-1
//This is our public key
const unsigned long long a = randomPrime(2*q);


int main(int argc, const char * argv[]) {
    cout << "Cryptosystem information:" << endl << endl;
    cout << "Big prime, q, is equal to: " << q << endl;
    cout << "The error interval is from -" << error_range << " to +" << error_range << endl;
    cout << "The dimension, N, is: " << N << endl;
    cout << "The public key, a, is: " << a << endl << endl;

    
   
    //Generate a short vector given m, n and q
    cout << "SAP short vector generation: " << endl << endl;

    SAPInstance inst = generateSAPInstance(n, m, 2*q);


    std::cout << "Matrix A (mod " << inst.q << "):\n";
    for (auto &row : inst.A) {
        for (auto x : row) std::cout << x << " ";
        std::cout << "\n";
    }

    std::cout << "\nShort solution z:\n";
    for (auto x : inst.z) std::cout << x << " ";
    std::cout << "\n";

    std::cout << "\nCheck A*z mod 2q:\n";
    for (int i = 0; i < n; i++) {
        int64_t sum = 0;
        for (int j = 0; j < m; j++)
            sum += inst.A[i][j] * inst.z[j];
        std::cout << modq(sum, inst.q) << " ";
    }
    std::cout << "\n";
   
    
    
    
    return 0;
}
