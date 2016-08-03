//IncrocioBB utilizza la classe BlockingBuffer, una reimplementazione della classe BlockingQueue di java;
//BlockingBuffer.class deve essere contenuto nella stessa directory in cui si vuole eseguire IncrocioBB.
//I dettagli sulla reimplementazione si trovano nel sorgente BlockingBuffer.java.

import java.util.*;


//PEDONE
class Pedestrian extends Thread{	
	private BlockingBuffer semBuffer;
	private BlockingBuffer pedBuffer;

	public Pedestrian (BlockingBuffer semBuffer, BlockingBuffer pedBuffer){
		this.semBuffer = semBuffer;
		this.pedBuffer = pedBuffer;
	}

	//produce richiesta di passaggio in semBuffer, consuma OK in pedBuffer
	public void run(){
		try { semBuffer.put("PS","	Il pedone " + this.getId() + " richiede il passaggio."); } catch(InterruptedException e){}
		try { pedBuffer.take("	Il pedone " + this.getId() + " attraversa la strada.\nSemaforo ROSSO per i pedoni!"); } catch(InterruptedException e){}
	}
}

//SEMAFORO
class TrafficLight extends Thread{	
	//semBuffer contiene le richieste di passaggio dei pedoni
	private BlockingBuffer semBuffer;
	//pedBuffer contiene i via libera del semaforo
	private BlockingBuffer pedBuffer;

	private Random traffic;

	public TrafficLight (BlockingBuffer semBuffer, BlockingBuffer pedBuffer){
		this.semBuffer = semBuffer;
		this.pedBuffer = pedBuffer;
		this.traffic = new Random();
	}

	//attende una richiesta di passaggio in semBuffer per consumarla.
	//quando accade, cicla in attesa che la variabile traffico sia inferiore a 0.5
	//poi produce un OK in pedBuffer
	public void run(){
		while(true){
			try{ semBuffer.take(""); } catch(InterruptedException e){}
			while ( traffic.nextFloat() > 0.5 ){
				System.out.println("TROPPO TRAFFICO");
			}
			try { pedBuffer.put("OK","Semaforo VERDE per i pedoni!"); } catch(InterruptedException e){}
		}
	}

}

class IncrocioBB{
	public static void main(String argv[]){
		BlockingBuffer sbuf = new BlockingBuffer(5);
		BlockingBuffer pbuf = new BlockingBuffer(5);
		Random r=new Random();
		int delay;
		//creo un thread semaforo, inizialmente rosso per i pedoni
		System.out.println("Semaforo ROSSO per i pedoni!");
		TrafficLight sem = new TrafficLight(sbuf,pbuf);
		sem.start();
		
		//genera infiniti thread pedone, aspettando un tempo casuale (inferiore a 5 secondi) tra un pedone ed il successivo
		while(true){
			delay=r.nextInt(5000);
			try { Thread.sleep(delay); } catch(InterruptedException e){}
			new Pedestrian(sbuf,pbuf).start();
		}

		
	}
}
