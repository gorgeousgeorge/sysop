import java.util.*;

class BlockingBuffer{
	LinkedList buf;
	int MAX_SIZE;
	
	public BlockingBuffer( int size ){
		this.buf = new LinkedList();
		this.MAX_SIZE = size;
	}

	public synchronized void put( String token, String msg ) throws InterruptedException{
		if( msg!="" ){
			System.out.println( msg );
		}
		while( buf.size()==MAX_SIZE ){
			wait();
		}
		buf.add( token );
		notifyAll();
	}

	public synchronized String take( String msg ) throws InterruptedException{
		String token;
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
