import java.util.*;
//import java.util.concurrent.ArrayBlockingQueue;
//import java.util.concurrent.BlockingQueue;



class Pedestrian extends Thread{	//PEDONE
	private BlockingBuffer semBuffer;
	private BlockingBuffer pedBuffer;

	//constructor
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

class TrafficLight extends Thread{	//SEMAFORO
	private BlockingBuffer semBuffer;
	private BlockingBuffer pedBuffer;

	private Random traffic;

	//constructor
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
//			System.out.println("Semaforo ROSSO per i pedoni!");
			try{ semBuffer.take(/*"Semaforo ROSSO per i pedoni!"*/""); } catch(InterruptedException e){}
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
		int delay=r.nextInt(5000);
		System.out.println("Semaforo ROSSO per i pedoni!");
		TrafficLight sem = new TrafficLight(sbuf,pbuf);
		sem.start();

		while(true){
			try { Thread.sleep(delay); } catch(InterruptedException e){}
			new Pedestrian(sbuf,pbuf).start();
		}

		
	}
}
