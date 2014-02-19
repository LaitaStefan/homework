
//Papa Florin, 324CA

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.Vector;


public class Bot {
    
    Pitch p = new Pitch();
    
    public Bot() {
        super();
    }
    
    
    public String getMessage() {
        try {
            BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
            return br.readLine();
        }

        catch(Exception e) { 
            System.out.println("Eroare la citire"); 
            return null;
        }
    }
    
    public static int Maxi (Pitch p, MyInteger direction){
    	
        if(p.gameOver() == 1) return 1000;

        else if(p.gameOver() == -1) return -1000;

        else if(p.noMoreMoves()) return -1000;
        
        
    	int maxVal = -Integer.MAX_VALUE;
    	int crtVal;
    	Vector<Integer> freePositions = p.getPossibleDirections();
    	
    	for (int i = 0; i < freePositions.size(); i++){
    		
    		Pitch aux_pitch = new Pitch (p.dir, p.currentPoz);
    		
            aux_pitch.move(freePositions.get(i));
        
            if(aux_pitch.visited()) crtVal = Maxi(aux_pitch, new MyInteger(0));
            else crtVal = aux_pitch.eval();
    			
    		if (crtVal > maxVal){
    			maxVal = crtVal;
                direction.set(freePositions.get(i));
    		}
    		
    	}
               
    	return maxVal;
    }
    
    public static void main(String arg[]) {

        Bot b = new Bot();

        try {
            String s = b.getMessage();
            String commands[] = s.split(" ");
            String send = "";
            
            if(commands[0].equals("S")) {   //citire comanda de inceput
                b.p.move(0);
                System.out.print("M "+ 1 + " " + 0 + "\n");
            }
            else if(commands[0].equals("M")) {
                int count = 0;
                b.p.multipleMoves(commands);
                MyInteger md = new MyInteger((0));

                b.Maxi(b.p, md);
                
                b.p.move(md.n);
                count++;
                send = send + " " + md.n;

                System.out.print("M " + count + send + "\n");
            }
            
            while(!(s = b.getMessage()).equals("F")) { //citire restul comenzilor

                String comm[] = s.split(" ");
                int count = 0;      //numarul de mutari la fiecare pas
                send = "";

                b.p.multipleMoves(comm); //execut mutarile adversarului

                MyInteger md = new MyInteger((0)); //variabila in care salvez mutarea pe care 
                                                   //trebuie sa o fac
                b.Maxi(b.p, md);
                
                b.p.move(md.n);
                count++;
                send = send + " " + md.n;    

                if(b.p.gameOver() != 0 || b.p.noMoreMoves()) {
                    System.out.print("M " + count + send + "\n");
                    continue;
                }

                while ( b.p.visited()) {
                    b.Maxi(b.p, md);
                    b.p.move(md.n);
                    count++;
                    send = send + " " + md.n;
                    if(b.p.gameOver() != 0 || b.p.noMoreMoves()) break;
                }

                System.out.print("M " + count + send + "\n");
            }
        }
        catch(Exception e) { System.out.println("Eroare la citirea mesajului"); }
    }
}

