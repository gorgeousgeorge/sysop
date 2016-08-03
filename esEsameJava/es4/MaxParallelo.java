import java.util.*;

class MaxParallelo{
	public static void main(String[] argv){
		int[] arrToSort=new int[100];
		
		//popola arrToSort con interi pseudorandom
		Random rng=new Random();
		for ( int j=0; j<100; j++ ) 
			arrToSort[j]=rng.nextInt(101); 
		System.out.println( "popolato arrToSort." );
		System.out.println( "Lancio Multisorter." );

		MultiSorter foo=new MultiSorter( arrToSort );
		foo.multiSort();
		System.out.println( "Multisorter ha finito: massimo ="+ arrToSort[99] + "\nVerifico che il risultato sia corretto." );

		//verifico che l'ultima posizione di arrToSort[] contenga effettivamente il massimo:
		//uso sort() della libreria Arrays di java per mettere in ordine crescente arrToSort[]
		//e ne stampo l'ultimo numero

		Arrays.sort(arrToSort);
		System.out.println("Arrays.sort() ha finito: massimo ="+ arrToSort[99]);
	}
}

//contiene il metodo multiSort() che crea gli array di thread responsabili dei 3 stadi di sorting parallelo
class MultiSorter{
	int nTh;
	int arrToSort[];

	public MultiSorter(int arrToSort[]){
		this.arrToSort=arrToSort;
	}

	public void multiSort(){
		//primo round di sorting
		nTh=20;
		sorter roundOne[]=new sorter[nTh];
		//il j-esimo sorter ordinera' gli elementi dell'array di indice (j*5+i), per i in [0,4)
		for ( int j=0; j<nTh; j++){ roundOne[j]=new sorter( arrToSort, j, 5, 1, 0 ); roundOne[j].start(); }
		for ( int j=0; j<nTh; j++){ try{roundOne[j].join();} catch (InterruptedException e){} }

		//secondo round di sorting
		nTh=4;
		sorter roundTwo[]=new sorter[nTh];
		//il j-esimo sorter ordinera' gli elementi dell'array di indice (j*25+i*5+4), per i in [0,4)
		for ( int j=0; j<nTh; j++){ roundTwo[j]=new sorter( arrToSort, j, 25, 5, 4 ); roundTwo[j].start(); }
		for ( int j=0; j<nTh; j++){ try{roundTwo[j].join();} catch (InterruptedException e){} }

		//terzo round di sorting
		sorter roundThree=new sorter( arrToSort, 0, 1, 25, -1 );
		//l' ultimo sorter ordinera' gli elementi dell'array di indice (i*25 -1), per i in [0,4)
		roundThree.start();
		try{roundThree.join();} 
			catch(InterruptedException e){}
	}
}

//thread responsabile dell'ordinamento di 5 elementi di arrToSort, selezionati in base alla combinazione lineare (var*coeffVar + i*coeffInd + offset).
//Variando i parametri 
//var, coeffVar, coeffInd, offset 
//il sorter lavora su sottoinsiemi diversi di elementi di arrToSort.

class sorter extends Thread{

	private int[] arrToSort;
	private int var;
	private int coeffVar;
	private int coeffInd;
	private int offset;
	
	//COSTRUTTORE: 
	//
	//
	//arrToSort=array da ordinare;
	//var=parametro aggiuntivo per creare combinazioni lineari di indici 
	//	al fine di selezionare particolari sotto array;
	//coeffVar= coefficente della variabile var;
	//coeffInd= coefficente dell'indice su cui itera il for() di sort();
	//offset= costante nella comb lineare;
	public sorter(int[] arrToSort, int var, int coeffVar, int coeffInd, int offset){
		this.arrToSort=arrToSort;
		this.var=var;
		this.coeffVar=coeffVar;
		this.coeffInd=coeffInd;
		this.offset=offset;
	}

	//Scambia di posto l'elemento individuato dall'indice i con quello di indice i+1 se il primo e' maggiore del secondo.
	private void sort(){
		int temp;
		for (int i=0; i<4; i++){
			if ( (var*coeffVar+i*coeffInd+offset)>=0 && arrToSort[ var*coeffVar+i*coeffInd+offset ]>arrToSort[ var*coeffVar+(i+1)*coeffInd+offset ] ){
				temp =arrToSort[ var*coeffVar+i*coeffInd+offset ];
				arrToSort[ var*coeffVar+i*coeffInd+offset ] =arrToSort[ var*coeffVar+(i+1)*coeffInd+offset ];
				arrToSort[ var*coeffVar+(i+1)*coeffInd+offset ] =temp;
			}
		}
	}
	
	public void run(){
		sort();
		return;
	}
}
