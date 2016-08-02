import java.util.*;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;



class Pedestrian extends Thread{	//PEDONE
	private BlockingQueue semBuffer;
	private BlockingQueue pedBuffer;

	//constructor
	public Pedestrian (BlockingQueue semBuffer, BlockingQueue pedBuffer){
		this.semBuffer = semBuffer;
		this.pedBuffer = pedBuffer;
	}

	//produce richiesta di passaggio in semBuffer, consuma OK in pedBuffer
	public void run(){
		try { semBuffer.put("PS"); } catch(InterruptedException e){}
		System.out.println("					Il pedone " + this.getId() + " richiede il passaggio.");
		try { pedBuffer.take().equals("OK"); } catch(InterruptedException e){}
		System.out.println("					Il pedone " + this.getId() + " attraversa la strada.");
			
	}
}

class TrafficLight extends Thread{	//SEMAFORO
	private BlockingQueue semBuffer;
	private BlockingQueue pedBuffer;

	private Random traffic;

	//constructor
	public TrafficLight (BlockingQueue semBuffer, BlockingQueue pedBuffer){
		this.semBuffer = semBuffer;
		this.pedBuffer = pedBuffer;
		this.traffic = new Random();
	}

	//attende una richiesta di passaggio in semBuffer per consumarla.
	//quando accade, cicla in attesa che la variabile traffico sia inferiore a 0.5
	//poi produce un OK in pedBuffer
	public void run(){
		while(true){
			System.out.println("Semaforo ROSSO per i pedoni!");
			try{ semBuffer.take().equals("PS"); } catch(InterruptedException e){}
			while ( traffic.nextFloat() > 0.5 ){
				System.out.println("TROPPO TRAFFICO");
			}
			System.out.println("Semaforo VERDE per i pedoni!");
			try { pedBuffer.put("OK"); } catch(InterruptedException e){}
		}
	}

}

class Incrocio{
	public static void main(String argv[]){
		BlockingQueue sbuf = new ArrayBlockingQueue(5);
		BlockingQueue pbuf = new ArrayBlockingQueue(5);
		Random r=new Random();
		long delay=(long)(200*r.nextFloat());

		TrafficLight sem = new TrafficLight(sbuf,pbuf);
		sem.start();

		while(true){
			try { Thread.sleep(delay); } catch(InterruptedException e){}
			new Pedestrian(sbuf,pbuf).start();
		}

		
	}
}
