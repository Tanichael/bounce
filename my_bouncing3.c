#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include "physics2.h"

int main(int argc, char **argv)
{
  const Condition cond =
    {
     .width  = 75,
     .height = 40,
     .G = 1.0,
     .dt = 1.0,
     .cor = 0.8,
     .border = 100.0
    };

  size_t objnum;
  if(argc == 1) {
    objnum = 2;
  } else if(argc == 2) {
    return 0;
  } else if(argc == 3) {
    objnum = atoi(argv[1]);
  }
  
  Object objects[objnum];

  if(argc == 1) {
    objects[0] = (Object){ .m = 60.0, .x = 0.0, .y = -19.9, .vx = 0.0, .vy = 0.0};
    objects[1] = (Object){ .m = 100000.0, .x = 0.0, .y =  1000.0, .vx = 0.0, .vy = 0.0};
  } else if(argc == 3) {
    const size_t bufsize = 200;
    char buf[bufsize];
    FILE *fp = fopen(argv[2], "r");

    for(int i = 0; i < objnum; i++) {
      if(fgets(buf, bufsize, fp) == NULL) {
        objects[i] = (Object){ .m = 0.0, .x = 0.0, .y = 0.0, .vx = 0.0, .vy = 0.0};
        continue;
      } else {
        double m;
        double x;
        double y;
        double vx;
        double vy;

        while(strcmp(buf, "#\n") != 0) {
          char *ptr;
          ptr = strtok(buf, " ");

          char *mem = ptr;
          while(ptr != NULL) {
            ptr = strtok(NULL, " ");
            
            if(ptr != NULL) {
              int len = strlen(ptr);
              ptr[len-1] = '\0';
              if(strcmp(mem, "m") == 0) {
                m  = atof(ptr);
              } else if(strcmp(mem, "x") == 0) {
                x  = atof(ptr);
              } else if(strcmp(mem, "y") == 0) {
                y  = atof(ptr);
              } else if(strcmp(mem, "vx") == 0) {
                vx  = atof(ptr);
              } else if(strcmp(mem, "vy") == 0) {
                vy  = atof(ptr);
              }
            }
          }
          
          fgets(buf, bufsize, fp);
        }
        objects[i] = (Object){ .m = m, .x = x, .y = y, .vx = vx, .vy = vy};
      }
    }
  }

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

void my_update_velocities(Object objs[], const size_t numobj, const Condition cond) {
  //地球と物体はy方向にかなり遠い距離にあるのでx方向には加速しないと考える
  for(size_t i = 0; i < numobj; i++) {
    double ax = 0.0;
    double ay = 0.0;
    for(size_t j = 0; j < numobj; j++) {
      if(i == j) continue;
      double tempax;
      double tempay;

      if(objs[j].x - objs[i].x == 0.0 || i == numobj - 1 || j == numobj - 1) {
        tempax = 0.0;
      } else {
        tempax = cond.G * objs[j].m / ((objs[j].x - objs[i].x) * (objs[j].x - objs[i].x));
      }
      if(objs[j].x - objs[i].x < 0) tempax *= -1;
      ax += tempax;

      if(objs[j].y - objs[i].y == 0.0) {
        tempay = 0;
      } else {
        tempay = cond.G * objs[j].m / ((objs[j].y - objs[i].y) * (objs[j].y - objs[i].y));
      }
      if(objs[j].y - objs[i].y < 0) tempay *= -1;
      ay += tempay;
    }
    double vx;
    double vy;
    vx = objs[i].vx + ax * cond.dt;
    vy = objs[i].vy + ay * cond.dt;
    objs[i].prev_vx = objs[i].vx;
    objs[i].prev_vy = objs[i].vy;
    objs[i].vx = vx;
    objs[i].vy = vy;
  }
}

void my_update_positions(Object objs[], const size_t numobj, const Condition cond) {
  double x;
  double y;
  for(size_t i = 0; i < numobj; i++) {
    x = objs[i].x + objs[i].prev_vx * cond.dt;
    y = objs[i].y + objs[i].prev_vy * cond.dt;
    objs[i].x = x;
    objs[i].y = y;
  }

  for(size_t i = 0; i < numobj; i++) {
    for(size_t j = i + 1; j < numobj; j++) {
      double dist;
      dist = (objs[i].x - objs[j].x) * (objs[i].x - objs[j].x) + (objs[i].y - objs[j].y) * (objs[i].y - objs[j].y);
      if(dist < cond.border) {
        //融合処理
        //運動量保存処理
        double tempvx;
        double tempvy;
        tempvx = (objs[i].m * objs[i].vx + objs[j].m * objs[j].vx) / (objs[i].m + objs[j].m);
        tempvy = (objs[i].m * objs[i].vy + objs[j].m * objs[j].vy) / (objs[i].m + objs[j].m);

        objs[i].vx = tempvx;
        objs[j].vx = tempvx;
        objs[i].vy = tempvy;
        objs[j].vy = tempvy;

        //融合させる
        objs[j].x = objs[i].x;
        objs[j].y = objs[i].y;

      }
    }
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
        double x = objs[i].x + (double)(cond.width / 2);
        double y = objs[i].y + (double)(cond.height / 2);
        double ix = round(x);
        double iy = round(y);
        if(j == iy && k == ix && f == 0) {
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
    printf("objs[%zu].x = %7.2lf, ", i, objs[i].x);
    printf("objs[%zu].y = %7.2lf", i, objs[i].y);
    if(i == numobj-1) {
      printf("\n");
    } else {
      printf(", ");
    }
  }
}

void my_bounce(Object objs[], const size_t numobj, const Condition cond) {
  for(size_t i = 0; i < numobj - 1; i++) {
    //i == numobj - 1の時は地球なのでバウンド処理はしない感じでいい？
    int ix = (int)round(objs[i].x + (double)(cond.width / 2));
    int iy = (int)round(objs[i].y + (double)(cond.height / 2));
    //x方向バウンド処理
    if(ix < 0) {
      // 位置を反転させる
      int ex = - ix;
      int ret = (int)round(ex * cond.cor);
      int nix = ret;
      objs[i].x = (int)round(nix - (double)(cond.width / 2));
      objs[i].vx = objs[i].vx * cond.cor * (-1);
    }
    if(ix >= cond.width) {
      // 位置を反転させる
      int ex = ix - cond.width + 1;
      int ret = (int)round(ex * cond.cor);
      int nix = cond.width - ret;
      objs[i].x = (int)round(nix - (double)(cond.width / 2));
      objs[i].vx = objs[i].vx * cond.cor * (-1);
    }

    //y方向バウンド処理
    if(iy >= cond.height) {
      // バウンドの処理 どのスケールで計算すればいいのか？？
      // 位置を反転させる
      int ex = iy - cond.height + 1;
      int ret = (int)round(ex * cond.cor);
      int niy = cond.height - ret;
      objs[i].y = (int)round(niy - (double)(cond.height / 2));
      objs[i].vy = objs[i].vy * cond.cor * (-1);
    }
  }
}
