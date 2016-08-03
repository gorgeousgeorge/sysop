import java.util.*;

//Contiene una LinkedList e offre un metodo put() ed un metodo take(), entrambi thread-safe, per accedervi
class BlockingBuffer{
	LinkedList buf;
	int MAX_SIZE;
	
	public BlockingBuffer( int size ){
		this.buf = new LinkedList();
		this.MAX_SIZE = size;
	}
	
	//put() prende in input una stringa token da aggiungere al buffer, ed una string msg da stampare a terminale
	public synchronized void put( String token, String msg ) throws InterruptedException{
		if( msg!="" ){
			System.out.println( msg );
		}
		//attende che vi siano dei posti liberi nel buffer
		while( buf.size()==MAX_SIZE ){
			wait();
		}
		buf.add( token );
		notifyAll();
	}

	//take() prende in input una string msg da stampare a terminale, e ritorna la string prelevata dal buffer
	public synchronized String take( String msg ) throws InterruptedException{
		String token;
		//attende che il buffer sia non vuoto
		while( buf.size()==0 ){
			wait();
		}
		if( msg!="" ){	
			System.out.println( msg );
		}
		token = (String)buf.remove();
		notifyAll();

		return token;
	}
}
