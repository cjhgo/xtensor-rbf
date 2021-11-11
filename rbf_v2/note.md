

几个问题


+ gemv
+ l2mv
+ gemm
+ l2mm
	- 算权重时, l2(x,x)
	- eval 时, l2(x,y)
+ l2m
	- 由于是对称的, 只需要算上三角或下三角 三角访存,有什么规律
	- 对角线上的值就是 squared norm 的值
	怎么利用这一点...
	- 预计算,怎么利用这一点...
	- 存在着各种对称....


我之前是怎么算 cdist 的 ?

+ 把 cdist 分解成 eigen 能支持的操作


真正的 hpc 怎么算 cdist ...
