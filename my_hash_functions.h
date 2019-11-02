//
// Created by andrew on 30.10.2019.
//

#ifndef HASHTABLE_MY_HASH_FUNCTIONS_H
#define HASHTABLE_MY_HASH_FUNCTIONS_H
char Simple_hash(const void* key, int len){
    return 1;
}

char First_letter(const void * key, int len){
    return (*(char*)key);
}

int String_len (const void* key, int len){
    return len;
}

unsigned int Char_Sum (const void* key, int len){
    unsigned int sum = 0;
    for(int i = 0; i < len; i++ ) {
        sum += *(char*)(key + i);
    }
    return sum;
}

inline unsigned  int roll (unsigned int whattoroll, int times)
{
    char first_bit = whattoroll >> sizeof(int)*8 - 1 ;
    for (int i = 0; i < times; i++){
        whattoroll = whattoroll << 1 + first_bit;
        first_bit = whattoroll >> sizeof(int)*8 - 1 ;
    }
}

unsigned int Recur (const char* key, int len){
    if (len == 0){
        return 0;
    }
    return roll(Recur(key + 1, len - 1), 1) ^ (*key);
}


unsigned int MurMur_Hash(const void * key, int len, unsigned int seed = 0)
{
    // 'm' and 'r' are mixing constants generated offline.
    // They're not really 'magic', they just happen to work well.

    const unsigned int m = 0xc6a4a793;
    const int r = 16;

    // Initialize the hash to a 'random' value

    unsigned int h = seed ^ (len * m);

    // Mix 4 bytes at a time into the hash

    const unsigned char * data = (const unsigned char *)key;

    while(len >= 4)
    {
        h += *(unsigned int *)data;
        h *= m;
        h ^= h >> r;

        data += 4;
        len -= 4;
    }

    // Handle the last few bytes of the input array

    switch(len)
    {
        case 3:
            h += data[2] << 16;
        case 2:
            h += data[1] << 8;
        case 1:
            h += data[0];
            h *= m;
            h ^= h >> r;
    };

    // Do a few final mixes of the hash to ensure the last few
    // bytes are well-incorporated.

    h *= m;
    h ^= h >> 10;
    h *= m;
    h ^= h >> 17;

    return h;

}

#endif //HASHTABLE_MY_HASH_FUNCTIONS_H
