how to run

1. run rbf-py.ipynb
2. mkdir build
3. cd build
4. cmake .. -DCMAKE_BUILE_TYPE=RELEASE
5. make -j VERBOSE=TRUE
6. cp ../data .
7. ./xtrbf



比较

1. xt 中 HAVE_CBLAS 编译选项的作用
怎样让它使用mklblas(arm上没用)
2. xt 中使用 osx lapack 与 brew lapack 的区别
3. eigen 中使用 原生 solve 与 lapacke solve的作用

3. 

```
> pwd
/Users/junhangc/anaconda3/include/xflens/cxxblas/drivers
.
├── atlas.h
├── cblas.h
├── drivers.h
├── drivers.tcc
├── gotoblas.h
├── mklblas.h
├── openblas.h
├── refblas.h
├── sparseblas.h
└── veclib.h
```
...


20:00-21:00 白搞一通...还是没让obj使用brew 自带的lapack
能用就行了...好了...

chave_blas 加不加,区别不大,官方推荐,还是加上吧....