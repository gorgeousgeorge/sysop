import java.util.concurrent.*;

class Spooling{
  public static void main(String argv[]){
    SpoolTable spTable= new SpoolTable(10);
    
    Writer w1=new Writer(spTable);
    Writer w2=new Writer(spTable);
    Writer w3=new Writer(spTable);
    
    Reader r= new Reader(spTable);
    
    r.start();
    w1.start();
    w2.start();
    w3.start();
    
    try{r.join();} catch(InterruptedException e){};
    try{w1.join();} catch(InterruptedException e){};
    try{w2.join();} catch(InterruptedException e){};
    try{w3.join();} catch(InterruptedException e){};
  }
}

//spooling table
class SpoolTable{
  String table[];
  
  int in,out,rows;
  Semaphore empty,full,mutex;
  
  public SpoolTable(int rows){
    this.table=new String[rows];
    this.rows=rows;
    this.in=0;
    this.out=0;
    this.empty=new Semaphore(rows);
    this.full=new Semaphore(0);
    this.mutex=new Semaphore(1);
  }
}
//problema del produttore-consumatore implementato con 2 semafori contatori (full,empty) ed un mutex
//
//consumatore
class Reader extends Thread{
  SpoolTable spTable;
  
  public Reader(SpoolTable spTable){
    this.spTable=spTable;
  }
  
  public void run(){
    while(true){
      try{spTable.full.acquire();} catch(InterruptedException e){};
      try{spTable.mutex.acquire();} catch(InterruptedException e){};
      System.out.println(spTable.table[spTable.out]);
      spTable.out=(spTable.out+1)%spTable.rows;
      spTable.mutex.release();
      spTable.empty.release();
    }
  }
}

//produttore
class Writer extends Thread{
  SpoolTable spTable;
  
  public Writer(SpoolTable spTable){
    this.spTable=spTable;
  }
  
  public void run(){
    while(true){
      try{spTable.empty.acquire();} catch(InterruptedException e){};
      try{spTable.mutex.acquire();} catch(InterruptedException e){};
      spTable.table[spTable.in]="Written by thread n."+this.getId();
      spTable.in=(spTable.in+1)%spTable.rows;
      spTable.mutex.release();
      spTable.full.release();
    }
  }
}
