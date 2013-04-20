/*
 *  根据股票代码，获取股票交易的详情
 *  王星友
 *  2013-4-3
 */
#ifndef STOCKS_H
#define STOCKS_H

#include <stdio.h>
#include "getFileName.h"
//股票交易数据目录
#define FILEPATH "/home/nlp/projects/blocks/down/"
//索引生成目录，运行生成索引之前为空目录
#define INDEXPATH "/home/nlp/projects/blocks/down_index/"
/*
 *  交易信息结构
 */
typedef struct deal{
  short m_year;char m_month;char m_day;
  float m_open;float m_high;float m_low;float m_close;
  float m_volume;float m_adj_close;
  struct deal* m_next;
}deal;
/*
 *  count表示该结构体的交易条数
 */
typedef struct{
  int m_count;
  deal* m_pData;
}blocks;

/*
 *  每次获取信息后调用该函数释放内存
 */
void freeBlocks(blocks *block){
  deal * pTemp;
  while(block->m_count){
    pTemp=block->m_pData;
    block->m_pData=pTemp->m_next;
    free(pTemp);
    block->m_count -= 1;
  }
}

void getDate(char *buf,short * year,short *month,short *day){
  //2013-04-02,10.07,10.17,9.94,10.03,83681500,10.03
  *day=*month=*year=0;
  int i,temp;
  for(i=0;i<4;i++){
    temp = buf[i]-'0';
    (*year)+=temp;
    (*year)*=10;
  }
  (*year)/=10;
  for(i=5;i<7;i++){
    temp = buf[i] - '0';
    (*month)+=temp;
    (*month)*=10;
  }
  (*month)/=10;
  for(i=8;i<10;i++){
    temp = buf[i] - '0';
    (*day) += temp;
    (*day) *=10;
  }
  (*day)/=10;
}

/*
 *  获取指定时间段内指定股票交易信息
 */
void getInfo(int blockCode,blocks * block,short fromYear,short toYear,short fromMonth,short toMonth,
      short fromDay,short toDay){
  freeBlocks(block); //先释放以前的内存
  FILE *readFp;
  char buf[256];
  sprintf(buf,"%s%d.txt.index\0",INDEXPATH,blockCode);
  if((readFp=fopen(buf,"r"))==NULL){
    perror("No info");
    return ; //返回空的blocks
  }
  long pos=0,toPos=0;
  short year=0;
  while(!feof(readFp)){
    fscanf(readFp,"%d%d",&year,&pos); //两次读取最后一行数据，对结果没有影响
    if(toYear>=year&&toPos==0){
      toPos=pos;
      break;
    }
  }
  /*
   *  打开相应文件，进一步读取内容
   */
  fclose(readFp);
  sprintf(buf,"%s%d.txt\0",FILEPATH,blockCode);
  if((readFp=fopen(buf,"r"))==NULL){
    perror("No source file");
    return ;
  }
  short month,day;
  fseek(readFp,toPos,SEEK_SET);
  char lineBuf[512];
  deal * pTemp;
  long fromTime=fromYear*10000+fromMonth*100+fromDay;
  long toTime = toYear*10000 + toMonth*100+toDay;
  long tempTime;
  while(!feof(readFp)){
    if(fgets(lineBuf,512,readFp)==NULL){
      break;
    }//读一行
    getDate(lineBuf,&year,&month,&day);
    tempTime=year*10000+month*100+day;
    if(toTime>=tempTime){
      if(tempTime<fromTime){
        break;
      }
      //解析字符串
      /*
       * char str[] = "now # is the time for all # good men to come to the # aid of their country";
       * char delims[] = "#";
       * char *result = NULL;
       * result = strtok( str, delims );
       * while( result != NULL ) {
       *     printf( "result is \"%s\"\n", result );
       *     result = strtok( NULL, delims );
       * }
       */
      //2013-04-02,10.07,10.17,9.94,10.03,83681500,10.03
      pTemp = (deal *)malloc(sizeof(deal));
      pTemp->m_year=year;pTemp->m_month=month;pTemp->m_day=day;
      char delims[] = ",";
      char *result=NULL;
      result = strtok(lineBuf,delims); //一个日期跳过
      result = strtok(NULL,delims); //Open
      pTemp->m_open = atof(result);
      result = strtok(NULL,delims); //High
      pTemp->m_high = atof(result);
      result = strtok(NULL,delims); //Low
      pTemp->m_low = atof(result);
      result = strtok(NULL,delims); //Close
      pTemp->m_close = atof(result);
      result = strtok(NULL,delims); //Volume
      pTemp->m_volume = atof(result);
      result = strtok(NULL,delims); //Adj
      pTemp->m_adj_close  = atof(result);
      block->m_count += 1;
      pTemp->m_next= block->m_pData;
      block->m_pData=pTemp;
    }
  }
  fclose(readFp);
}
/*
 *  读取文件，生成索引
 */
void process_index(const char * fileName){
  char buf[256];
  sprintf(buf,"%s%s\0",FILEPATH,fileName);//存放完整路径名
  FILE *readFp,*writeFp;
  if((readFp=fopen(buf,"r"))==NULL){
    printf("file %s",buf);
    perror("Open file failed\n");
    return ;
  }
  sprintf(buf,"%s%s.index\0",INDEXPATH,fileName);
  if((writeFp=fopen(buf,"w+"))==NULL){
    fclose(readFp);
    printf("file %s\n",buf);
    perror("Open out file failed\n");
    return ;
  }
  char lineBuf[512];
  fgets(lineBuf,512,readFp);//跳过第一行
  long pos=0;
  short year,month,day;
  short preYear=0,preMonth=0,preDay=0;
  while(!feof(readFp)){
    //2013-04-02,10.07,10.17,9.94,10.03,83681500,10.03
    pos=ftell(readFp);
    if(fgets(lineBuf,512,readFp)==NULL){
      break;
    }
    getDate(lineBuf,&year,&month,&day);
    //printf("%d %d %d %d\n",year,month,day,pos);
    if(preYear!=year){
      preYear=year;
      fprintf(writeFp,"%d %d\n",year,pos);
    }
    //getchar();
  }
  fclose(readFp);
  fclose(writeFp);
}
void create_index(){
  _fileNameNode *pFile,*pCur;
  pCur=pFile=getFileNameByDir(FILEPATH);
  char buf[256];
  while(pCur){
    printf("processing: %s\n",pCur->fileName);
    sprintf(buf,"%s%s",FILEPATH,pCur->fileName);
    //printf(" %s \n",buf);
    process_index(pCur->fileName);
    pCur=pCur->next;
  }
  freefileNameList(pFile);
  pFile=pCur=NULL;
  freefileNameList(pFile);
  pFile=pCur=NULL;
}
/*
 *  测试用法
 */
int main(){
  //create_index(); //生成索引，执行一次,以后不用执行
  blocks block;
  block.m_count=0;block.m_pData=NULL;
  getInfo(600002,&block,2012,2014,3,4,1,1);
  getInfo(600000,&block,1990,2990,2,2,1,1);
  int i = 0;
  deal * pD = block.m_pData;
  if(block.m_count==0){
    printf("没有数据\n");
  }
  for(i=0;i<block.m_count;i++){
    printf("year:%d month:%d day:%d open:%f high:%f low:%f close:%f volume:%f adj_close:%f \n",
          pD->m_year,pD->m_month,pD->m_day,
          pD->m_open,pD->m_high,pD->m_low,
          pD->m_close,pD->m_volume,pD->m_adj_close);
    pD = pD->m_next;
  }
  freeBlocks(&block);
}
#endif
