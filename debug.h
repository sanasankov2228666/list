#ifndef DEBAG_H_
#define DEBAG_H_

#ifdef DEBUG
#define DBG(...) printf(__VA_ARGS__)
#else
#define DBG(...) //
#endif


#endif