#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "physics2.h"

int main(int argc, char **argv)
{
  const Condition cond =
    {
     .width  = 75,
     .height = 40,
     .G = 1.0,
     .dt = 1.0,
     .cor = 0.8
    };
  
  size_t objnum = 2;
  Object objects[objnum];

  // objects[1] は巨大な物体を画面外に... 地球のようなものを想定
  objects[0] = (Object){ .m = 60.0, .y = -19.9, .vy = 2.0};
  objects[1] = (Object){ .m = 100000.0, .y =  1000.0, .vy = 0.0};

  // シミュレーション. ループは整数で回しつつ、実数時間も更新する
  const double stop_time = 400;
  double t = 0;
  for (size_t i = 0 ; t <= stop_time ; i++){
    t = i * cond.dt;
    my_update_velocities(objects, objnum, cond);
    my_update_positions(objects, objnum, cond);
    my_bounce(objects, objnum, cond); // 壁があると仮定した場合に壁を跨いでいたら反射させる
    
    // 表示の座標系は width/2, height/2 のピクセル位置が原点となるようにする
    my_plot_objects(objects, objnum, t, cond);
    
    usleep(200 * 1000); // 200 x 1000us = 200 ms ずつ停止
    printf("\e[%dA", cond.height+3);// 壁とパラメータ表示分で3行
  }
  return EXIT_SUCCESS;
}

// 実習: 以下に my_ で始まる関数を実装する
// 最終的に phisics2.h 内の事前に用意された関数プロトタイプをコメントアウト

void my_update_velocities(Object objs[], const size_t numobj, const Condition cond) {
  for(size_t i = 0; i < numobj; i++) {
    double a = 0.0;
    for(size_t j = 0; j < numobj; j++) {
      if(i == j) continue;
      double temp;
      temp = cond.G * objs[j].m / ((objs[j].y - objs[i].y) * (objs[j].y - objs[i].y));
      if(objs[j].y - objs[i].y < 0) temp *= -1;
      a += temp;
    }

    double vy;
    vy = objs[i].vy + a * cond.dt;
    objs[i].prev_vy = objs[i].vy;
    objs[i].vy = vy;
  }
}

void my_update_positions(Object objs[], const size_t numobj, const Condition cond) {
  double y;
  for(size_t i = 0; i < numobj; i++) {
    y = objs[i].y + objs[i].prev_vy * cond.dt;
    objs[i].y = y;
  }
}

void my_plot_objects(Object objs[], const size_t numobj, const double t, const Condition cond) {
  printf("+");
  for(int i = 0; i < cond.width; i++) {
    printf("-");
  }
  printf("+\n");

  for(int j = 0; j < cond.height; j++) {
    printf("|");
    for(int k = 0; k < cond.width; k++) {
      int f = 0;
      for(size_t i = 0; i < numobj; i++) {
        //座標からTerminal上の位置に変換 (左上が(0, 0))
        double x = (double)(cond.width / 2);
        double y = objs[i].y + (double)(cond.height / 2);
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
    printf("|\n");
  }

  printf("+");
  for(int i = 0; i < cond.width; i++) {
    printf("-");
  }
  printf("+\n");

  printf("t = %5.1lf, ", t);
  for(size_t i = 0; i < numobj; i++) {
    printf("objs[%zu].y = %7.2lf", i, objs[i].y);
    if(i == numobj-1) {
      printf("\n");
    } else {
      printf(", ");
    }
  }
}

void my_bounce(Object objs[], const size_t numobj, const Condition cond) {
  for(size_t i = 0; i < numobj; i++) {
    int iy = (int)round(objs[i].y + (double)(cond.height / 2));
    if(iy < cond.height) {
      continue;
    } else {
      //バウンドの処理 どのスケールで計算すればいいのか？？
      int ex = iy - cond.height + 1;
      int ret = (int)round(ex * cond.cor);
      int niy = cond.height - ret;
      objs[i].y = niy;

    }
  }
}
