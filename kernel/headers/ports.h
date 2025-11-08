#ifndef PORT
#define PORT

unsigned char inb(unsigned short port);
void outb(unsigned short port, unsigned char val);

// outw
static inline void outw(unsigned short port, unsigned short val) {
    asm volatile ( "outw %0, %1" : : "a"(val), "Nd"(port) );
}


#endif
