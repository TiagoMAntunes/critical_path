#include "iterator.h"

typedef struct {
    List ** tableLastIndexes;
    List ** table;
    size_t size;
} Hash;

Hash * createTable(size_t tableSize, size_t structSize);

void insertInTable(Hash * table, void * element, unsigned long id);

void removeFromTable(Hash * table, List * element, unsigned long id);

List * findInTable(Hash * table, int id);

void deleteTable(Hash * table);