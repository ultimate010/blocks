#说明在当前目录下需要code.txt文件指明股票代码
#当前目录下新建down目录，下载结果均在down目录下的
use LWP::Simple;

$url0="http://ichart.finance.yahoo.com/table.csv?s=";
$url1=".SS&a=10&b=10&c=1999&d=3&e=3&f=2013&g=d&ignore=.csv";
$url="http://ichart.finance.yahoo.com/table.csv?s=600000.SS&a=10&b=10&c=1999&d=3&e=3&f=2013&g=d&ignore=.csv";
$fileName="./down/abd.txt";

open(IN,"code.txt");
while(<IN>){
	chomp;
	if(~/(\d+)(.+)/){
		sleep 60;
		print "Downloading:$1 $2\n";
		$url=$url0.$1.$url1; $fileName="./down/".$1.".txt";
		getstore($url,$fileName) || die "get $1 $2 failed!\n";
	}
}
close(IN);
