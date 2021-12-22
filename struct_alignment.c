#include <stdio.h>

typedef struct student
{
  int id;
  char name[100];
  int age;
  double height;
  double weight;
} Student;

typedef struct tagged_student1
{
  int id;
  char name[100];
  int age;
  double height;
  double weight;
  char tag;
} TStudent1;

typedef struct tagged_student2
{
  char tag;
  int id;
  char name[100];
  int age;
  double height;
  double weight;
} TStudent2;

int main (int argc, char**argv)
{
  Student s_begin;
  Student s1;
  TStudent1 s2;
  TStudent2 s3;
  TStudent2 s_end;
  
  /* 以下に各構造体の先頭アドレスを表示するプログラムを書く */
  /* Hint: printf で %p フォーマットを指定する*/
  /* 逆順に表示（s_end, s3, s2, s1, s_begin の順）*/

  // do_something!!
  printf("s_end, %p\n", &s_end);
  printf("s_end.tag, %p\n", &(s_end.tag));
  printf("s_end.id, %p\n", &(s_end.id));
  printf("s_end.name, %p\n", &(s_end.name));
  printf("s_end.age, %p\n", &(s_end.age));
  printf("s_end.height, %p\n", &(s_end.height));
  printf("s_end.weight, %p\n", &(s_end.weight));
  
  printf("s3, %p\n", &s3);
  printf("s3.tag, %p\n", &(s3.tag));
  printf("s3.id, %p\n", &(s3.id));
  printf("s3.name, %p\n", &(s3.name));
  printf("s3.age, %p\n", &(s3.age));
  printf("s3.height, %p\n", &(s3.height));
  printf("s3.weight, %p\n", &(s3.weight));

  printf("s2, %p\n", &s2);
  printf("s2.id, %p\n", &(s2.id));
  printf("s2.name, %p\n", &(s2.name));
  printf("s2.age, %p\n", &(s2.age));
  printf("s2.height, %p\n", &(s2.height));
  printf("s2.weight, %p\n", &(s2.weight));
  printf("s2.tag, %p\n", &(s2.tag));

  printf("s1, %p\n", &s1);
  printf("s1.id, %p\n", &(s1.id));
  printf("s1.name, %p\n", &(s1.name));
  printf("s1.age, %p\n", &(s1.age));
  printf("s1.height, %p\n", &(s1.height));
  printf("s1.weight, %p\n", &(s1.weight));

  printf("s_begin, %p\n", &s_begin);
  printf("s_begin.id, %p\n", &(s_begin.id));
  printf("s_begin.name, %p\n", &(s_begin.name));
  printf("s_begin.age, %p\n", &(s_begin.age));
  printf("s_begin.height, %p\n", &(s_begin.height));
  printf("s_begin.weight, %p\n", &(s_begin.weight));

  /* 以下には各構造体のサイズをsizeof演算子で計算し、表示する */
  /* printf 表示には%zu を用いる*/

  printf("size of TStudent2 = %zu\n", sizeof(TStudent2));
  printf("size of TStudent1 = %zu\n", sizeof(TStudent1));
  printf("size of Student = %zu\n", sizeof(Student));

  // do_something!!
  
  return 0;
}
