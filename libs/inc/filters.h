#ifndef  __FILTERS_H
#define __FILTERS_H

#include "main.h"
// Цифровые фильтры

#define filtrKalman50(F,x) ((F)+=((((x)>(F))?((x)-(F)+1):-((F)-(x)+1))/2))
#define filtrKalman25(F,x) ((F)+=((((x)>(F))?((x)-(F)+2):-((F)-(x)+2))/4))
#define filtrKalman12(F,x) ((F)+=((((x)>(F))?((x)-(F)+4):-((F)-(x)+4))/8))
#define filtrKalman6(F,x)  ((F)+=((((x)>(F))?((x)-(F)+8):-((F)-(x)+8))/16))
#define filtrKalman3(F,x)  ((F)+=((((x)>(F))?((x)-(F)+16):-((F)-(x)+16))/32))
#define filtrKalman1(F,x)  ((F)+=((((x)>(F))?((x)-(F)+32):-((F)-(x)+32))/64))

#define filtrSma(F,x,k) (((F*k+x)/(k+1)))

#endif 
