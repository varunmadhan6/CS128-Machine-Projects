#include <iostream>

#include "cord_utilities.hpp"
#include "cord.hpp"

int main() {
  //SharedPointer<Cord> a = new Cord("he");
  //SharedPointer<Cord> b = new Cord("llo");
  //SharedPointer<Cord> c = new Cord("wor");
  //SharedPointer<Cord> d = new Cord("ld");
  //SharedPointer<Cord> e = ConcatCords(a, b);
  //SharedPointer<Cord> f = ConcatCords(c, d);
  //SharedPointer<Cord> g = ConcatCords(e, f);
  SharedPointer<Cord> h = new Cord("sse");
  SharedPointer<Cord> i = new Cord("ssion");
  SharedPointer<Cord> j = ConcatCords(h, i);
  SharedPointer<Cord> k = new Cord("po");
  SharedPointer<Cord> l = ConcatCords(k, j);
  SharedPointer<Cord> m = new Cord("pre");
  SharedPointer<Cord> n = ConcatCords(m, l);

   auto res = SubString(n, 2, 13);
   auto res_str = res->ToString();
   std::cout << res_str.Get() << std::endl;

   res_str = res->Left()->ToString();
   std::cout << res_str.Get() << std::endl;

   res_str = res->Right()->Left()->ToString();
   std::cout << res_str.Get() << std::endl;

   res_str = res->Right()->Right()->Left()->ToString();
   std::cout << res_str.Get() << std::endl;

   res_str = res->Right()->Right()->Right()->ToString();
   std::cout << res_str.Get() << std::endl;

   auto r = res->Length();
   std::cout << r << std::endl;

   r = res->Left()->Length();
   std::cout << r << std::endl;

   r = res->Right()->Length();
   std::cout << r << std::endl;

  r = res->Right()->Right()->Length();   
  std::cout << r << std::endl;

  if (i->Data() == res->Right()->Right()->Right()->Data()) {
    std::cout << "yes" << std::endl;
  }
}