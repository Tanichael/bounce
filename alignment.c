#include <stdio.h>

struct point1 {
  int x;
  int y;
};

struct point2 {
  char c;
  int x;
  int y;
};

int main() {
  printf("%zu\n", sizeof(struct point1));
  printf("%zu\n", sizeof(struct point2));

  double d;
  char c1;
  char c2;
  int i;
  float f;

  printf("%p\n", &d);
  printf("%p\n", &c1);
  printf("%p\n", &c2);
  printf("%p\n", &i);
  printf("%p\n", &f);
}