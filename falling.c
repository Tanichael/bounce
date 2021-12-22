// どこまでも落ちていく...

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "physics.h" // この中に構造体定義、関数プロトタイプがある

int main(int argc, char **argv)
{
  const Condition cond =
    {
     .width  = 75,
     .height = 40,
     .G = 1.0,
     .dt = 1.0
    };
  
  size_t objnum = 2;
  Object objects[objnum];

  // o[1] は巨大な物体を画面外に... 地球のようなものを想定
  objects[0] = (Object){ .m = 60.0, .y = -20.0, .vy = 0.2};
  objects[1] = (Object){ .m = 100000.0, .y =  1000.0, .vy = 0.0};

  // シミュレーション. ループは整数で回しつつ、実数時間も更新する
  const double stop_time = 400;
  double t = 0;
  for (size_t i = 0 ; t <= stop_time ; i++){
    t = i * cond.dt;
    update_velocities(objects, objnum, cond);
    update_positions(objects, objnum, cond);

    // 表示の座標系は width/2, height/2 のピクセル位置が原点となるようにする
    plot_objects(objects, objnum, t, cond);
    
    usleep(200 * 1000); // 200 x 1000us = 200 ms ずつ停止
    printf("\e[%dA", cond.height+2);// 表示位置を巻き戻す。壁がないのでheight+2行（境界とパラメータ表示分）
  }
  return EXIT_SUCCESS;
}

// 実習: 以下に my_ で始まる関数を実装する
// 最終的には physics.h 内の、こちらが用意したプロトタイプをコメントアウト

void my_update_velocities(Object *objects, const size_t objnum, const Condition cond) {
  for(size_t i = 0; i < objnum; i++) {
    double a = 0.0;
    for(size_t j = 0; j < objnum; j++) {
      if(i == j) continue;
      double temp;
      temp = cond.G * objects[j].m / ((objects[j].y - objects[i].y) * (objects[j].y - objects[i].y));
      if(objects[j].y - objects[i].y < 0) temp *= -1;
      a += temp;
    }

    double vy;
    vy = objects[i].vy + a * cond.dt;
    objects[i].prev_vy = objects[i].vy;
    objects[i].vy = vy;
  }
}

void my_update_positions(Object *objects, const size_t objnum, const Condition cond) {
  double y;
  for(size_t i = 0; i < objnum; i++) {
    y = objects[i].y + objects[i].prev_vy * cond.dt;
    objects[i].y = y;
  }
}

void my_plot_objects(Object *objects, const size_t objnum, double t, const Condition cond) {
  for(int j = 0; j < cond.height; j++) {
    for(int k = 0; k < cond.width; k++) {
      int f = 0;
      for(size_t i = 0; i < objnum; i++) {
        //座標からTerminal上の位置に変換 (左上が(0, 0))
        double x = (double)(cond.width / 2);
        double y = objects[i].y + (double)(cond.height / 2);
        double ix = round(x);
        double iy = round(y);
        if(j == iy && k == ix) {
          printf("o");
          f = 1;
        }
      }
      if(f == 0) {
        printf(" ");
      }
    }
    printf("\n");
  }

  printf("-----\n");
  printf("t = %5.1lf ", t);
  for(size_t i = 0; i < objnum; i++) {
    printf("objs[%zu].y = %5.2lf", i, objects[i].y);
    if(i == objnum-1) {
      printf("\n");
    } else {
      printf(", ");
    }
  }
}
