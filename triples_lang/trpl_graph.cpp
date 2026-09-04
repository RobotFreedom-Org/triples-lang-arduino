   
#include <Arduino.h> 

class HashTableProp {
private:
    static const int TABLE_SIZE = 6; // Fixed size for simplicity
    char* table[TABLE_SIZE];  
    int CURRENTID = -1;

    // Simple hash function
    int hashFunction(int key) const {
        return key % TABLE_SIZE;
    }

//double StringSimilarity(char  *first, char *second) { 
double StringSimilarity(const char *str1, const char *str2) {
    if (!str1 || !str2) {
        return 0.0; // Null pointer safety
    }

    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);

    // If both are empty, similarity is 100%
    if (len1 == 0 && len2 == 0) {
        return 100.0;
    }

    // Compare up to the shorter length
    size_t min_len = len1 < len2 ? len1 : len2;
    size_t matches = 0;

    for (size_t i = 0; i < min_len; i++) {
        // Case-insensitive comparison
        if (tolower((unsigned char)str1[i]) == tolower((unsigned char)str2[i])) {
            matches++;
        }
    }

    // Similarity is based on the longer string length
    size_t max_len = len1 > len2 ? len1 : len2;
    return ((double)matches / max_len) * 100.0;
}


public:

    // Insert key-value pair
    int nextID( ) {
       return CURRENTID + 1;
    }

    // Insert key-value pair
    void insert(int key,   char*  value) { 
        table[key] = value; 
    }

    // Search for a value by key
    char* search(int key) const {

       // for (int i = 0; i <= TABLE_SIZE; i++) { 
         // if (i == key){
             return  table[key] ;
          //}
        //} 
    }
 
    // Search for a value by value similarity
    int similar(char* value) const { 

        for (int i = 0; i <= TABLE_SIZE; i++) { 
              double sim = StringSimilarity(value, table[i] ); 
 
              if (sim > 80) {
                  String v =  table[i];
                  return i ;
              }
        }  
        return "";
    } 
};

class HashTableEdge {
private:
    static const int TABLE_SIZE = 6; // Fixed size for simplicity
    int table[TABLE_SIZE];  
    int CURRENTID = -1;

    // Simple hash function
    int hashFunction(int key) const {
        return key % TABLE_SIZE;
    }

public:
    // Insert key-value pair
    int nextID( ) {
       return CURRENTID + 1;
    }

    void insert(int key, int value) {
        table[key] = value; 
    }

    // Search for a value by key
    int search(int key) const {

          return table[key];
       // for (int i = 0; i <= TABLE_SIZE; i++) { 
       //   int v =  table[i];
      //    return v; 
      //  } 
    }
 
 
};
