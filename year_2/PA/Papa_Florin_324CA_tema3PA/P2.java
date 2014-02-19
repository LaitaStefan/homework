//Papa Florin, 324CA

import java.io.BufferedWriter;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.LinkedList;

@SuppressWarnings("unchecked")
public class P2 {
    
    private static final int MAX_CHOICES = 24;
    
    DiceStatus dice[][][];
    int lines, cols;
    int start_x, start_y;
    int stop_x, stop_y;
    int sel_nr[][];
    int chosen[][];
    
    //vectori pentru mutarea pe harta (sus, dreapta, jos, stanga)
    int move_x[] = {-1, 0, 1, 0};
    int move_y[] = {0, 1, 0, -1};
    
    public P2(String file) throws FileNotFoundException, IOException {
        
        InputParser2 ip = new InputParser2(file);
        
        //initializari variabile
        lines = ip.lines;
        cols = ip.cols;
        start_x = ip.start_x;
        start_y = ip.start_y;
        stop_x = ip.stop_x;
        stop_y = ip.stop_y;
        
        sel_nr = new int[lines][cols];
        chosen = new int[lines][cols];
        
        dice = new DiceStatus[lines][cols][MAX_CHOICES];
       
        dice[start_x][start_y][0] = new DiceStatus();
        dice[start_x][start_y][0].faces = ip.dice.clone();
        dice[start_x][start_y][0].x = start_x;
        dice[start_x][start_y][0].y = start_y;
        dice[start_x][start_y][0].sum = ip.dice[0];
        dice[start_x][start_y][0].index = 0;
    }
    
    public boolean isSafe(int l, int c, int d) {
        
        int m1 = l + move_x[d];
        int m2 = c + move_y[d];
        
        return (m1 >= 0) && (m2 >= 0) && (m1 < lines) && (m2 < cols);
    }
    
    public void Dijkstra () {
        
        LinkedList<DiceStatus> queue = new LinkedList();
        
        queue.addLast(dice[start_x][start_y][0]);
        
        while(!queue.isEmpty()) {
            
            DiceStatus ds = queue.removeFirst();
            
            chosen[ds.x][ds.y] = 1;
            
            for(int i = 0; i < 4; i++) {
                
                if(isSafe(ds.x, ds.y, i)) {
                    
                    int line = ds.x + move_x[i];
                    int col = ds.y + move_y[i];
                    
                    if(chosen[line][col] == 1) continue;
                    
                    DiceStatus newN = new DiceStatus();
                    newN.faces = ds.moveDice(i);
                    newN.x = line;
                    newN.y = col;
                    newN.index = sel_nr[line][col];
                    newN.sum = dice[ds.x][ds.y][ds.index].sum + newN.faces[0];
                    
                    int count = 0;
                    
                    for (int j = 0; j < sel_nr[line][col]; j++) {
                            
                        if (dice[line][col][j].equals(newN)) {
                            
                            count++;
                            if (dice[line][col][j].sum > newN.sum) {

                                dice[line][col][j] = newN;
                                break;
                            }
                        }
                    }

                    if (count == 0) {
                        
                        dice[line][col][sel_nr[line][col]] = newN;
                        sel_nr[line][col]++;

                        if (line != stop_x || col != stop_y) {

                            queue.addLast(newN);
                        }
                    }
                }
            }          
        }
    }    
    
    public int getMin() {
        
        int min = dice[stop_x][stop_y][0].sum;
        
        for(int i = 1; i < sel_nr[stop_x][stop_y]; i++) {
            
            if(dice[stop_x][stop_y][i].sum < min) min = dice[stop_x][stop_y][i].sum;
        }
        
        return min;
    }
    
    public static void main(String arg[]) throws FileNotFoundException, IOException {
        
        P2 p = new P2("zar.in");
        BufferedWriter bw = new BufferedWriter(new FileWriter("zar.out"));
        
        p.Dijkstra();
        
        bw.write(p.getMin() + "");
        bw.newLine();
        
        bw.close();
    }
}
