#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <random>
#include <map>
#include <algorithm>
#include <iomanip>


using namespace std;
using namespace std::chrono; //for the execution time




// Structure to hold benchmarks for each algorithm for testing and information
struct Metrics 
{
    long long operations; //used for tracking the number of operations required
    double executionTime; //used for tracking the time it takes to execute the algorithm
};


//ALGORITHMS FOR FINDING THE GCD OF 2 NUMBERS


//(1): Euiclidian Algorithm
//(2): Decrease by 1 Algorithm
//(3): Prime Factorization Algorithm via trial division


/**
 * Algorithm 1: Euclidian Algorithm
 * Efficiency Class: O(log W) where W = min(m, n)
 */
Metrics gcdEuclid(long long m, long long n) 
{
    long long divisions = 0; //start divisions at 0
    auto start = high_resolution_clock::now(); //start time for algorithm 
    
    while (n != 0) //loop until n = 0 where (when n is equal to 0 the algorithm is finished since the remainder is equal to 0)
    {
        divisions++; //one division for each iteration
        long long temp = n; 
        n = m % n;
        m = temp;
    }
    
    auto end = high_resolution_clock::now(); // stop time with algorithm is finished
    double timeTakenE = duration_cast<nanoseconds>(end - start).count(); //cast the time taken to nanoseconds
    
    return {divisions, timeTakenE}; // return the Metrics for the Euclidian algorithm
}

/**
 * Algorithm 2: Consecutive Integer Decrease-by-1 Method
 * Efficiency Class: O(W) where W = min(m, n)
 */
Metrics gcdDecreaseBy1(long long m, long long n) 
{
    long long divisions = 0; //start divisions at 0
    auto start = high_resolution_clock::now(); //begin time for algorithm
    
    if (m == 0 || n == 0) 
    {
        auto end = high_resolution_clock::now(); // end the algorithm when either m or n is equal to 0 immediately
        return {0, (double)duration_cast<nanoseconds>(end - start).count()}; //return 0 divisions and the short time taken for the algorithm to check
    }
    
    long long candidate = min(m, n); //designates the minimum value of the two numbers to the canidate value as the GCD cannot be larger than the smallest of the two numbers
    while (candidate > 0) 
    {
        divisions += 2; //increase divisions by 2 because one for each 
        if (m % candidate == 0 && n % candidate == 0) //If the canidate divides both with remainder 0 it must be the GCD since it starts at max possible value and decreases
        {
            break; //GCD found
        }
        candidate--; //GCD not found, decrease 
    }
    
    auto end = high_resolution_clock::now(); //time end for algorithm
    double timeTakenD = duration_cast<nanoseconds>(end - start).count(); //casting time to nanoseconds for the execution time
    
    return {divisions, timeTakenD}; //returns the metrics for the decrease-by-one algorithm
}

/**
 * Algorithm 3: Logarithmic Sieve Factorization via trial division
 * Efficiency Class: O(sqrt(M)) where M is the minimum of the two elements
 * https://www.geeksforgeeks.org/dsa/print-all-prime-factors-of-a-given-number/
 */
Metrics gcdPrimeFactorization(long long m, long long n) 
{
    long long divisions = 0; // start divisions at 0
    auto start = high_resolution_clock::now(); // algorithm start 
    
    if (m == 0 || n == 0) //if either is equal to 0 to start then instantly end and return since the must be 0
    {
        auto end = high_resolution_clock::now(); 
        return {0, (double)duration_cast<nanoseconds>(end - start).count()}; 
    }
    

    map<int, int> factors_m; // create a map to store the prime factors and the frequencies of each prime factor
    long long temp_m = m;  //used to edit the value of m without changing the value of the users input
    
    if (temp_m > 1) //if m is equal to 1 then then it is already factorized
    {
        
        while (temp_m % 2 == 0) //if m is evenly divisible by 2, then divide m by 2 and register 2 as a prime factor of m in the map
        {
            divisions++; //increase div count
            factors_m[2]++; //increase freq count of prime factor 2
            temp_m /= 2; //divides m by 2
        }

        
        for (long long i = 3; i * i <= temp_m; i += 2)  //checks for odd number prime factor with a hard ceiling at the square root of m
        {                                               //this is done because prime factors of m cannot be greater than the square root of m

            while (temp_m % i == 0) //if a prime factor is found
            {
                divisions++; // count successful factor division loop execution
                factors_m[i]++; //increases the freq counter of the found prime factor
                temp_m /= i; // take the prime factor out of m to continue checking
            }
        }

        
        if (temp_m > 1) // If remainder value exceeds 1, it must be prime itself
        {
            factors_m[temp_m]++; //increase th efreq counter for prime factor
        }
    }

    map<int, int> factors_n; // create a map to store the prime factors and the frequencies of each prime factor
    long long temp_n = n; //used to edit the value of n without changing the value of the users input
    
    if (temp_n > 1) //if n is equal to 1 then then it is already factorized
    {
        
        while (temp_n % 2 == 0)  //if n is evenly divisible by 2, then divide n by 2 and register 2 as a prime factor of n in the map
        {
            
            divisions++; //increase div count
            factors_n[2]++; //increase frq count
            temp_n /= 2; //divide the value of n by 2
            
        }
            // Check odd numbers up to the square root of n
            for (long long i = 3; i * i <= temp_n; i += 2) //checks for odd number prime factor with a hard ceiling at the square root of m
            {                                               //this is done because prime factors of m cannot be greater than the square root of n
                while (temp_n % i == 0) //if a prime factor is found
                {
                    divisions++; // count successful factor division loop execution
                    factors_n[i]++; //increase the freq counter of the found prime factor
                    temp_n /= i; // dynamically shrink boundary check space
                }
            }

            
            if (temp_n > 1) // If remainder value exceeds 1, it must be prime itself
            {
                factors_n[temp_n]++;//increase the freq cunter of the prime factor
            }
        
    }

    long long gcd = 1; //initialize gcd to 1
    
    for (auto it = factors_m.begin(); it != factors_m.end(); ++it) //loop through all of the prime factors and their frequencies
    {
        int prime = it->first;   //initialize variable for the prime factor
        int count = it->second;  //initialize variable for the frequency count of each prime factor

        if (factors_n.find(prime) != factors_n.end()) //if the prime factor is a shared prime factor in n
        {
            int sharedPrimeFactorCount = min(count, factors_n[prime]); //find the shared prime factor count

            
            for (int i = 0; i < sharedPrimeFactorCount; ++i) // Safe integer-based multiplication loops to completely avoid floating point truncation bugs
            {
                gcd *= prime;
            }
        }
    }
    
    auto end = high_resolution_clock::now(); //end algorithm time after all divisions are finished
    double timeTakenF = duration_cast<nanoseconds>(end - start).count(); //cast the time taken to nanoseconds 
    
    return {divisions, timeTakenF}; //return the metrics of the algorithm
}


//TESTING AND DISPLAY SUITE


int main() 
{
    random_device rd;
    mt19937 gen(rd());
    
    cout << fixed << setprecision(2);
    cout << "--- EMPIRICAL C++ GCD BENCHMARK RUN ---\n\n";
    
    vector<int> scales = {100, 1000, 10000};
    
    for (size_t s = 0; s < scales.size(); ++s) 
    {
        int base = scales[s];
        cout << "===========================================\n";
        cout << "INPUT DOMAIN: SCALE " << (s + 1) << " (~" << base << ")\n";
        cout << "===========================================\n";
        
        double avg_t_euclid = 0;
        double avg_t_dec = 0; 
        double avg_t_fact = 0;
        double avg_d_euclid = 0; 
        double avg_d_dec = 0;
        double avg_d_fact = 0;
        
        uniform_int_distribution<long long> dist_m(base, base * 2);
        uniform_int_distribution<long long> dist_n(base / 3, base - 1);
        
        for (int pair = 0; pair < 5; ++pair) 
        {
            long long m = dist_m(gen);
            long long n = dist_n(gen);
            
            Metrics E = gcdEuclid(m, n);
            Metrics D = gcdDecreaseBy1(m, n);
            Metrics F = gcdPrimeFactorization(m, n);
        
            avg_t_euclid += E.executionTime; 
            avg_d_euclid += E.operations;

            avg_t_dec += D.executionTime;    
            avg_d_dec += D.operations;
            
            avg_t_fact += F.executionTime; 
            avg_d_fact += F.operations;
        }
        
        cout << "Algorithm 1 (Euclid):\n  Avg Divisions: " << (avg_d_euclid / 5.0) << "\n  Avg Time:       " << (avg_t_euclid / 5.0) << " ns\n\n";
        cout << "Algorithm 2 (Decrease-by-1):\n  Avg Divisions: " << (avg_d_dec / 5.0) << "\n  Avg Time:       " << (avg_t_dec / 5.0) << " ns\n\n";
        cout << "Algorithm 3 (Prime Factorization):\n  Avg Divisions: " << (avg_d_fact / 5.0) << "\n  Avg Time:       " << (avg_t_fact / 5.0) << " ns\n\n";
    }
    
    return 0;
}