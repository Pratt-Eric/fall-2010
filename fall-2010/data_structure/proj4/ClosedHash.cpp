#include "ClosedHash.h"

// Constructor of ClosedHash class. 
// It is responsible for initializing each slot of the HashTable array
// to -1 and the item count in the hash table to zero. 
ClosedHash::ClosedHash():cnt(0) {
  for(int i = 0; i < NUM_SLOTS; i++){
    HashTable[i] = EMPTY;
  }
}
ClosedHash::ClosedHash(int size):cnt(0) {
  for(int i = 0; i < NUM_SLOTS; i++){
    HashTable[i] = EMPTY;
  }
}
// Insert an integer into the hash.  Return true if successful,
// false if there are no more slots in the hash, or the integer
// is already found.  Store the number of collisions in the
// variable 'collisions'.
bool ClosedHash::insert(int newValue, int &collisions){
  collisions = 0;
  if(count() >= NUM_SLOTS) {
    cerr << "Hash Table Full." << endl;
    return false; 
  }
  int hidx = h(newValue); 	// home index; 
  int pidx = h2(newValue); 	// probe index; 

  for(int i = 0; ; i++) {
    int nidx = (hidx + i * pidx) % NUM_SLOTS; 
    if(HashTable[nidx] == EMPTY || HashTable[nidx] == TOMBSTONE) {
      HashTable[nidx] = newValue;
      cnt++;
      break; 
    } else {
      collisions++;
    }
  }
  return true; 
}

// Find an integer value in the hash.  Return true if found,
// false if not.  Store the number of probes in the variable
// 'probes'.
bool ClosedHash::find(int searchValue, int &probes) const{
  probes = 0;
  int hidx = h(searchValue); 	// home index; 
  int pidx = h2(searchValue); 	// probe index; 

  for(int i = 0; ; i++) {
    int nidx = (hidx + i * pidx) % NUM_SLOTS; 
    if(HashTable[nidx] == searchValue) {
      return true;
    } else {
      if(HashTable[nidx] == EMPTY) return false; 
      probes++;
    }
  }
  return false;
}

// Delete an integer from the hash.  Return true if successful,
// false if unsuccessful.
bool ClosedHash::remove(int delValue){
  if(isEmpty()) return false;
  int hidx = h(delValue); 	// home index; 
  int pidx = h2(delValue); 	// probe index; 

  for(int i = 0; ; i++) {
    int nidx = (hidx + i * pidx) % NUM_SLOTS; 
    if(HashTable[nidx] == delValue) {
      HashTable[nidx] = TOMBSTONE;
      cnt--;
      return true;
    }
  }
  return false;
}

////////////////////////////////////////////////////
// Hash functions. 
////////////////////////////////////////////////////
unsigned int ClosedHash::h(int key) const {
  unsigned int hash = 0;      // The hash value starts at 0
  unsigned int keyarr = key;  // A copy of the key

  // We will treat the key (an integer) as an array of 4
  // unsigned characters
  unsigned char *keyptr = (unsigned char *) &keyarr;
     
  // Mix each 8-bit character into the hash
  for (int i = 0; i < (sizeof(int)); i++) {

    // This is the combining step:
    hash += keyptr[i];

    // This is the mixing step:
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }

  // After all the bits of the key have been mixed in,
  // ensure that they are properly distributed throughout
  // the final hash value:
  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  // This last line assumes the you have a data member 
  // or constant in class ClosedHash called maxSize.  This
  // is the value M (the number of buckets in the hash).  This
  // can be a private data element or constant.
  int maxSize = NUM_SLOTS;
  return hash % maxSize;
}

//  ****
//  For comments, see h(), above!
//  Also see the comments near the end of h2().
//  ****
unsigned int ClosedHash::h2(int key) const {
  unsigned int hash = 0;
  unsigned int keyarr = key;
  unsigned char *keyptr = (unsigned char *) &keyarr;
     
  for (int i = 0; i < sizeof(int); i++) {
    hash += keyptr[i];
    hash += (hash << 9);
    hash ^= (hash >> 5);
  }
  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  // The following code ensures that the value of h2(k) will be
  // odd, which should be coprime with the size of the closed
  // hash (32,768 == 2^15).
  int maxSize = NUM_SLOTS;
  return (((hash * 2) + 1) % maxSize);
}