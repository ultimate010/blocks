股票交易信息下载查询
====================

下载股票交易信息，创建索引,封装了根据时间和代码获取信息的api。

使用方法，down.pl目录下新建down目录和down_index目录，同时创建code.txt文件.</br>

code.txt文件格式为(股票代码 股票名称):</br>

600000  浦发银行</br>

600004	白云机场</br>

600005	武钢股份</br>

600006	东风汽车</br>

600007	中国国贸</br>

600008	首创股份</br>

600009	上海机场</br>

600010	包钢股份</br>

600011	华能国际</br>

600012	皖通高速</br>

600015	华夏银行</br>

首先运行 ./down.pl下载股票交易信息,然后修改stock.c中main函数，取消create_index()函数的
注释(第二次运行不需要运行create_index()函数)，然后make run即可.
</p>
