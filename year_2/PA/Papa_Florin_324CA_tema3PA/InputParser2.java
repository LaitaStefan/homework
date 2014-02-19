//Papa Florin, 324CA

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

public class InputParser2 {
    
    int dice[];
    int start_x, start_y, stop_x, stop_y;
    int lines, cols;
    
    public InputParser2(String file) throws FileNotFoundException, IOException {
        
        BufferedReader br = new BufferedReader(new FileReader(file));
        
        dice = new int[6];
        
        getData(br);
        
        br.close();      
    }
    
    public void getData(BufferedReader br) throws IOException {
        
        String s, tok[];
        
        s = br.readLine();
        tok = s.split(" ");
        lines = Integer.parseInt(tok[0].trim());
        cols = Integer.parseInt(tok[1].trim());
        
        s = br.readLine();
        tok = s.split(" ");
        
        for(int i = 0; i < 6; i++) {
            
            dice[i] = Integer.parseInt(tok[i].trim());
            
        }
        
        s = br.readLine();
        tok = s.split(" ");
        start_x = Integer.parseInt(tok[0].trim());
        start_y = Integer.parseInt(tok[1].trim());
        
        s = br.readLine();
        tok = s.split(" ");
        stop_x = Integer.parseInt(tok[0].trim());
        stop_y = Integer.parseInt(tok[1].trim());
    }
    
    public int[] cloneDice() {
        
        int aux[] = new int[6];
        
        aux = dice.clone();
        
        return aux;
    }
}
