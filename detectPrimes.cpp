
#include "detectPrimes.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <iostream>
#include <semaphore.h>
#include <cassert>


sem_t sem; =
int vectorCounter; //mutex protected thread counter
std::vector<int64_t> vecNums;
std::vector<int64_t> res; 
pthread_mutex_t mutex1;
pthread_mutex_t mutex2; 

struct threadParam{ /==
  int n;
};

void *is_prime(void *a)
{
  std::vector<int64_t> localRes; //local prime results vector

  //DEBUG 
  //std::cout << vectorCounter; 
  while(vectorCounter > -1){
    //bool prime = false; //immediately set the number as not being prime 
    
    pthread_mutex_lock( &mutex1);  //using a QUEUE 
      int64_t current = vecNums[vectorCounter];
      //DEBUG current thread and the counter 
      //std::cout << "| " << vectorCounter << "-" <<"current: " << current << " |"; 
      vectorCounter--; 
    pthread_mutex_unlock( &mutex1);


    bool prime = false; 
    bool innerPrime = false;

    while (prime != true){
      if (current < 2) break;
      if (current <= 3) prime = true; // 2 and 3 are primes
      if (current % 2 == 0) break; 
      if (current % 3 == 0) break; 
      // try to divide n by every number 5 .. sqrt(n)
      int64_t i = 5;
      int64_t max = sqrt(current);
      while (i <= max) {
        if (current % i == 0){
          innerPrime = true; //break and set flag
          i = max + 1; 
        } 
        if (current % (i + 2) == 0) {
          innerPrime = true; 
          i = max + 1; 
        } 
        i += 6;
      }   
      if (innerPrime == true){ //i am so sorry for this loop hell 
         prime = false;
         break;
         }
      else{
        prime = true;
      }
    }

    if (prime == true){
      localRes.push_back(current);
      //DEBUG check push
      //std::cout << "pushed as prime: " << localRes.back();
    }

  }
  //printf("--------------------------\n");
  //DEBUG checking vector
/*     std::cout << "print localRes"; 
    for (auto x : localRes){
      std::cout << x << "-"; 
    } */

  //concatenating results 
  pthread_mutex_lock( &mutex2);
    res.insert(res.end(), localRes.begin(), localRes.end());
  pthread_mutex_unlock( &mutex2);

  pthread_exit(0);
}

// This function takes a list of numbers in nums[] and returns only numbers that
// are primes.
//
// The parameter n_threads indicates how many threads should be created to speed
// up the computation.
// -----------------------------------------------------------------------------
std::vector<int64_t> detect_primes(const std::vector<int64_t> &nums, int n_threads)
{
  std::vector<int64_t> result; 
  if (n_threads == 0) n_threads = 1;  //even if 0, must still run with one thread 

  vectorCounter = nums.size()-1;  
  vecNums = nums; 

  sem_init( &sem, 0, 1); //binary

    pthread_t threads[n_threads];

    for (int i=0; i< n_threads ; i++){
      pthread_create ( &threads[i], 0, is_prime, NULL);
    } 
  
    for (int i=0; i< n_threads ; i++){
      pthread_join (threads[i], NULL);
    } 

  return res;
}
