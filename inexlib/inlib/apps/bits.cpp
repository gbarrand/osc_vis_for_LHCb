#include <cstdio>
int main() {
  for(int i=0;i<32;i++) {
    ::printf("%d : %u %x\n",i,(1<<i),(1<<i));
  }
  return 0;
}
