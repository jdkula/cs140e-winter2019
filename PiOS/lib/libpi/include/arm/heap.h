//
// Created by jonak on 2/6/19.
//

#ifndef PIOS_HEAP_H
#define PIOS_HEAP_H

void *kmalloc_heap_end(void);
void *kmalloc_heap_start(void);
void *kmalloc(unsigned sz) ;
void kfree(void *p);
void kfree_all(void);

#endif //PIOS_HEAP_H
