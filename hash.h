#include "iterator.h"

typedef struct {
    List ** table;
    size_t size;
} Hash;

Hash * createTable(size_t tableSize, size_t structSize);

void insertInTable(Hash * table, void * element, long long id);

void removeFromTable(Hash * table, List * element, long long id);

List * findInTable(Hash * table, int id);